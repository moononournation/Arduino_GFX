#pragma once

#include "Arduino_DataBus.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)

#ifndef LCD_MAX_PIXELS_AT_ONCE
#define LCD_MAX_PIXELS_AT_ONCE 2046
#endif
#ifndef USE_DMA_THRESHOLD
#define USE_DMA_THRESHOLD 6
#endif

#if (!defined(ESP_ARDUINO_VERSION_MAJOR)) || (ESP_ARDUINO_VERSION_MAJOR < 3)

class Arduino_ESP32LCD8 : public Arduino_DataBus
{
public:
  Arduino_ESP32LCD8(
      int8_t dc, int8_t cs, int8_t wr, int8_t rd,
      int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7); // Constructor

  bool begin(int32_t speed = GFX_NOT_DEFINED, int8_t dataMode = GFX_NOT_DEFINED) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void writeCommandBytes(uint8_t *data, uint32_t len) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;

  void writeC8D8(uint8_t c, uint8_t d) override;
  void writeC8D16(uint8_t c, uint16_t d) override;
  void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2) override;
  void writeC8D16D16Split(uint8_t c, uint16_t d1, uint16_t d2) override;

  void writeRepeat(uint16_t p, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

  void writeBytes(uint8_t *data, uint32_t len) override;

  void writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len) override;
  void writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len) override;

protected:
private:
  GFX_INLINE void CS_HIGH(void);
  GFX_INLINE void CS_LOW(void);

  int8_t _dc, _cs, _wr, _rd;
  int8_t _d0, _d1, _d2, _d3, _d4, _d5, _d6, _d7;

  PORTreg_t _csPortSet; ///< PORT register for chip select SET
  PORTreg_t _csPortClr; ///< PORT register for chip select CLEAR
  uint32_t _csPinMask;  ///< Bitmask for chip select

  esp_lcd_i80_bus_handle_t _i80_bus = nullptr;
  dma_descriptor_t *_dmadesc = nullptr;
  gdma_channel_handle_t _dma_chan;

  union
  {
    uint32_t value;
    struct
    {
      uint16_t value16;
      uint16_t value16_2;
    };
    struct
    {
      uint8_t lsb;
      uint8_t msb;
      uint8_t lsb_2;
      uint8_t msb_2;
    };
  } _data32;

  union
  {
    uint8_t* _buffer;
    uint16_t* _buffer16;
    uint32_t* _buffer32;
  };
};

#else // (ESP_ARDUINO_VERSION_MAJOR >= 3)

#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include <esp_private/gdma.h>
#include <hal/dma_types.h>

// The Arduino_ESP32LCD8 bus can send bytes. Sending 16-bit commands is not supported.
// Sending 16-bit pixel data is sending in msb,lsb order.

class Arduino_ESP32LCD8 : public Arduino_DataBus
{
public:
  Arduino_ESP32LCD8(
      int8_t dc, int8_t cs, int8_t wr, int8_t rd,
      int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7); // Constructor

  bool begin(int32_t speed = GFX_NOT_DEFINED, int8_t dataMode = GFX_NOT_DEFINED) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void writeCommandBytes(uint8_t *data, uint32_t len) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;

  void writeRepeat(uint16_t p, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

  void writeBytes(uint8_t *data, uint32_t len) override;

  void writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len) override;
  void writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len) override;

protected:
private:

 // flush _cmd and _buffer 
  void flushBuffer();

  int8_t _dc, _cs, _wr, _rd;
  int8_t _d0, _d1, _d2, _d3, _d4, _d5, _d6, _d7;

  PORTreg_t _csPortSet; ///< PORT register for chip select SET
  PORTreg_t _csPortClr; ///< PORT register for chip select CLEAR
  uint32_t _csPinMask;  ///< Bitmask for chip select

  esp_lcd_i80_bus_handle_t _i80_bus = nullptr;
  esp_lcd_panel_io_handle_t _io_handle = nullptr;

  dma_descriptor_t *_dmadesc = nullptr;
  gdma_channel_handle_t _dma_chan;

  // record command and parameters in _cmd and _buffer

  /// command byte for next DMA transfer
  int _cmd = -1;

  /// buffer is in use for color data
  bool _isColor = false;

  /// data size in _buffer for next DMA transfer
  int _bufferLen = 0;

  union
  {
    uint32_t value;
    struct
    {
      uint16_t value16;
      uint16_t value16_2;
    };
    struct
    {
      uint8_t lsb;
      uint8_t msb;
      uint8_t lsb_2;
      uint8_t msb_2;
    };
  } _data32;

  union
  {
    uint8_t* _buffer;
    uint16_t* _buffer16;
    uint32_t* _buffer32;
  };
};

#endif // #if ... ESP_ARDUINO_VERSION_MAJOR

#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
