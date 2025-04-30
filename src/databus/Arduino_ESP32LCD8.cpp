#include "Arduino_ESP32LCD8.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
#if (!defined(ESP_ARDUINO_VERSION_MAJOR)) || (ESP_ARDUINO_VERSION_MAJOR < 3)

#define WAIT_LCD_NOT_BUSY while (LCD_CAM.lcd_user.val & LCD_CAM_LCD_START)

/**
 * @brief Construct a new Arduino_ESP32LCD8::Arduino_ESP32LCD8 object
 *
 * @param dc
 * @param cs
 * @param wr
 * @param rd
 * @param d0
 * @param d1
 * @param d2
 * @param d3
 * @param d4
 * @param d5
 * @param d6
 * @param d7
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
 *
 * @param speed
 * @param dataMode
 * @return true
 * @return false
 */
bool Arduino_ESP32LCD8::begin(int32_t speed, int8_t dataMode)
{
  if (speed == GFX_NOT_DEFINED)
  {
    _speed = 20000000UL; // safe frequency
  }
  else
  {
    _speed = speed;
  }

  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode

  if (_cs != GFX_NOT_DEFINED)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // disable chip select
  }
  if (_cs >= 32)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t)GPIO_OUT1_W1TS_REG;
    _csPortClr = (PORTreg_t)GPIO_OUT1_W1TC_REG;
  }
  else if (_cs != GFX_NOT_DEFINED)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t)GPIO_OUT_W1TS_REG;
    _csPortClr = (PORTreg_t)GPIO_OUT_W1TC_REG;
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
      .clk_src = LCD_CLK_SRC_PLL160M,
      .data_gpio_nums = {
          _d0, _d1, _d2, _d3, _d4, _d5, _d6, _d7},
      .bus_width = 8,
      .max_transfer_bytes = LCD_MAX_PIXELS_AT_ONCE * 2,
      .psram_trans_align = 0,
      .sram_trans_align = 0};
  esp_lcd_new_i80_bus(&bus_config, &_i80_bus);

  uint32_t diff = INT32_MAX;
  uint32_t div_n = 256;
  uint32_t div_a = 63;
  uint32_t div_b = 62;
  uint32_t clkcnt = 64;
  uint32_t start_cnt = std::min<uint32_t>(64u, (F_CPU / (_speed * 2) + 1));
  uint32_t end_cnt = std::max<uint32_t>(2u, F_CPU / 256u / _speed);
  if (start_cnt <= 2)
  {
    end_cnt = 1;
  }
  for (uint32_t cnt = start_cnt; diff && cnt >= end_cnt; --cnt)
  {
    float fdiv = (float)F_CPU / cnt / _speed;
    uint32_t n = std::max<uint32_t>(2u, (uint32_t)fdiv);
    fdiv -= n;

    for (uint32_t a = 63; diff && a > 0; --a)
    {
      uint32_t b = roundf(fdiv * a);
      if (a == b && n == 256)
      {
        break;
      }
      uint32_t freq = F_CPU / ((n * cnt) + (float)(b * cnt) / (float)a);
      uint32_t d = abs(_speed - (int)freq);
      if (diff <= d)
      {
        continue;
      }
      diff = d;
      clkcnt = cnt;
      div_n = n;
      div_b = b;
      div_a = a;
      if (b == 0 || a == b)
      {
        break;
      }
    }
  }
  if (div_a == div_b)
  {
    div_b = 0;
    div_n += 1;
  }

  lcd_cam_lcd_clock_reg_t lcd_clock;
  lcd_clock.lcd_clkcnt_n = std::max(1u, clkcnt - 1);
  lcd_clock.lcd_clk_equ_sysclk = (clkcnt == 1);
  lcd_clock.lcd_ck_idle_edge = true;
  lcd_clock.lcd_ck_out_edge = false;
  lcd_clock.lcd_clkm_div_num = div_n;
  lcd_clock.lcd_clkm_div_b = div_b;
  lcd_clock.lcd_clkm_div_a = div_a;
  lcd_clock.lcd_clk_sel = 2; // clock_select: 1=XTAL CLOCK / 2=240MHz / 3=160MHz
  lcd_clock.clk_en = true;

  LCD_CAM.lcd_clock.val = lcd_clock.val;

  _dma_chan = _i80_bus->dma_chan;
  _dmadesc = (dma_descriptor_t *)heap_caps_malloc(sizeof(dma_descriptor_t), MALLOC_CAP_DMA);

  _buffer = (uint8_t *)heap_caps_aligned_alloc(16, LCD_MAX_PIXELS_AT_ONCE * 2, MALLOC_CAP_DMA);
  if (!_buffer)
  {
    return false;
  }

  return true;
}

/**
 * @brief beginWrite
 *
 */
void Arduino_ESP32LCD8::beginWrite()
{
  CS_LOW();

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
  LCD_CAM.lcd_user.val = 0;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG;
}

/**
 * @brief endWrite
 *
 */
void Arduino_ESP32LCD8::endWrite()
{
  WAIT_LCD_NOT_BUSY;

  CS_HIGH();
}

/**
 * @brief writeCommand
 *
 * @param c
 */
void Arduino_ESP32LCD8::writeCommand(uint8_t c)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  LCD_CAM.lcd_cmd_val.lcd_cmd_value = c;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
}

/**
 * @brief writeCommand16
 *
 * @param c
 */
void Arduino_ESP32LCD8::writeCommand16(uint16_t c)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  _data16.value = c;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
}

void Arduino_ESP32LCD8::writeCommandBytes(uint8_t *data, uint32_t len)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  while (len--)
  {
    _data16.value = *data++;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  }

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
}

/**
 * @brief write
 *
 * @param d
 */
void Arduino_ESP32LCD8::write(uint8_t d)
{
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = d;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}

/**
 * @brief write16
 *
 * @param d
 */
void Arduino_ESP32LCD8::write16(uint16_t d)
{
  _data16.value = d;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}

/**
 * @brief writeC8D8
 *
 * @param c
 * @param d
 */
void Arduino_ESP32LCD8::writeC8D8(uint8_t c, uint8_t d)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  LCD_CAM.lcd_cmd_val.lcd_cmd_value = c;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

  LCD_CAM.lcd_cmd_val.lcd_cmd_value = d;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}

/**
 * @brief writeC8D16
 *
 * @param c
 * @param d
 */
void Arduino_ESP32LCD8::writeC8D16(uint8_t c, uint16_t d)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  LCD_CAM.lcd_cmd_val.lcd_cmd_value = c;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

  LCD_CAM.lcd_cmd_val.lcd_cmd_value = d;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}

/**
 * @brief writeC8D16D16
 *
 * @param c
 * @param d1
 * @param d2
 */
void Arduino_ESP32LCD8::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  LCD_CAM.lcd_cmd_val.lcd_cmd_value = c;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

  _data16.value = d1;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  _data16.value = d2;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}

/**
 * @brief writeC8D16D16Split
 *
 * @param c
 * @param d1
 * @param d2
 */
void Arduino_ESP32LCD8::writeC8D16D16Split(uint8_t c, uint16_t d1, uint16_t d2)
{
  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

  _data16.value = c;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  LCD_CAM.lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

  _data16.value = d1;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

  _data16.value = d2;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
  WAIT_LCD_NOT_BUSY;
  LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
}

/**
 * @brief writeRepeat
 *
 * @param p
 * @param len
 */
void Arduino_ESP32LCD8::writeRepeat(uint16_t p, uint32_t len)
{
  if (len > USE_DMA_THRESHOLD)
  {
    uint32_t bufLen = (len < LCD_MAX_PIXELS_AT_ONCE) ? len : LCD_MAX_PIXELS_AT_ONCE;
    uint32_t xferLen, l;
    uint32_t c32;
    MSB_32_16_16_SET(c32, p, p);

    l = (bufLen + 1) / 2;
    for (uint32_t i = 0; i < l; i++)
    {
      _buffer32[i] = c32;
    }

    _data16.value = p;
    _data32.value = 0;
    _data32.lsb = _data16.msb;
    _data32.lsb_2 = _data16.lsb;

    while (len > USE_DMA_THRESHOLD)
    {
      xferLen = (bufLen <= len) ? bufLen : len; // How many this pass?

      LCD_CAM.lcd_cmd_val.val = _data32.value;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      l = xferLen - 2;
      l <<= 1;
      *(uint32_t *)_dmadesc = ((l + 3) & (~3)) | l << 12 | 0xC0000000;
      _dmadesc->buffer = _buffer;
      _dmadesc->next = nullptr;
      gdma_start(_dma_chan, (intptr_t)(_dmadesc));
      LCD_CAM.lcd_cmd_val.val = _data32.value;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;

      len -= xferLen;

      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
    }
  }

  while (len--)
  {
    _data16.value = p;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  }
}

/**
 * @brief writePixels
 *
 * @param data
 * @param len
 */
void Arduino_ESP32LCD8::writePixels(uint16_t *data, uint32_t len)
{
  uint32_t xferLen, l;

  while (len > (USE_DMA_THRESHOLD)) // While pixels remain
  {
    xferLen = (len >= LCD_MAX_PIXELS_AT_ONCE) ? (LCD_MAX_PIXELS_AT_ONCE) : len; // How many this pass?

    _data16.value = *data++;
    _data32.value = 0;
    _data32.lsb = _data16.msb;
    _data32.lsb_2 = _data16.lsb;

    LCD_CAM.lcd_cmd_val.val = _data32.value;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

    _data16.value = *data++;
    _data32.value = 0;
    _data32.lsb = _data16.msb;
    _data32.lsb_2 = _data16.lsb;

    l = xferLen - 2;
    l <<= 1;
    for (uint32_t i = 0; i < l;)
    {
      _data16.value = *data++;
      _buffer[i++] = _data16.msb;
      _buffer[i++] = _data16.lsb;
    }

    *(uint32_t *)_dmadesc = ((l + 3) & (~3)) | l << 12 | 0xC0000000;
    _dmadesc->buffer = _buffer32;
    _dmadesc->next = nullptr;
    gdma_start(_dma_chan, (intptr_t)(_dmadesc));
    LCD_CAM.lcd_cmd_val.val = _data32.value;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;

    len -= xferLen;

    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
  }

  while (len--)
  {
    _data16.value = *data++;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  }
}

/**
 * @brief writeBytes
 *
 * @param data
 * @param len
 */
void Arduino_ESP32LCD8::writeBytes(uint8_t *data, uint32_t len)
{
  uint32_t xferLen, l;

  if (esp_ptr_dma_capable(data))
  {
    while (len > (USE_DMA_THRESHOLD << 1)) // While pixels remain
    {
      xferLen = (len >= LCD_MAX_PIXELS_AT_ONCE << 1) ? (LCD_MAX_PIXELS_AT_ONCE << 1) : len; // How many this pass?

      _data32.value = 0;
      _data32.lsb = *data++;
      _data32.lsb_2 = *data++;

      LCD_CAM.lcd_cmd_val.val = _data32.value;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      _data32.value = 0;
      _data32.lsb = *data++;
      _data32.lsb_2 = *data++;

      l = xferLen - 4;
      *(uint32_t *)_dmadesc = ((l + 3) & (~3)) | l << 12 | 0xC0000000;
      _dmadesc->buffer = data;
      _dmadesc->next = nullptr;
      gdma_start(_dma_chan, (intptr_t)(_dmadesc));
      LCD_CAM.lcd_cmd_val.val = _data32.value;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;

      data += xferLen - 4;
      len -= xferLen;

      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
    }
  }
  else
  {
    uint32_t l4;
    uint32_t *p;
    while (len > (USE_DMA_THRESHOLD << 1)) // While pixels remain
    {
      xferLen = (len >= LCD_MAX_PIXELS_AT_ONCE << 1) ? (LCD_MAX_PIXELS_AT_ONCE << 1) : len; // How many this pass?

      _data32.value = 0;
      _data32.lsb = *data++;
      _data32.lsb_2 = *data++;

      LCD_CAM.lcd_cmd_val.val = _data32.value;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      _data32.value = 0;
      _data32.lsb = *data++;
      _data32.lsb_2 = *data++;

      l = xferLen - 4;

      l4 = (l + 3) >> 2;
      p = (uint32_t *)data;
      for (uint32_t i = 0; i < l4; i++)
      {
        _buffer32[i] = *p++;
      }

      *(uint32_t *)_dmadesc = ((l + 3) & (~3)) | l << 12 | 0xC0000000;
      _dmadesc->buffer = _buffer32;
      _dmadesc->next = nullptr;
      gdma_start(_dma_chan, (intptr_t)(_dmadesc));
      LCD_CAM.lcd_cmd_val.val = _data32.value;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;

      data += xferLen - 4;
      len -= xferLen;

      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
    }
  }

  while (len)
  {
    if (len == 1)
    {
      LCD_CAM.lcd_cmd_val.lcd_cmd_value = *data;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      len--;
    }
    else
    {
      LCD_CAM.lcd_cmd_val.lcd_cmd_value = *data++;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
      LCD_CAM.lcd_cmd_val.lcd_cmd_value = *data++;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      len -= 2;
    }
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
  if (len > USE_DMA_THRESHOLD)
  {
    uint32_t bufLen = (len < LCD_MAX_PIXELS_AT_ONCE) ? len : LCD_MAX_PIXELS_AT_ONCE;
    uint32_t xferLen, l;

    while (len > USE_DMA_THRESHOLD)
    {
      xferLen = (bufLen <= len) ? bufLen : len; // How many this pass?

      _data16.value = idx[*data++];
      _data32.value = 0;
      _data32.lsb = _data16.msb;
      _data32.lsb_2 = _data16.lsb;

      LCD_CAM.lcd_cmd_val.val = _data32.value;
      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      _data16.value = idx[*data++];
      _data32.value = 0;
      _data32.lsb = _data16.msb;
      _data32.lsb_2 = _data16.lsb;

      l = xferLen - 2;
      l >>= 1;
      for (uint32_t i = 0; i < l; i++)
      {
        _data16.value = idx[*data++];
        _data32.lsb = _data16.msb;
        _data32.msb = _data16.lsb;
        _data16.value = idx[*data++];
        _data32.lsb_2 = _data16.msb;
        _data32.msb_2 = _data16.lsb;
        _buffer32[i] = _data32.value;
      }

      l <<= 2;
      *(uint32_t *)_dmadesc = ((l + 3) & (~3)) | l << 12 | 0xC0000000;
      _dmadesc->buffer = _buffer;
      _dmadesc->next = nullptr;
      gdma_start(_dma_chan, (intptr_t)(_dmadesc));
      LCD_CAM.lcd_cmd_val.val = _data32.value;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;

      len -= xferLen;

      WAIT_LCD_NOT_BUSY;
      LCD_CAM.lcd_user.val = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
    }
  }

  while (len--)
  {
    _data16.value = idx[*data++];
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  }
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
  while (len--)
  {
    _data16.value = idx[*data++];
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.msb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
    LCD_CAM.lcd_cmd_val.lcd_cmd_value = _data16.lsb;
    WAIT_LCD_NOT_BUSY;
    LCD_CAM.lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
  }
}

/******** low level bit twiddling **********/

/**
 * @brief CS_HIGH
 *
 * @return GFX_INLINE
 */
GFX_INLINE void Arduino_ESP32LCD8::CS_HIGH(void)
{
  if (_cs != GFX_NOT_DEFINED)
  {
    *_csPortSet = _csPinMask;
  }
}

/**
 * @brief CS_LOW
 *
 * @return GFX_INLINE
 */
GFX_INLINE void Arduino_ESP32LCD8::CS_LOW(void)
{
  if (_cs != GFX_NOT_DEFINED)
  {
    *_csPortClr = _csPinMask;
  }
}

#else // (ESP_ARDUINO_VERSION_MAJOR >= 3)

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
    _speed = 20000000UL; // safe frequency
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

  // Serial.printf("max_transfer_bytes=%zu\n", bus_config.max_transfer_bytes);

  esp_lcd_panel_io_i80_config_t io_config = {
      .cs_gpio_num = _cs,
      .pclk_hz = _speed,
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
          .swap_color_bytes = 0,
          .pclk_idle_low = 0,
      },
  };
  esp_lcd_new_panel_io_i80(_i80_bus, &io_config, &_io_handle);

  // allocate some DMA buffer
  _cmd = -1;
  _isColor = false;
  _bufferLen = 0;
  _buffer = (uint8_t *)esp_lcd_i80_alloc_draw_buffer(_io_handle, LCD_MAX_PIXELS_AT_ONCE * 2 + 16, MALLOC_CAP_DMA);

  return (_buffer != nullptr);
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
  // Serial.printf("writeCommand(%02x)\n", c);
  _cmd = c;
  _isColor = false;
}

/**
 * @brief write a word command
 * @param c the command word
 */
void Arduino_ESP32LCD8::writeCommand16(uint16_t c)
{
  // Serial.printf("ESP32LCD8::writeCommand16 not implemented.\n");
}

/** write a set of data as command + params */
void Arduino_ESP32LCD8::writeCommandBytes(uint8_t *data, uint32_t len)
{
  flushBuffer();
  // Serial.printf("**writeCommandBytes()\n");

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
  // Serial.printf("  writeRepeat(#%04x, %d)\n", p, len);
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
  // Serial.printf("  writePixels( [...], %ld)\n", len);
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
  // Serial.printf("  writeBytes( [...], %ld)\n", len);
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
  // Serial.printf("**writeIndexedPixels(...)\n");
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
  // Serial.printf("**writeIndexedPixelsDouble(...)\n");
}

// ===== sending the buffer =====

// for details, see
// <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/lcd/index.html>

void Arduino_ESP32LCD8::flushBuffer()
{

  if (_cmd == 0x2c)
  {
    if (!_isColor)
      _isColor = true;
  }

  if ((_cmd >= 0) || (_bufferLen > 0))
  {
    // Serial.printf("  flush(%02x: (%d)", _cmd, _bufferLen);

    // for (int n = 0; (n < _bufferLen) && (n < 32); n++)
    // {
    //   Serial.printf(" %02x", _buffer[n]);
    // }

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
    // Serial.println(")");
  }
}

#endif // #if (!defined(ESP_ARDUINO_VERSION_MAJOR)) || (ESP_ARDUINO_VERSION_MAJOR < 3)
#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
