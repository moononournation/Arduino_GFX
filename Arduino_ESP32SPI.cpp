/*
 * start rewrite from:
 * https://github.com/espressif/arduino-esp32.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_ESP32SPI.h"

Arduino_ESP32SPI::Arduino_ESP32SPI(int8_t dc, int8_t cs, int8_t sck, int8_t mosi, int8_t miso /* = -1 */)
{
  _dc = dc;
  _cs = cs;
  _sck = sck;
  _mosi = mosi;
  _miso = miso;
}

static void _on_apb_change(void *arg, apb_change_ev_t ev_type, uint32_t old_apb, uint32_t new_apb)
{
  spi_t *_spi = (spi_t *)arg;
  if (ev_type == APB_BEFORE_CHANGE)
  {
    SPI_MUTEX_LOCK();
    while (_spi->dev->cmd.usr)
      ;
  }
  else
  {
    _spi->dev->clock.val = spiFrequencyToClockDiv(old_apb / ((_spi->dev->clock.clkdiv_pre + 1) * (_spi->dev->clock.clkcnt_n + 1)));
    SPI_MUTEX_UNLOCK();
  }
}

void Arduino_ESP32SPI::begin(uint32_t speed)
{
  _speed = speed ? speed : 80000000;
  if (_dc >= 0)
  {
    pinMode(_dc, OUTPUT);
    digitalWrite(_dc, HIGH); // Data mode
  }

  if (_dc >= 32)
  {
    dcPinMask = digitalPinToBitMask(_dc);
    dcPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    dcPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else if (_dc >= 0)
  {
    dcPinMask = digitalPinToBitMask(_dc);
    dcPortSet = (PORTreg_t)&GPIO.out_w1ts;
    dcPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }

  _spi = &_spi_bus_array[VSPI];

#if !CONFIG_DISABLE_HAL_LOCKS
  if (_spi->lock == NULL)
  {
    _spi->lock = xSemaphoreCreateMutex();
  }
#endif

  // VSPI
  DPORT_SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_SPI_CLK_EN_2);
  DPORT_CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_SPI_RST_2);

  // spiStopBus
  SPI_MUTEX_LOCK();
  _spi->dev->slave.trans_done = 0;
  _spi->dev->slave.slave_mode = 0;
  _spi->dev->pin.val = 0;
  _spi->dev->user.val = 0;
  _spi->dev->user1.val = 0;
  _spi->dev->ctrl.val = 0;
  _spi->dev->ctrl1.val = 0;
  _spi->dev->ctrl2.val = 0;
  _spi->dev->clock.val = 0;
  SPI_MUTEX_UNLOCK();
  removeApbChangeCallback(_spi, _on_apb_change);

  setDataMode(SPI_MODE0);

  // spiSetBitOrder SPI_MSBFIRST
  _spi->dev->ctrl.wr_bit_order = 0;
  _spi->dev->ctrl.rd_bit_order = 0;

  // spiSetClockDiv
  SPI_MUTEX_LOCK();
  _spi->dev->clock.val = spiFrequencyToClockDiv(_speed);
  SPI_MUTEX_UNLOCK();

  SPI_MUTEX_LOCK();
  _spi->dev->user.usr_mosi = 1;
  if (_miso < 0)
  {
    _spi->dev->user.usr_miso = 0;
    _spi->dev->user.doutdin = 0;
  }
  else
  {
    _spi->dev->user.usr_miso = 1;
    _spi->dev->user.doutdin = 1;
  }

  int i;
  for (i = 0; i < 16; i++)
  {
    _spi->dev->data_buf[i] = 0x00000000;
  }
  SPI_MUTEX_UNLOCK();

  addApbChangeCallback(_spi, _on_apb_change);

  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    pinMatrixOutAttach(_cs, SPI_SS_IDX(VSPI, 0), false, false);
    spiEnableSSPins(_spi, (1 << 0));
  }
  pinMode(_sck, OUTPUT);
  pinMatrixOutAttach(_sck, SPI_CLK_IDX(VSPI), false, false);
  if (_miso >= 0)
  {
    pinMode(_miso, INPUT);
    pinMatrixInAttach(_miso, SPI_MISO_IDX(VSPI), false);
  }
  pinMode(_mosi, OUTPUT);
  pinMatrixOutAttach(_mosi, SPI_MOSI_IDX(VSPI), false, false);
}

void Arduino_ESP32SPI::beginWrite()
{
  SPI_MUTEX_LOCK();
  if (_dc >= 0)
  {
    DC_HIGH();
  }
  data_buf_bit_idx = 0;
  data_buf[0] = 0;
}

void Arduino_ESP32SPI::write9bitCommand(uint32_t c)
{
  int idx = data_buf_bit_idx >> 3;
  int shift = (data_buf_bit_idx % 8);
  if (shift)
  {
    data_buf[idx++] |= c >> (shift + 1);
    data_buf[idx] = c << (7 - shift);
  }
  else
  {
    data_buf[idx++] = c >> 1;
    data_buf[idx] = c << 7;
  }
  data_buf_bit_idx += 9;
  if (data_buf_bit_idx >= 504) // 56 bytes * 9 bits
  {
    flush_data_buf();
  }
}

void Arduino_ESP32SPI::writeCommand(uint8_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
    write9bitCommand(c);
  }
  else
  {
    flush_data_buf();

    DC_LOW();
    _spi->dev->mosi_dlen.usr_mosi_dbitlen = 7;
    _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
    _spi->dev->data_buf[0] = c;
    _spi->dev->cmd.usr = 1;
    while (_spi->dev->cmd.usr)
      ;
    DC_HIGH();
  }
}

void Arduino_ESP32SPI::writeCommand16(uint16_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
    write9bitCommand(c >> 8);
    write9bitCommand(c & 0xff);
  }
  else
  {
    flush_data_buf();

    DC_LOW();
    _spi->dev->mosi_dlen.usr_mosi_dbitlen = 15;
    _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
    _spi->dev->data_buf[0] = c;
    _spi->dev->cmd.usr = 1;
    while (_spi->dev->cmd.usr)
      ;
    DC_HIGH();
  }
}

inline void Arduino_ESP32SPI::flush_data_buf()
{
  if (data_buf_bit_idx > 0)
  {
    _spi->dev->mosi_dlen.usr_mosi_dbitlen = data_buf_bit_idx - 1;
    _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
    int len = (data_buf_bit_idx + 31) / 32;
    for (int i = 0; i < len; i++)
    {
      _spi->dev->data_buf[i] = data_buf32[i];
    }
    _spi->dev->cmd.usr = 1;
    while (_spi->dev->cmd.usr)
      ;
    data_buf_bit_idx = 0;
  }
}

void Arduino_ESP32SPI::write(uint8_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    uint32_t b = 0x100 | d;
    int idx = data_buf_bit_idx >> 3;
    int shift = (data_buf_bit_idx % 8);
    if (shift)
    {
      data_buf[idx++] |= b >> (shift + 1);
      data_buf[idx] = b << (7 - shift);
    }
    else
    {
      data_buf[idx++] = b >> 1;
      data_buf[idx] = b << 7;
    }
    data_buf_bit_idx += 9;
    if (data_buf_bit_idx >= 504) // 56 bytes * 9 bits
    {
      flush_data_buf();
    }
  }
  else
  {
    int idx = data_buf_bit_idx >> 3;
    data_buf[idx] = d;
    data_buf_bit_idx += 8;
    if (data_buf_bit_idx >= 512)
    {
      flush_data_buf();
    }
  }
}

void Arduino_ESP32SPI::write16(uint16_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    write(d >> 8);
    write(d);
  }
  else
  {
    write(d >> 8);
    write(d);
  }
}

void Arduino_ESP32SPI::write32(uint32_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    write(d >> 24);
    write(d >> 16);
    write(d >> 8);
    write(d);
  }
  else
  {
    write(d >> 24);
    write(d >> 16);
    write(d >> 8);
    write(d);
  }
}

void Arduino_ESP32SPI::endWrite()
{
  flush_data_buf();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendCommand(uint8_t c)
{
  SPI_MUTEX_LOCK();
  writeCommand(c);
  flush_data_buf();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendCommand16(uint16_t c)
{
  SPI_MUTEX_LOCK();
  writeCommand16(c);
  flush_data_buf();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendData(uint8_t d)
{
  SPI_MUTEX_LOCK();
  write(d);
  flush_data_buf();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendData16(uint16_t d)
{
  SPI_MUTEX_LOCK();
  write16(d);
  flush_data_buf();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendData32(uint32_t d)
{
  SPI_MUTEX_LOCK();
  write32(d);
  flush_data_buf();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::setDataMode(uint8_t dataMode)
{
  SPI_MUTEX_LOCK();
  switch (dataMode)
  {
  case SPI_MODE1:
    _spi->dev->pin.ck_idle_edge = 0;
    _spi->dev->user.ck_out_edge = 1;
    break;
  case SPI_MODE2:
    _spi->dev->pin.ck_idle_edge = 1;
    _spi->dev->user.ck_out_edge = 1;
    break;
  case SPI_MODE3:
    _spi->dev->pin.ck_idle_edge = 1;
    _spi->dev->user.ck_out_edge = 0;
    break;
  case SPI_MODE0:
  default:
    _spi->dev->pin.ck_idle_edge = 0;
    _spi->dev->user.ck_out_edge = 0;
    break;
  }
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::writeRepeat(uint16_t p, uint32_t len)
{
  flush_data_buf();

  if (_dc < 0) // 9-bit SPI
  {
    uint32_t hi = 0x100 | (p >> 8);
    uint32_t lo = 0x100 | (p & 0xff);
    int idx, shift, l;
    uint16_t bufLen = (len <= 28) ? len : 28;
    uint16_t xferLen;
    for (uint32_t t = 0; t < bufLen; t++)
    {
      idx = data_buf_bit_idx >> 3;
      shift = (data_buf_bit_idx % 8);
      if (shift)
      {
        data_buf[idx++] |= hi >> (shift + 1);
        data_buf[idx] = hi << (7 - shift);
      }
      else
      {
        data_buf[idx++] = hi >> 1;
        data_buf[idx] = hi << 7;
      }
      data_buf_bit_idx += 9;

      idx = data_buf_bit_idx >> 3;
      shift = (data_buf_bit_idx % 8);
      if (shift)
      {
        data_buf[idx++] |= lo >> (shift + 1);
        data_buf[idx] = lo << (7 - shift);
      }
      else
      {
        data_buf[idx++] = lo >> 1;
        data_buf[idx] = lo << 7;
      }
      data_buf_bit_idx += 9;
    }

    if (_miso < 0)
    {
      l = (data_buf_bit_idx + 31) / 32;
      for (int i = 0; i < l; i++)
      {
        _spi->dev->data_buf[i] = data_buf32[i];
      }
    }

    // Issue pixels in blocks from temp buffer
    while (len)
    {                                          // While pixels remain
      xferLen = (bufLen < len) ? bufLen : len; // How many this pass?
      data_buf_bit_idx = xferLen * 18;
      _spi->dev->mosi_dlen.usr_mosi_dbitlen = data_buf_bit_idx - 1;
      _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
      if (_miso >= 0)
      {
        l = (data_buf_bit_idx + 31) / 32;
        for (int i = 0; i < l; i++)
        {
          _spi->dev->data_buf[i] = data_buf32[i];
        }
      }
      _spi->dev->cmd.usr = 1;
      while (_spi->dev->cmd.usr)
        ;
      len -= xferLen;
    }
  }
  else // 8-bit SPI
  {
    uint16_t bufLen = (len < 32) ? len : 32;
    uint16_t xferLen, l;
    MSB_16_SET(p, p);
    uint32_t c32 = p * 0x00010001;

    if (_miso < 0)
    {
      l = (bufLen + 1) / 2;
      for (int i = 0; i < l; i++)
      {
        _spi->dev->data_buf[i] = c32;
      }
    }

    // Issue pixels in blocks from temp buffer
    while (len)
    {                                           // While pixels remain
      xferLen = (bufLen <= len) ? bufLen : len; // How many this pass?
      _spi->dev->mosi_dlen.usr_mosi_dbitlen = (xferLen * 16) - 1;
      _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
      if (_miso >= 0)
      {
        l = (xferLen + 1) / 2;
        for (int i = 0; i < l; i++)
        {
          _spi->dev->data_buf[i] = c32;
        }
      }
      _spi->dev->cmd.usr = 1;
      while (_spi->dev->cmd.usr)
        ;
      len -= xferLen;
    }
  }

  data_buf_bit_idx = 0;
}

void Arduino_ESP32SPI::writeBytes(uint8_t *data, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
    while (len--)
    {
      write(*data++);
    }
  }
  else // 8-bit SPI
  {
    if (len < 64)
    {
      flush_data_buf();

      _spi->dev->mosi_dlen.usr_mosi_dbitlen = (len * 8) - 1;
      _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
      uint8_t *buf = (uint8_t *)&_spi->dev->data_buf;
      while (len)
      {
        *buf++ = *data++;
      }
      _spi->dev->cmd.usr = 1;
      while (_spi->dev->cmd.usr)
        ;
      data_buf_bit_idx = 0;
    }
    else
    {
      while (len--)
      {
        write(*data++);
      }
    }
  }
}

void Arduino_ESP32SPI::writePixels(uint16_t *data, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
    while (len--)
    {
      write16(*data++);
    }
  }
  else // 8-bit SPI
  {
    if (((len % 2) == 0) && (len < 32))
    {
      flush_data_buf();

      _spi->dev->mosi_dlen.usr_mosi_dbitlen = (len * 16) - 1;
      _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
      uint32_t v1, v2;
      len >>= 1; // 2 pixels to a 32-bit data
      for (int i = 0; i < len; i++)
      {
        v1 = *data++;
        v2 = *data++;
        _spi->dev->data_buf[i] = ((v1 & 0xff00) >> 8) | ((v1 & 0xff) << 8) | ((v2 & 0xff00) << 8) | ((v1 & 0xff) << 24);
      }
      _spi->dev->cmd.usr = 1;
      while (_spi->dev->cmd.usr)
        ;
      data_buf_bit_idx = 0;
    }
    else
    {
      while (len--)
      {
        write16(*data++);
      }
    }
  }
}

/**
 * @param data uint8_t *
 * @param size uint8_t  max for size is 64Byte
 * @param repeat uint32_t
 */
void Arduino_ESP32SPI::writePattern(uint8_t *data, uint8_t size, uint32_t repeat)
{
  while (repeat--)
  {
    writeBytes(data, size);
  }
}

/******** low level bit twiddling **********/

inline void Arduino_ESP32SPI::DC_HIGH(void)
{
  *dcPortSet = dcPinMask;
}

inline void Arduino_ESP32SPI::DC_LOW(void)
{
  *dcPortClr = dcPinMask;
}
