/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_HWSPI_NoDC.h"

#if defined(SPI_HAS_TRANSACTION)
#define SPI_BEGIN_TRANSACTION() _spi->beginTransaction(mySPISettings)
#define SPI_END_TRANSACTION() _spi->endTransaction()
#else
#define SPI_BEGIN_TRANSACTION() \
  {                             \
  }
#define SPI_END_TRANSACTION() \
  {                           \
  }
#endif

#if defined(SPI_HAS_TRANSACTION)
static SPISettings mySPISettings;
#elif defined(__AVR__) || defined(CORE_TEENSY)
static uint8_t SPCRbackup;
static uint8_t mySPCR;
#endif

#if defined(ESP32)
Arduino_HWSPI_NoDC::Arduino_HWSPI_NoDC(int8_t cs /* = GFX_NOT_DEFINED */, int8_t sck /* = GFX_NOT_DEFINED */, int8_t mosi /* = GFX_NOT_DEFINED */, int8_t miso /* = GFX_NOT_DEFINED */, SPIClass *spi, bool is_shared_interface /* = true */, uint16_t commandValue, uint16_t dataValue)
    : _cs(cs), _sck(sck), _mosi(mosi), _miso(miso), _spi(spi), _is_shared_interface(is_shared_interface), _commandValue(commandValue), _dataValue(dataValue)
{
#else
Arduino_HWSPI_NoDC::Arduino_HWSPI_NoDC(int8_t cs /* = GFX_NOT_DEFINED */, SPIClass *spi, bool is_shared_interface /* = true */, uint16_t commandValue, uint16_t dataValue, uint16_t readValue, uint16_t writeValue)
    : _cs(cs), _spi(spi), _is_shared_interface(is_shared_interface), _commandValue(commandValue), _dataValue(dataValue), _readValue(readValue), _writeValue(writeValue)
{
#endif
}

bool Arduino_HWSPI_NoDC::begin(int32_t speed, int8_t dataMode)
{
  _speed = (speed == GFX_NOT_DEFINED) ? SPI_DEFAULT_FREQ : speed;
  _dataMode = dataMode;

  if (_cs != GFX_NOT_DEFINED)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // Deselect
  }

#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(ARDUINO_ARCH_NRF52840)
  if (_cs != GFX_NOT_DEFINED)
  {
    uint32_t pin = digitalPinToPinName((pin_size_t)_cs);
    NRF_GPIO_Type *reg = nrf_gpio_pin_port_decode(&pin);
    _csPortSet = &reg->OUTSET;
    _csPortClr = &reg->OUTCLR;
    _csPinMask = 1UL << pin;
  }
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  if (_cs != GFX_NOT_DEFINED)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t) & (((R_PORT0_Type *)IOPORT_PRV_PORT_ADDRESS(digitalPinToPort(_cs)))->POSR);
    _csPortClr = (PORTreg_t) & (((R_PORT0_Type *)IOPORT_PRV_PORT_ADDRESS(digitalPinToPort(_cs)))->PORR);
  }
#elif defined(TARGET_RP2040)
  if (_cs != GFX_NOT_DEFINED)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t)&sio_hw->gpio_set;
    _csPortClr = (PORTreg_t)&sio_hw->gpio_clr;
  }
#elif defined(ESP32) && (CONFIG_IDF_TARGET_ESP32C3)
  if (_cs != GFX_NOT_DEFINED)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t)GPIO_OUT_W1TS_REG;
    _csPortClr = (PORTreg_t)GPIO_OUT_W1TC_REG;
  }
#elif defined(ESP32)
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
#elif defined(CORE_TEENSY)
  if (_cs != GFX_NOT_DEFINED)
  {
#if !defined(KINETISK)
    _csPinMask = digitalPinToBitMask(_cs);
#endif
    _csPortSet = portSetRegister(_cs);
    _csPortClr = portClearRegister(_cs);
  }
#else  // !CORE_TEENSY
  if (_cs != GFX_NOT_DEFINED)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = &(PORT->Group[g_APinDescription[_cs].ulPort].OUTSET.reg);
    _csPortClr = &(PORT->Group[g_APinDescription[_cs].ulPort].OUTCLR.reg);
  }
#endif // end !CORE_TEENSY
#else  // !HAS_PORT_SET_CLR
  if (_cs != GFX_NOT_DEFINED)
  {
    _csPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_cs));
    _csPinMaskSet = digitalPinToBitMask(_cs);
  }
  _csPinMaskClr = ~_csPinMaskSet;
#endif // !HAS_PORT_SET_CLR
#endif // USE_FAST_PINIO

#if defined(ESP32)
  _spi->begin(_sck, _miso, _mosi);
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE0;
  }
  mySPISettings = SPISettings(_speed, MSBFIRST, _dataMode);
#elif defined(ESP8266)
  _spi->begin();
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE0;
  }
  mySPISettings = SPISettings(_speed, MSBFIRST, _dataMode);
// Teensy 4.x
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
  _spi->begin();
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE0;
  }
  mySPISettings = SPISettings(_speed, MSBFIRST, _dataMode);
#elif defined(SPI_HAS_TRANSACTION)
  _spi->begin();
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE2;
  }
  mySPISettings = SPISettings(_speed, MSBFIRST, _dataMode);
#elif defined(__AVR__) || defined(CORE_TEENSY)
  SPCRbackup = SPCR;
  _spi->begin();
  _spi->setClockDivider(SPI_CLOCK_DIV2);
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE2;
  }
  _spi->setDataMode(_dataMode);
  mySPCR = SPCR;     // save our preferred state
  SPCR = SPCRbackup; // then restore
#elif defined(__SAM3X8E__)
  _spi->begin();
  _spi->setClockDivider(21); // 4MHz
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE2;
  }
  _spi->setDataMode(_dataMode);
#elif defined(__arm__)
  if (_dataMode == GFX_NOT_DEFINED)
  {
    _dataMode = SPI_MODE2;
  }
#endif

  return true;
}

void Arduino_HWSPI_NoDC::beginWrite()
{
  if (_is_shared_interface)
  {
    SPI_BEGIN_TRANSACTION();
  }

  CS_LOW();
}

void Arduino_HWSPI_NoDC::endWrite()
{
  CS_HIGH();

  if (_is_shared_interface)
  {
    SPI_END_TRANSACTION();
  }
}

void Arduino_HWSPI_NoDC::writeCommand(uint8_t c)
{
  beginWrite();
  WRITE16(_commandValue | _writeValue | c);
  endWrite();
}

void Arduino_HWSPI_NoDC::writeCommand16(uint16_t c)
{
#if defined(LITTLE_FOOT_PRINT)
  _data16.value = c;
  WRITE16(_commandValue | _writeValue | _data16.msb);
  WRITE16(_commandValue | _writeValue | _data16.lsb);
#else  // !defined(LITTLE_FOOT_PRINT)
  beginWrite();
  WRITE16(_commandValue | _writeValue | c);
  endWrite();
#endif // !defined(LITTLE_FOOT_PRINT)
}

void Arduino_HWSPI_NoDC::writeCommandBytes(uint8_t *data, uint32_t len)
{
  beginWrite();
  while (len--)
  {
    WRITE16(_commandValue | _writeValue | (*data++));
  }
  endWrite();
}

void Arduino_HWSPI_NoDC::write(uint8_t d)
{
  beginWrite();
  WRITE16(_dataValue | _writeValue | d);
  endWrite();
}

void Arduino_HWSPI_NoDC::write16(uint16_t d)
{
#if defined(LITTLE_FOOT_PRINT)
  _data16.value = d;
  WRITE16(_dataValue | _writeValue | _data16.msb);
  WRITE16(_dataValue | _writeValue | _data16.lsb);
#else  // !defined(LITTLE_FOOT_PRINT)
  beginWrite();
  WRITE16(_dataValue | _writeValue | d);
  endWrite();
#endif // !defined(LITTLE_FOOT_PRINT)
}

void Arduino_HWSPI_NoDC::writeRepeat(uint16_t p, uint32_t len)
{
#if defined(LITTLE_FOOT_PRINT)
  _data16.value = p;
  while (len--)
  {
    WRITE16(_dataValue | _writeValue | _data16.msb);
    WRITE16(_dataValue | _writeValue | _data16.lsb);
  }
#elif defined(ESP8266) || defined(CONFIG_ARCH_CHIP_CXD56XX)
  MSB_16_SET(p, p);
  uint32_t xferLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
  for (uint32_t i = 0; i < xferLen; i++)
  {
    _buffer.v16[i] = p;
  }

  while (len)
  {
    xferLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
    len -= xferLen;

    xferLen += xferLen;
    WRITEBUF(_buffer.v8, xferLen);
  }
#else  // other arch
  MSB_16_SET(p, p);
  uint32_t xferLen;

  beginWrite();
  while (len)
  {
    xferLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
    for (uint32_t i = 0; i < xferLen; i++)
    {
      _buffer.v16[i] = p;
    }
    len -= xferLen;

    xferLen += xferLen;
    WRITEBUF(_buffer.v8, xferLen);
  }
  endWrite();
#endif // other arch
}

void Arduino_HWSPI_NoDC::writeBytes(uint8_t *data, uint32_t len)
{
#if defined(LITTLE_FOOT_PRINT)
  while (len--)
  {
    WRITE(*data++);
  }
#else  // !defined(LITTLE_FOOT_PRINT)
  beginWrite();
  WRITEBUF(data, len);
  endWrite();
#endif // !defined(LITTLE_FOOT_PRINT)
}

void Arduino_HWSPI_NoDC::writePixels(uint16_t *data, uint32_t len)
{
#if defined(LITTLE_FOOT_PRINT)
  while (len--)
  {
    _data16.value = *data++;
    WRITE(_data16.msb);
    WRITE(_data16.lsb);
  }
#else  // !defined(LITTLE_FOOT_PRINT)
  uint32_t xferLen;
  uint8_t *b;
  union
  {
    uint16_t val;
    struct
    {
      uint8_t lsb;
      uint8_t msb;
    };
  } t;

  beginWrite();
  while (len)
  {
    xferLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
    b = _buffer.v8;
    for (uint32_t i = 0; i < xferLen; i++)
    {
      t.val = *data++;
      *b++ = t.msb;
      *b++ = t.lsb;
    }
    len -= xferLen;

    xferLen += xferLen; // uint16_t to uint8_t, double length
    WRITEBUF(_buffer.v8, xferLen);
  }
  endWrite();
#endif // !defined(LITTLE_FOOT_PRINT)
}

#if !defined(LITTLE_FOOT_PRINT)
void Arduino_HWSPI_NoDC::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
#if defined(ESP8266) || defined(ESP32)
  _spi->writePattern(data, len, repeat);
#else  // !(defined(ESP8266) || defined(ESP32))
  beginWrite();
  while (repeat--)
  {
    WRITEBUF(data, len);
  }
  endWrite();
#endif // !(defined(ESP8266) || defined(ESP32))
}


#endif // !defined(LITTLE_FOOT_PRINT)

#if defined(INCLUDE_READ_OPERATIONS)

uint8_t Arduino_HWSPI_NoDC::receive(uint8_t commandByte, uint8_t index)
{
    uint8_t result;
    beginWrite();

    result=WRITE16(_commandValue | _writeValue | commandByte);
    while(index>0) {
      result = WRITE16(_dataValue | _readValue);
      index--;
    } // Discard bytes up to index'th byte
    endWrite();

    return result;
}

uint16_t Arduino_HWSPI_NoDC::receive16(uint16_t addr)
{
    beginWrite();
    uint16_t val = WRITE16(addr);
    endWrite();

    return val;
}


#endif // defined(INCLUDE_READ_OPERATIONS)


#if !defined(LITTLE_FOOT_PRINT)

INLINE uint16_t Arduino_HWSPI_NoDC::WRITE16(uint16_t d)
{
#if defined(ESP8266) || defined(ESP32)
  _spi->write16(d);
#elif defined(SPI_HAS_TRANSACTION)
  return _spi->transfer16(d);
#elif defined(__AVR__) || defined(CORE_TEENSY)
  SPCRbackup = SPCR;
  SPCR = mySPCR;
  uint16_t r = _spi->transfer16(d);
  SPCR = SPCRbackup;
  return r;
#elif defined(__arm__)
  _spi->setClockDivider(21); // 4MHz
  _spi->setDataMode(_dataMode);
  return _spi->transfer16(d);
#else
  return _spi->transfer16(d);
#endif
}

INLINE void Arduino_HWSPI_NoDC::WRITEBUF(uint8_t *buf, size_t count)
{
  for(size_t i=0;i<count;i++)
  {
    uint16_t data = _dataValue | buf[i];
#if defined(ESP8266) || defined(ESP32)
    _spi->writeBytes(data);
#elif defined(CONFIG_ARCH_CHIP_CXD56XX)
    _spi->send(data);
#else  // other arch.
    _spi->transfer16(data);
#endif // other arch.
  }
}

#endif // !defined(LITTLE_FOOT_PRINT)

/******** low level bit twiddling **********/


INLINE void Arduino_HWSPI_NoDC::CS_HIGH(void)
{
  if (_cs != GFX_NOT_DEFINED)
  {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
    *_csPortSet = 1;
#else  // !KINETISK
    *_csPortSet = _csPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
    *_csPort |= _csPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
    digitalWrite(_cs, HIGH);
#endif // end !USE_FAST_PINIO
  }
}

INLINE void Arduino_HWSPI_NoDC::CS_LOW(void)
{
  if (_cs != GFX_NOT_DEFINED)
  {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
    *_csPortClr = 1;
#else  // !KINETISK
    *_csPortClr = _csPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
    *_csPort &= _csPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
    digitalWrite(_cs, LOW);
#endif // end !USE_FAST_PINIO
  }
}
