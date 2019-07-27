/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/ananevilya/Arduino-ST7789-Library.git
 */
#include <SPI.h>
#include "Arduino_DataBus.h"
#include "Arduino_HWSPI.h"

#if defined(SPI_HAS_TRANSACTION)
#define SPI_BEGIN_TRANSACTION() SPI.beginTransaction(mySPISettings)
#define SPI_END_TRANSACTION() SPI.endTransaction()
#else
#define SPI_BEGIN_TRANSACTION() {}
#define SPI_END_TRANSACTION() {}
#endif

#if defined(SPI_HAS_TRANSACTION)
static SPISettings mySPISettings;
#elif defined(__AVR__) || defined(CORE_TEENSY)
static uint8_t SPCRbackup;
static uint8_t mySPCR;
#endif

#if defined(ESP32)
Arduino_HWSPI::Arduino_HWSPI(int8_t dc, int8_t cs  /* = -1 */, int8_t sck  /* = -1 */, int8_t mosi  /* = -1 */, int8_t miso  /* = -1 */)
{
  _sck = sck;
  _mosi = mosi;
  _miso = miso;
#else
Arduino_HWSPI::Arduino_HWSPI(int8_t dc, int8_t cs /* = -1 */)
{
#endif
  _dc = dc;
  _cs = cs;
}

void Arduino_HWSPI::begin(uint32_t speed)
{
  if(!speed) {
    speed = SPI_DEFAULT_FREQ;
  }

  pinMode(_dc, OUTPUT);
  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
  }

#if defined(USE_FAST_IO)
  dcport = portOutputRegister(digitalPinToPort(_dc));
  dcpinmask = digitalPinToBitMask(_dc);
  if (_cs)
  {
    csport = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
  }
#endif

#if defined(ESP32)
  SPI.begin(_sck, _miso, _mosi);
  mySPISettings = SPISettings(speed, MSBFIRST, SPI_MODE0);
#elif defined(ESP8266)
  SPI.begin();
  mySPISettings = SPISettings(speed, MSBFIRST, SPI_MODE0);
#elif defined(SPI_HAS_TRANSACTION)
  SPI.begin();
  mySPISettings = SPISettings(speed, MSBFIRST, SPI_MODE2);
#elif defined(__AVR__) || defined(CORE_TEENSY)
  SPCRbackup = SPCR;
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE2);
  mySPCR = SPCR;           // save our preferred state
  SPCR = SPCRbackup;       // then restore
#elif defined(__SAM3X8E__)
  SPI.begin();
  SPI.setClockDivider(21); //4MHz
  SPI.setDataMode(SPI_MODE2);
#endif
}

void Arduino_HWSPI::beginWrite()
{
  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();
}

void Arduino_HWSPI::write(uint8_t c)
{
#if defined(SPI_HAS_TRANSACTION)
  SPI.transfer(c);
#elif defined(__AVR__) || defined(CORE_TEENSY)
  SPCRbackup = SPCR;
  SPCR = mySPCR;
  SPI.transfer(c);
  SPCR = SPCRbackup;
#elif defined(__arm__)
  SPI.setClockDivider(21); //4MHz
  SPI.setDataMode(SPI_MODE2);
  SPI.transfer(c);
#endif
}

void Arduino_HWSPI::endWrite()
{
  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::writeCommand(uint8_t c)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  writeCommandCore(c);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::writeCommandCore(uint8_t c)
{
  DC_LOW();
  write(c);
  DC_HIGH();
}

void Arduino_HWSPI::writeData(uint8_t d)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  write(d);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::write16(uint16_t d)
{
#if defined(ESP8266) || defined(ESP32)
  SPI.write16(d);
#else
  write(d >> 8);
  write(d);
#endif
}

void Arduino_HWSPI::write32(uint32_t d)
{
#if defined(ESP8266) || defined(ESP32)
  SPI.write32(d);
#else
  write(d >> 24);
  write(d >> 16);
  write(d >> 8);
  write(d);
#endif
}

void Arduino_HWSPI::writePixels(uint16_t p, uint32_t len)
{
#ifdef ESP32
#define SPI_MAX_PIXELS_AT_ONCE 32
#define TMPBUF_LONGWORDS (SPI_MAX_PIXELS_AT_ONCE + 1) / 2
#define TMPBUF_PIXELS (TMPBUF_LONGWORDS * 2)
  static uint32_t temp[TMPBUF_LONGWORDS];
  uint32_t c32 = p * 0x00010001;
  uint16_t bufLen = (len < TMPBUF_PIXELS) ? len : TMPBUF_PIXELS,
           xferLen, fillLen;
  // Fill temp buffer 32 bits at a time
  fillLen = (bufLen + 1) / 2; // Round up to next 32-bit boundary
  for (uint32_t t = 0; t < fillLen; t++)
  {
    temp[t] = c32;
  }
  // Issue pixels in blocks from temp buffer
  while (len)
  {                                          // While pixels remain
    xferLen = (bufLen < len) ? bufLen : len; // How many this pass?
    SPI.writePixels((uint16_t *)temp, xferLen * 2);
    len -= xferLen;
  }
#elif defined(ESP8266)
  static uint8_t temp[2];
  temp[0] = p >> 8;
  temp[1] = p & 0xFF;
  SPI.writePattern((uint8_t *)temp, 2, len);
#else
  uint8_t hi = p >> 8, lo = p;

  while (len--)
  {
    write(hi);
    write(lo);
  }
#endif
}

/******** low level bit twiddling **********/

inline void Arduino_HWSPI::CS_HIGH(void)
{
  if (_cs >= 0)
  {
#if defined(USE_FAST_IO)
    *csport |= cspinmask;
#else
    digitalWrite(_cs, HIGH);
#endif
  }
}

inline void Arduino_HWSPI::CS_LOW(void)
{
  if (_cs >= 0)
  {
#if defined(USE_FAST_IO)
    *csport &= ~cspinmask;
#else
    digitalWrite(_cs, LOW);
#endif
  }
}

inline void Arduino_HWSPI::DC_HIGH(void)
{
#if defined(USE_FAST_IO)
  *dcport |= dcpinmask;
#else
  digitalWrite(_dc, HIGH);
#endif
}

inline void Arduino_HWSPI::DC_LOW(void)
{
#if defined(USE_FAST_IO)
  *dcport &= ~dcpinmask;
#else
  digitalWrite(_dc, LOW);
#endif
}
