/*
 * start rewrite from:
 * https://github.com/Bodmer/TFT_eSPI.git
 */
#ifdef ESP8266
#include <SPI.h>
#include "Arduino_ESP8266SPI.h"

#define SPI_MAX_PIXELS_AT_ONCE 32

Arduino_ESP8266SPI::Arduino_ESP8266SPI(int8_t dc, int8_t cs /* = -1 */)
    : _dc(dc), _cs(cs)
{
}

void Arduino_ESP8266SPI::begin(int32_t speed, int8_t dataMode)
{
  _speed = speed ? speed : SPI_DEFAULT_FREQ;
  _dataMode = dataMode;

  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode
  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // Deselect
  }

  _dcPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_dc));
  _dcPinMaskSet = digitalPinToBitMask(_dc);
  if (_cs >= 0)
  {
    _csPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_cs));
    _csPinMaskSet = digitalPinToBitMask(_cs);
  }
  else
  {
    // No chip-select line defined; might be permanently tied to GND.
    // Assign a valid GPIO register (though not used for CS), and an
    // empty pin bitmask...the nonsense bit-twiddling might be faster
    // than checking _cs and possibly branching.
    _csPort = _dcPort;
    _csPinMaskSet = 0;
  }
  _csPinMaskClr = ~_csPinMaskSet;
  _dcPinMaskClr = ~_dcPinMaskSet;

  SPI.begin();
  if (_dataMode < 0)
  {
    _dataMode = SPI_MODE0;
  }
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(_dataMode);
  SPI.setFrequency(_speed);
}

void Arduino_ESP8266SPI::beginWrite()
{
  DC_HIGH();

  CS_LOW();
}

void Arduino_ESP8266SPI::writeCommand(uint8_t c)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();
}

void Arduino_ESP8266SPI::writeCommand16(uint16_t c)
{
  DC_LOW();

  MSB_16_SET(c, c);
  WRITE16(c);

  DC_HIGH();
}

void Arduino_ESP8266SPI::writeCommand32(uint32_t c)
{
  DC_LOW();

  MSB_32_SET(c, c);
  WRITE32(c);

  DC_HIGH();
}

void Arduino_ESP8266SPI::write(uint8_t d)
{
  WRITE(d);
}

void Arduino_ESP8266SPI::write16(uint16_t d)
{
  MSB_16_SET(d, d);
  WRITE16(d);
}

void Arduino_ESP8266SPI::write32(uint32_t d)
{
  MSB_32_SET(d, d);
  WRITE32(d);
}

void Arduino_ESP8266SPI::writeC8D8(uint8_t c, uint8_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d);
}

void Arduino_ESP8266SPI::writeC8D16(uint8_t c, uint16_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  MSB_16_SET(d, d);
  WRITE16(d);
}

void Arduino_ESP8266SPI::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  uint32_t d;
  MSB_32_16_16_SET(d, d1, d2);
  WRITE32(d);
}

void Arduino_ESP8266SPI::endWrite()
{
  CS_HIGH();
}

void Arduino_ESP8266SPI::sendCommand(uint8_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_ESP8266SPI::sendCommand16(uint16_t c)
{
  CS_LOW();

  DC_LOW();

  MSB_16_SET(c, c);
  WRITE16(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_ESP8266SPI::sendCommand32(uint32_t c)
{
  CS_LOW();

  DC_LOW();

  MSB_32_SET(c, c);
  WRITE32(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_ESP8266SPI::sendData(uint8_t d)
{
  CS_LOW();

  WRITE(d);

  CS_HIGH();
}

void Arduino_ESP8266SPI::sendData16(uint16_t d)
{
  CS_LOW();

  MSB_16_SET(d, d);
  WRITE16(d);

  CS_HIGH();
}

void Arduino_ESP8266SPI::sendData32(uint32_t d)
{
  CS_LOW();

  MSB_32_SET(d, d);
  WRITE32(d);

  CS_HIGH();
}

void Arduino_ESP8266SPI::writeRepeat(uint16_t p, uint32_t len)
{
  static uint8_t temp[2];
  temp[0] = p >> 8;
  temp[1] = p & 0xFF;
  SPI.writePattern((uint8_t *)temp, 2, len);
}

void Arduino_ESP8266SPI::writeBytes(uint8_t *data, uint32_t len)
{
  SPI.writeBytes(data, len);
}

void Arduino_ESP8266SPI::writePixels(uint16_t *data, uint32_t len)
{
  uint8_t *data8 = (uint8_t *)data;

  uint32_t color[8];

  SPI1U1 = (255 << SPILMOSI) | (255 << SPILMISO);

  while (len > 15)
  {
    uint32_t i = 0;
    while (i < 8)
    {
      MSB_32_8_ARRAY_SET(color[i++], data8);
      data8 += 4;
    }

    len -= 16;

    // ESP8266 wait time here at 40MHz SPI is ~5.45us
    while (SPI1CMD & SPIBUSY)
      ;
    SPI1W0 = color[0];
    SPI1W1 = color[1];
    SPI1W2 = color[2];
    SPI1W3 = color[3];
    SPI1W4 = color[4];
    SPI1W5 = color[5];
    SPI1W6 = color[6];
    SPI1W7 = color[7];
    SPI1CMD |= SPIBUSY;
  }

  if (len)
  {
    uint32_t i = 0;
    uint32_t bits = (len * 16 - 1); // bits left to shift - 1
    len = (len + 1) >> 1;
    while (len--)
    {
      MSB_32_8_ARRAY_SET(color[i++], data8);
      data8 += 4;
    }

    while (SPI1CMD & SPIBUSY)
      ;
    SPI1U1 = (bits << SPILMOSI) | (bits << SPILMISO);
    SPI1W0 = color[0];
    SPI1W1 = color[1];
    SPI1W2 = color[2];
    SPI1W3 = color[3];
    SPI1W4 = color[4];
    SPI1W5 = color[5];
    SPI1W6 = color[6];
    SPI1W7 = color[7];
    SPI1CMD |= SPIBUSY;
  }

  while (SPI1CMD & SPIBUSY)
    ;
}

/**
 * @param data uint8_t *
 * @param len uint8_t
 * @param repeat uint32_t
 */
void Arduino_ESP8266SPI::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  SPI.writePattern(data, len, repeat);
}

INLINE void Arduino_ESP8266SPI::WRITE(uint8_t d)
{
  SPI1U1 = (7 << SPILMOSI);
  SPI1W0 = d;
  SPI1CMD |= SPIBUSY;
  while (SPI1CMD & SPIBUSY)
    ;
}

INLINE void Arduino_ESP8266SPI::WRITE16(uint16_t d)
{
  SPI1U1 = (15 << SPILMOSI);
  SPI1W0 = d;
  SPI1CMD |= SPIBUSY;
  while (SPI1CMD & SPIBUSY)
    ;
}

INLINE void Arduino_ESP8266SPI::WRITE32(uint32_t d)
{
  SPI1U1 = (31 << SPILMOSI);
  SPI1W0 = d;
  SPI1CMD |= SPIBUSY;
  while (SPI1CMD & SPIBUSY)
    ;
}

/******** low level bit twiddling **********/

INLINE void Arduino_ESP8266SPI::CS_HIGH(void)
{
  if (_cs >= 0)
  {
    *_csPort |= _csPinMaskSet;
  }
}

INLINE void Arduino_ESP8266SPI::CS_LOW(void)
{
  if (_cs >= 0)
  {
    *_csPort &= _csPinMaskClr;
  }
}

INLINE void Arduino_ESP8266SPI::DC_HIGH(void)
{
  *_dcPort |= _dcPinMaskSet;
}

INLINE void Arduino_ESP8266SPI::DC_LOW(void)
{
  *_dcPort &= _dcPinMaskClr;
}

#endif // ESP8266
