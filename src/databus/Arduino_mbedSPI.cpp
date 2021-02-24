/*
 * start rewrite from:
 * https://github.com/arduino/ArduinoCore-mbed/blob/master/libraries/SPI/SPI.cpp
 */
#ifdef ARDUINO_ARCH_NRF52840

#include <SPI.h>

#include "Arduino_DataBus.h"
#include "Arduino_mbedSPI.h"

#define SPI_DEFAULT_FREQ 8000000

Arduino_mbedSPI::Arduino_mbedSPI(int8_t dc, int8_t cs /* = -1 */)
    : _dc(dc), _cs(cs)
{
}

void Arduino_mbedSPI::begin(int32_t speed, int8_t dataMode)
{
  _speed = speed ? speed : SPI_DEFAULT_FREQ;
  _dataMode = dataMode;

  _dcGpio = new mbed::DigitalInOut(digitalPinToPinName((pin_size_t)_dc), PIN_OUTPUT, PullNone, HIGH);
  _dcGpio->write(HIGH); // Data mode
  if (_cs >= 0)
  {
    _csGpio = new mbed::DigitalInOut(digitalPinToPinName((pin_size_t)_cs), PIN_OUTPUT, PullNone, HIGH);
    _csGpio->write(HIGH); // Deselect
  }

  if (_dataMode < 0)
  {
    _dataMode = SPI_MODE0;
  }
  _dev = new mbed::SPI((PinName)SPI_MOSI, (PinName)SPI_MISO, (PinName)SPI_SCK);
}

void Arduino_mbedSPI::beginWrite()
{
  _dev->lock();
  _dev->format(8, _dataMode);
  _dev->frequency(_speed);

  DC_HIGH();

  CS_LOW();
}

void Arduino_mbedSPI::writeCommand(uint8_t c)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();
}

void Arduino_mbedSPI::writeCommand16(uint16_t c)
{
  DC_LOW();

  WRITE16(c);

  DC_HIGH();
}

void Arduino_mbedSPI::writeCommand32(uint32_t c)
{
  DC_LOW();

  WRITE32(c);

  DC_HIGH();
}

void Arduino_mbedSPI::write(uint8_t d)
{
  WRITE(d);
}

void Arduino_mbedSPI::write16(uint16_t d)
{
  WRITE16(d);
}

void Arduino_mbedSPI::write32(uint32_t d)
{
  WRITE32(d);
}

void Arduino_mbedSPI::writeC8D8(uint8_t c, uint8_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d);
}

void Arduino_mbedSPI::writeC8D16(uint8_t c, uint16_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE16(d);
}

void Arduino_mbedSPI::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  DC_LOW();

  _dev->write((const char *)&c, 1, NULL, 0);

  DC_HIGH();

  uint32_t d;
  MSB_32_16_16_SET(d, d1, d2);
  _dev->write((const char *)&d, 4, NULL, 0);
}

void Arduino_mbedSPI::endWrite()
{
  CS_HIGH();
  _dev->unlock();
}

void Arduino_mbedSPI::sendCommand(uint8_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_mbedSPI::sendCommand16(uint16_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE16(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_mbedSPI::sendCommand32(uint32_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE32(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_mbedSPI::sendData(uint8_t d)
{
  CS_LOW();

  WRITE(d);

  CS_HIGH();
}

void Arduino_mbedSPI::sendData16(uint16_t d)
{
  CS_LOW();

  WRITE16(d);

  CS_HIGH();
}

void Arduino_mbedSPI::sendData32(uint32_t d)
{
  CS_LOW();

  WRITE32(d);

  CS_HIGH();
}

void Arduino_mbedSPI::writeRepeat(uint16_t p, uint32_t len)
{
  MSB_16_SET(p, p);
  uint32_t bufLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
  uint32_t xferLen;
  for (uint32_t i = 0; i < bufLen; i++)
  {
    buffer16[i] = p;
  }

  while (len)
  {
    xferLen = (bufLen < len) ? bufLen : len;
    WRITEBUF(buffer, xferLen * 2);
    len -= xferLen;
  }
}

void Arduino_mbedSPI::writeBytes(uint8_t *data, uint32_t len)
{
  WRITEBUF(data, len);
}

void Arduino_mbedSPI::writePixels(uint16_t *data, uint32_t len)
{
  uint32_t xferLen;
  uint8_t *p;
  union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;
  while (len)
  {
    xferLen = (len < SPI_MAX_PIXELS_AT_ONCE) ? len : SPI_MAX_PIXELS_AT_ONCE;
    p = buffer;
    for (uint32_t i = 0; i < xferLen; i++)
    {
      t.val = *data++;
      *p++ = t.msb;
      *p++ = t.lsb;
    }
    xferLen += xferLen;
    WRITEBUF(buffer, xferLen);
    len -= xferLen;
  }
}

/**
 * @param data uint8_t *
 * @param len uint8_t
 * @param repeat uint32_t
 */
void Arduino_mbedSPI::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
#if defined(ESP8266) || defined(ESP32)
  HWSPI.writePattern(data, len, repeat);
#else  // !(defined(ESP8266) || defined(ESP32))
  while (repeat--)
  {
    for (uint8_t i = 0; i < len; i++)
    {
      write(data[i]);
    }
  }
#endif // !(defined(ESP8266) || defined(ESP32))
}

INLINE void Arduino_mbedSPI::WRITE(uint8_t d)
{
  _dev->write((const char *)&d, 1, NULL, 0);
}

INLINE void Arduino_mbedSPI::WRITE16(uint16_t d)
{
  MSB_16_SET(d, d);
  _dev->write((const char *)&d, 2, NULL, 0);
}

INLINE void Arduino_mbedSPI::WRITE32(uint32_t d)
{
  MSB_32_SET(d, d);
  _dev->write((const char *)&d, 4, NULL, 0);
}

INLINE void Arduino_mbedSPI::WRITEBUF(uint8_t *buf, size_t count)
{
  _dev->write((const char *)buf, count, NULL, 0);
}

/******** low level bit twiddling **********/

INLINE void Arduino_mbedSPI::CS_HIGH(void)
{
  if (_cs >= 0)
  {
    _csGpio->write(HIGH);
  }
}

INLINE void Arduino_mbedSPI::CS_LOW(void)
{
  if (_cs >= 0)
  {
    _csGpio->write(LOW);
  }
}

INLINE void Arduino_mbedSPI::DC_HIGH(void)
{
  _dcGpio->write(HIGH);
}

INLINE void Arduino_mbedSPI::DC_LOW(void)
{
  _dcGpio->write(LOW);
}

#endif // #ifdef ARDUINO_ARCH_NRF52840
