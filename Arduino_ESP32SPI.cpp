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
  _speed = speed ? speed : 80;
  if (_dc >= 0)
  {
    pinMode(_dc, OUTPUT);
    digitalWrite(_dc, HIGH); // Data mode
  }
  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // Deselect
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
  if (_cs >= 32)
  {
    csPinMask = digitalPinToBitMask(_cs);
    csPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    csPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else if (_cs >= 0)
  {
    csPinMask = digitalPinToBitMask(_cs);
    csPortSet = (PORTreg_t)&GPIO.out_w1ts;
    csPortClr = (PORTreg_t)&GPIO.out_w1tc;
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
  _spi->dev->user.usr_miso = 1;
  _spi->dev->user.doutdin = 1;

  int i;
  for (i = 0; i < 16; i++)
  {
    _spi->dev->data_buf[i] = 0x00000000;
  }
  SPI_MUTEX_UNLOCK();

  addApbChangeCallback(_spi, _on_apb_change);

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
  CS_LOW();
}

void Arduino_ESP32SPI::writeCommand(uint8_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
  }
  else
  {
    DC_LOW();
    write(c);
    DC_HIGH();
  }
}

void Arduino_ESP32SPI::writeCommand16(uint16_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
  }
  else
  {
    DC_LOW();
    write16(c);
    DC_HIGH();
  }
}

void Arduino_ESP32SPI::write(uint8_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
  }
  else
  {
    _spi->dev->mosi_dlen.usr_mosi_dbitlen = 7;
    _spi->dev->miso_dlen.usr_miso_dbitlen = 0;
    _spi->dev->data_buf[0] = d;
    _spi->dev->cmd.usr = 1;
    while (_spi->dev->cmd.usr)
      ;
  }
}

void Arduino_ESP32SPI::write16(uint16_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
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
  CS_HIGH();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendCommand(uint8_t c)
{
  SPI_MUTEX_LOCK();
  CS_LOW();

  writeCommand(c);

  CS_HIGH();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendCommand16(uint16_t c)
{
  SPI_MUTEX_LOCK();
  CS_LOW();

  writeCommand16(c);

  CS_HIGH();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendData(uint8_t d)
{
  SPI_MUTEX_LOCK();
  CS_LOW();

  write(d);

  CS_HIGH();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendData16(uint16_t d)
{
  SPI_MUTEX_LOCK();
  CS_LOW();

  write16(d);

  CS_HIGH();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::sendData32(uint32_t d)
{
  SPI_MUTEX_LOCK();
  CS_LOW();

  write32(d);

  CS_HIGH();
  SPI_MUTEX_UNLOCK();
}

void Arduino_ESP32SPI::setDataMode(uint8_t dataMode)
{
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
}

void Arduino_ESP32SPI::writeRepeat(uint16_t p, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
  }
  else
  {
  }
}

void Arduino_ESP32SPI::writeBytes(uint8_t *data, uint32_t len)
{
  while (len--)
  {
    write(*data++);
  }
}

void Arduino_ESP32SPI::writePixels(uint16_t *data, uint32_t len)
{
  while (len--)
  {
    write16(*data++);
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
    for (uint8_t i = 0; i < size; i++)
    {
      write(data[i]);
    }
  }
}

/******** low level bit twiddling **********/

inline void Arduino_ESP32SPI::CS_HIGH(void)
{
  if (_cs >= 0)
  {
    *csPortSet = csPinMask;
  }
}

inline void Arduino_ESP32SPI::CS_LOW(void)
{
  if (_cs >= 0)
  {
    *csPortClr = csPinMask;
  }
}

inline void Arduino_ESP32SPI::DC_HIGH(void)
{
  *dcPortSet = dcPinMask;
}

inline void Arduino_ESP32SPI::DC_LOW(void)
{
  *dcPortClr = dcPinMask;
}
