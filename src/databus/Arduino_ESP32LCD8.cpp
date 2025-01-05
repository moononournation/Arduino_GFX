/*
 * start rewrite from:
 * https://github.com/lovyan03/LovyanGFX/blob/master/src/lgfx/v0/platforms/LGFX_PARALLEL_ESP32.hpp
 * https://github.com/espressif/esp-bsp/tree/a7de01ba3b38edd6198903a84855b04a12b15003/components/lcd/esp_lcd_st7796
 * https://components.espressif.com/components/espressif/esp_lcd_st7796/versions/1.3.0
 * https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32s3/api-reference/peripherals/lcd.html
 * see https://docs.espressif.com/projects/esp-idf/en/v5.1.2/esp32s3/api-reference/peripherals/lcd.html#i80-interfaced-lcd
 * and https://github.com/espressif/esp-idf/blob/master/examples/peripherals/lcd/i80_controller/main/i80_controller_example_main.c
 * https://github.com/espressif/esp-idf/pull/8644/files
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/lcd/index.html
 */
#include "Arduino_ESP32LCD8.h"

#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include <esp_private/gdma.h>
#include <hal/dma_types.h>

// #include <esp_lcd_panel_io_interface.h>
// #include <esp_pm.h>

#ifndef LCD_MAX_PIXELS_AT_ONCE
#define LCD_MAX_PIXELS_AT_ONCE (2048)
#endif

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
#if 1 || (!defined(ESP_ARDUINO_VERSION_MAJOR)) || (ESP_ARDUINO_VERSION_MAJOR >= 5)

/**
 * @brief Construct a new Arduino_ESP32LCD8::Arduino_ESP32LCD8 object
 */
Arduino_ESP32LCD8::Arduino_ESP32LCD8(
    int8_t dc, int8_t cs, int8_t wr, int8_t rd,
    int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7)
    : _dc(dc), _cs(cs), _wr(wr), _rd(rd),
      _d0(d0), _d1(d1), _d2(d2), _d3(d3), _d4(d4), _d5(d5), _d6(d6), _d7(d7)
{
}

/**
 * @brief begin
 */
bool Arduino_ESP32LCD8::begin(int32_t speed, int8_t dataMode)
{
  if (speed == GFX_NOT_DEFINED)
  {
    _speed = 40000000UL;
  }
  else
  {
    _speed = speed;
  }

  if (_cs != GFX_NOT_DEFINED)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // disable chip select
  }

  pinMode(_wr, OUTPUT);
  digitalWrite(_wr, HIGH); // Set write strobe high (inactive)

  if (_rd != GFX_NOT_DEFINED)
  {
    pinMode(_rd, OUTPUT);
    digitalWrite(_rd, HIGH);
  }

  esp_lcd_i80_bus_config_t bus_config = {
      .dc_gpio_num = _dc,
      .wr_gpio_num = _wr,
      .clk_src = LCD_CLK_SRC_PLL160M, // ??? LCD_CLK_SRC_DEFAULT,
      .data_gpio_nums = {_d0, _d1, _d2, _d3, _d4, _d5, _d6, _d7},
      .bus_width = 8,
      .max_transfer_bytes = LCD_MAX_PIXELS_AT_ONCE * 2,
      .dma_burst_size = 64,
      // .sram_trans_align = 4
  };
  esp_lcd_new_i80_bus(&bus_config, &_i80_bus);

  Serial.printf("max_transfer_bytes=%zu\n", bus_config.max_transfer_bytes);

  esp_lcd_panel_io_i80_config_t io_config = {
      .cs_gpio_num = _cs,
      .pclk_hz = 10 * 1000 * 1000, // ??? _speed,
      .trans_queue_depth = 10,
      // on_color_trans_done = nullptr,
      // user_ctx = nullptr,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
      .dc_levels = {
          .dc_idle_level = 0,
          .dc_cmd_level = 0,
          .dc_dummy_level = 0,
          .dc_data_level = 1,
      },
      .flags = {
          .swap_color_bytes = 1,
          .pclk_idle_low = 0,
      },
  };
  esp_lcd_new_panel_io_i80(_i80_bus, &io_config, &_io_handle);

  // allocate some DMA buffer
  // _buffer = (uint8_t *)heap_caps_aligned_alloc(16, LCD_MAX_PIXELS_AT_ONCE * 2 + 16, MALLOC_CAP_DMA);
  _cmd = -1;
  _isColor = false;
  _bufferLen = 0;
  _buffer = (uint8_t *)esp_lcd_i80_alloc_draw_buffer(_io_handle, LCD_MAX_PIXELS_AT_ONCE * 2 + 16, MALLOC_CAP_DMA);

  if (!_buffer)
  {
    return false;
  }

  return (true);
}

/**
 * @brief start any command or data sequence
 */
void Arduino_ESP32LCD8::beginWrite()
{
  _cmd == -1;
  _bufferLen = 0;
}

/**
 * @brief end any command or data sequence
 */
void Arduino_ESP32LCD8::endWrite()
{
  flushBuffer();
}

/**
 * @brief write a byte command
 * @param c the command byte
 */
void Arduino_ESP32LCD8::writeCommand(uint8_t c)
{
  flushBuffer();
  Serial.printf("writeCommand(%02x)\n", c);
  _cmd = c;
  _isColor = false;
}

/**
 * @brief write a word command
 * @param c the command word
 */
void Arduino_ESP32LCD8::writeCommand16(uint16_t c)
{
  Serial.printf("ESP32LCD8::writeCommand16 not implemented.\n");
}

/** write a set of data as command + params */
void Arduino_ESP32LCD8::writeCommandBytes(uint8_t *data, uint32_t len)
{
  flushBuffer();
  Serial.printf("**writeCommandBytes()\n");

  if (len)
  {
    _cmd = *data++;
    _isColor = false;
    len--;
  }

  while (len)
  {
    write(*data++);
    len--;
  }
}

/**
 * @brief write a byte to the param buffer.
 * @param d data byte
 */
void Arduino_ESP32LCD8::write(uint8_t d)
{
  _buffer[_bufferLen++] = d;
  if (_bufferLen >= LCD_MAX_PIXELS_AT_ONCE * 2)
  {
    flushBuffer();
  }
}

/**
 * @brief write16 - write 16-bit to the param buffer.
 * @param d data word
 */
void Arduino_ESP32LCD8::write16(uint16_t d)
{
  _data16.value = d;
  _buffer[_bufferLen++] = _data16.msb;
  _buffer[_bufferLen++] = _data16.lsb;

  if (_bufferLen >= LCD_MAX_PIXELS_AT_ONCE * 2 - 4)
  {
    flushBuffer();
  }
}

/**
 * @brief writeRepeat
 *
 * @param p
 * @param len
 */
void Arduino_ESP32LCD8::writeRepeat(uint16_t p, uint32_t len)
{
  Serial.printf("  writeRepeat(#%04x, %d)\n", p, len);
  _isColor = true;

  while (len--)
  {
    write16(p);
  }

  // // fill buffer
  // uint32_t buf16Len = (len > LCD_MAX_PIXELS_AT_ONCE) ? LCD_MAX_PIXELS_AT_ONCE : len;
  // _buffer[0] = p >> 8;
  // _buffer[1] = p & 0x00ff;
  // uint16_t c16 = _buffer16[0];
  // for (uint32_t i = 0; i < buf16Len; i++)
  // {
  //   _buffer16[i] = c16;
  // }

  // while (len > 0)
  // { // while there are still pixels to write
  //   uint32_t txlen16 = (len > LCD_MAX_PIXELS_AT_ONCE) ? LCD_MAX_PIXELS_AT_ONCE : len;
  //   _bufferLen = txlen16 * 2;
  //   flushBuffer();
  //   len -= txlen16;
  // }
}

/**
 * @brief write Pixel data words to the buffer.
 * @param data array of Pixel data words
 * @param len length of data array
 */
void Arduino_ESP32LCD8::writePixels(uint16_t *data, uint32_t len)
{
  Serial.printf("  writePixels( [...], %ld)\n", len);
  _isColor = true;

  // transfer in chunks
  while (len--)
  {
    write16(*data++);
  }
}

/**
 * @brief write data bytes to the buffer
 * @param data array of data bytes
 * @param len length of data array
 */
void Arduino_ESP32LCD8::writeBytes(uint8_t *data, uint32_t len)
{
  Serial.printf("  writeBytes( [...], %ld)\n", len);
  // transfer in chunks
  while (len--)
  {
    write(*data++);
  }
}

/**
 * @brief writeIndexedPixels
 *
 * @param data
 * @param idx
 * @param len
 */
void Arduino_ESP32LCD8::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  Serial.printf("**writeIndexedPixels(...)\n");
}

/**
 * @brief writeIndexedPixelsDouble
 *
 * @param data
 * @param idx
 * @param len
 */
void Arduino_ESP32LCD8::writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len)
{
  Serial.printf("**writeIndexedPixelsDouble(...)\n");
}

// ===== sending the buffer =====

// for details, see
// <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/lcd/index.html>

void Arduino_ESP32LCD8::flushBuffer()
{

  if (_cmd == 0x2c)
  {
    if (!_isColor)
      Serial.printf("  isColor mismatch\n");
      _isColor = true;
  }

  if ((_cmd >= 0) || (_bufferLen > 0))
  {
    Serial.printf("  flush(%02x: (%d)", _cmd, _bufferLen);

    for (int n = 0; (n < _bufferLen) && (n < 32); n++)
    {
      Serial.printf(" %02x", _buffer[n]);
    }

    // wait for color completion (when color sending is on the way)
    // send cmd and buffer and wait for completion

    if (_isColor)
    // if (_bufferLen > 32)
    // if (_cmd == 0x2c9)
    {
      // async DMA transfer
      esp_lcd_panel_io_tx_color(_io_handle, _cmd, _buffer, _bufferLen);
    }
    else
    {
      esp_lcd_panel_io_tx_param(_io_handle, _cmd, _buffer, _bufferLen);
    }
    // if (!_bufferLen)
    // {
    //   _cmd = -1; // next time, we start a data send out without command.
    // }
    _cmd = -1; // next time, we start a data send out without command.
    _bufferLen = 0;
    Serial.println(")");
  }
}

#endif // #if (!defined(ESP_ARDUINO_VERSION_MAJOR)) || (ESP_ARDUINO_VERSION_MAJOR < 3)
#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
