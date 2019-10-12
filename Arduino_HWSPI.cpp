/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include <SPI.h>
#include "Arduino_DataBus.h"
#include "Arduino_HWSPI.h"

#if defined(SPI_HAS_TRANSACTION)
#define SPI_BEGIN_TRANSACTION() SPI.beginTransaction(mySPISettings)
#define SPI_END_TRANSACTION() SPI.endTransaction()
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
Arduino_HWSPI::Arduino_HWSPI(int8_t dc, int8_t cs /* = -1 */, int8_t sck /* = -1 */, int8_t mosi /* = -1 */, int8_t miso /* = -1 */)
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
  _speed = speed ? speed : SPI_DEFAULT_FREQ;

  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode
  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // Deselect
  }

#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(CORE_TEENSY)
#if !defined(KINETISK)
  dcPinMask = digitalPinToBitMask(_dc);
#endif
  dcPortSet = portSetRegister(_dc);
  dcPortClr = portClearRegister(_dc);
  if (_cs >= 0)
  {
#if !defined(KINETISK)
    csPinMask = digitalPinToBitMask(_cs);
#endif
    csPortSet = portSetRegister(_cs);
    csPortClr = portClearRegister(_cs);
  }
  else
  {
#if !defined(KINETISK)
    csPinMask = 0;
#endif
    csPortSet = dcPortSet;
    csPortClr = dcPortClr;
  }
#elif defined(ESP32)
  dcPinMask = digitalPinToBitMask(_dc);
  if (_dc >= 32)
  {
    dcPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    dcPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else
  {
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
  else
  {
    // No chip-select line defined; might be permanently tied to GND.
    // Assign a valid GPIO register (though not used for CS), and an
    // empty pin bitmask...the nonsense bit-twiddling might be faster
    // than checking _cs and possibly branching.
    csPortSet = (PORTreg_t)dcPortSet;
    csPortClr = (PORTreg_t)dcPortClr;
    csPinMask = 0;
  }
#else  // !CORE_TEENSY
  dcPinMask = digitalPinToBitMask(_dc);
  dcPortSet = &(PORT->Group[g_APinDescription[_dc].ulPort].OUTSET.reg);
  dcPortClr = &(PORT->Group[g_APinDescription[_dc].ulPort].OUTCLR.reg);
  if (_cs >= 0)
  {
    csPinMask = digitalPinToBitMask(_cs);
    csPortSet = &(PORT->Group[g_APinDescription[_cs].ulPort].OUTSET.reg);
    csPortClr = &(PORT->Group[g_APinDescription[_cs].ulPort].OUTCLR.reg);
  }
  else
  {
    // No chip-select line defined; might be permanently tied to GND.
    // Assign a valid GPIO register (though not used for CS), and an
    // empty pin bitmask...the nonsense bit-twiddling might be faster
    // than checking _cs and possibly branching.
    csPortSet = dcPortSet;
    csPortClr = dcPortClr;
    csPinMask = 0;
  }
#endif // end !CORE_TEENSY
#else  // !HAS_PORT_SET_CLR
  dcPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_dc));
  dcPinMaskSet = digitalPinToBitMask(_dc);
  if (_cs >= 0)
  {
    csPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_cs));
    csPinMaskSet = digitalPinToBitMask(_cs);
  }
  else
  {
    // No chip-select line defined; might be permanently tied to GND.
    // Assign a valid GPIO register (though not used for CS), and an
    // empty pin bitmask...the nonsense bit-twiddling might be faster
    // than checking _cs and possibly branching.
    csPort = dcPort;
    csPinMaskSet = 0;
  }
  csPinMaskClr = ~csPinMaskSet;
  dcPinMaskClr = ~dcPinMaskSet;
#endif // !end HAS_PORT_SET_CLR
#endif // end USE_FAST_PINIO

#if defined(ESP32)
  SPI.begin(_sck, _miso, _mosi);
  mySPISettings = SPISettings(_speed, MSBFIRST, SPI_MODE0);
#elif defined(ESP8266)
  SPI.begin();
  mySPISettings = SPISettings(_speed, MSBFIRST, SPI_MODE0);
#elif defined(SPI_HAS_TRANSACTION)
  SPI.begin();
  mySPISettings = SPISettings(_speed, MSBFIRST, SPI_MODE2);
#elif defined(__AVR__) || defined(CORE_TEENSY)
  SPCRbackup = SPCR;
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE2);
  mySPCR = SPCR;     // save our preferred state
  SPCR = SPCRbackup; // then restore
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

void Arduino_HWSPI::writeCommand(uint8_t c)
{
  DC_LOW();
  write(c);
  DC_HIGH();
}

void Arduino_HWSPI::writeCommand16(uint16_t c)
{
  DC_LOW();
  write16(c);
  DC_HIGH();
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

void Arduino_HWSPI::endWrite()
{
  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::sendCommand(uint8_t c)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  writeCommand(c);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::sendCommand16(uint16_t c)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  writeCommand16(c);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::sendData(uint8_t d)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  write(d);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::sendData16(uint16_t d)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  write16(d);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::sendData32(uint32_t d)
{
  SPI_BEGIN_TRANSACTION();
  CS_LOW();

  write32(d);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_HWSPI::writeRepeat(uint16_t p, uint32_t len)
{
#if defined(ESP32)
#define SPI_MAX_PIXELS_AT_ONCE 32
#define TMPBUF_LONGWORDS ((SPI_MAX_PIXELS_AT_ONCE + 1) / 2)
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

void Arduino_HWSPI::writeBytes(uint8_t *data, uint32_t len)
{
#if defined(ESP8266) || defined(ESP32)
  SPI.writeBytes(data, len);
#else
  while (len--)
  {
    write(*d++);
  }
#endif
}

void Arduino_HWSPI::writePixels(uint16_t *data, uint32_t len)
{
#ifdef ESP32
  SPI.writePixels(data, len * 2); // don't know why len require *2
#elif defined(ESP8266)
  SPI.writePattern((uint8_t *)data, len * 2, 1);
#else
  uint8_t *d = (uint8_t *)data;
  while (len--)
  {
    write(*d++);
    write(*d++);
  }
#endif
}

/**
 * @param data uint8_t *
 * @param size uint8_t  max for size is 64Byte
 * @param repeat uint32_t
 */
void Arduino_HWSPI::writePattern(uint8_t *data, uint8_t size, uint32_t repeat)
{
#if defined(ESP8266) || defined(ESP32)
  SPI.writePattern(data, size, repeat);
#else
  while (repeat--)
  {
    for (uint8_t i = 0; i < size; i++)
    {
      write(data[i]);
    }
  }
#endif
}

void Arduino_HWSPI::setDataMode(uint8_t dataMode)
{
#if defined(SPI_HAS_TRANSACTION)
  mySPISettings = SPISettings(_speed, MSBFIRST, dataMode);
#else
  SPI.setDataMode(dataMode);
#endif
}

/******** low level bit twiddling **********/

inline void Arduino_HWSPI::CS_HIGH(void)
{
  if (_cs >= 0)
  {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
    *csPortSet = 1;
#else  // !KINETISK
    *csPortSet = csPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
    *csPort |= csPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
    digitalWrite(_cs, HIGH);
#endif // end !USE_FAST_PINIO
  }
}

inline void Arduino_HWSPI::CS_LOW(void)
{
  if (_cs >= 0)
  {
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
    *csPortClr = 1;
#else  // !KINETISK
    *csPortClr = csPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
    *csPort &= csPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
    digitalWrite(_cs, LOW);
#endif // end !USE_FAST_PINIO
  }
}

inline void Arduino_HWSPI::DC_HIGH(void)
{
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *dcPortSet = 1;
#else  // !KINETISK
  *dcPortSet = dcPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
  *dcPort |= dcPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_dc, HIGH);
#endif // end !USE_FAST_PINIO
}

inline void Arduino_HWSPI::DC_LOW(void)
{
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *dcPortClr = 1;
#else  // !KINETISK
  *dcPortClr = dcPinMask;
#endif // end !KINETISK
#else  // !HAS_PORT_SET_CLR
  *dcPort &= dcPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_dc, LOW);
#endif // end !USE_FAST_PINIO
}
