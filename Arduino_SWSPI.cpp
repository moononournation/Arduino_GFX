/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_SWSPI.h"

Arduino_SWSPI::Arduino_SWSPI(int8_t dc, int8_t cs, int8_t sck, int8_t mosi, int8_t miso)
{
  _dc = dc;
  _cs = cs;
  _sck = sck;
  _mosi = mosi;
  _miso = miso;
}

void Arduino_SWSPI::begin(uint32_t speed)
{
  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode
  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // Deselect
  }
  pinMode(_mosi, OUTPUT);
  digitalWrite(_mosi, LOW);
  pinMode(_sck, OUTPUT);
  digitalWrite(_sck, LOW);
  if (_miso >= 0)
  {
    pinMode(_miso, INPUT);
  }

#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(CORE_TEENSY)
#if !defined(KINETISK)
  dcPinMask = digitalPinToBitMask(dc);
  sckPinMask = digitalPinToBitMask(sck);
  mosiPinMask = digitalPinToBitMask(mosi);
#endif
  dcPortSet = portSetRegister(dc);
  dcPortClr = portClearRegister(dc);
  sckPortSet = portSetRegister(sck);
  sckPortClr = portClearRegister(sck);
  mosiPortSet = portSetRegister(mosi);
  mosiPortClr = portClearRegister(mosi);
  if (cs >= 0)
  {
#if !defined(KINETISK)
    csPinMask = digitalPinToBitMask(cs);
#endif
    csPortSet = portSetRegister(cs);
    csPortClr = portClearRegister(cs);
  }
  else
  {
#if !defined(KINETISK)
    csPinMask = 0;
#endif
    csPortSet = dcPortSet;
    csPortClr = dcPortClr;
  }
  if (miso >= 0)
  {
    misoPort = portInputRegister(miso);
#if !defined(KINETISK)
    misoPinMask = digitalPinToBitMask(miso);
#endif
  }
  else
  {
    misoPort = portInputRegister(dc);
  }
#else  // !CORE_TEENSY
  dcPinMask = digitalPinToBitMask(dc);
  sckPinMask = digitalPinToBitMask(sck);
  mosiPinMask = digitalPinToBitMask(mosi);
  dcPortSet = &(PORT->Group[g_APinDescription[dc].ulPort].OUTSET.reg);
  dcPortClr = &(PORT->Group[g_APinDescription[dc].ulPort].OUTCLR.reg);
  sckPortSet = &(PORT->Group[g_APinDescription[sck].ulPort].OUTSET.reg);
  sckPortClr = &(PORT->Group[g_APinDescription[sck].ulPort].OUTCLR.reg);
  mosiPortSet = &(PORT->Group[g_APinDescription[mosi].ulPort].OUTSET.reg);
  mosiPortClr = &(PORT->Group[g_APinDescription[mosi].ulPort].OUTCLR.reg);
  if (cs >= 0)
  {
    csPinMask = digitalPinToBitMask(cs);
    csPortSet = &(PORT->Group[g_APinDescription[cs].ulPort].OUTSET.reg);
    csPortClr = &(PORT->Group[g_APinDescription[cs].ulPort].OUTCLR.reg);
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
  if (miso >= 0)
  {
    misoPinMask = digitalPinToBitMask(miso);
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(miso));
  }
  else
  {
    misoPinMask = 0;
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(dc));
  }
#endif // end !CORE_TEENSY
#else  // !HAS_PORT_SET_CLR
  dcPort = (PORTreg_t)portOutputRegister(digitalPinToPort(dc));
  dcPinMaskSet = digitalPinToBitMask(dc);
  sckPort = (PORTreg_t)portOutputRegister(digitalPinToPort(sck));
  sckPinMaskSet = digitalPinToBitMask(sck);
  mosiPort = (PORTreg_t)portOutputRegister(digitalPinToPort(mosi));
  mosiPinMaskSet = digitalPinToBitMask(mosi);
  if (cs >= 0)
  {
    csPort = (PORTreg_t)portOutputRegister(digitalPinToPort(cs));
    csPinMaskSet = digitalPinToBitMask(cs);
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
  if (miso >= 0)
  {
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(miso));
    misoPinMask = digitalPinToBitMask(miso);
  }
  else
  {
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(dc));
    misoPinMask = 0;
  }
  csPinMaskClr = ~csPinMaskSet;
  dcPinMaskClr = ~dcPinMaskSet;
  sckPinMaskClr = ~sckPinMaskSet;
  mosiPinMaskClr = ~mosiPinMaskSet;
#endif // !end HAS_PORT_SET_CLR
#endif // end USE_FAST_PINIO
}

void Arduino_SWSPI::beginWrite()
{
  DC_HIGH();
  CS_LOW();
}

void Arduino_SWSPI::writeCommand(uint8_t c)
{
  DC_LOW();
  write(c);
  DC_HIGH();
}

void Arduino_SWSPI::writeCommand16(uint16_t c)
{
  DC_LOW();
  write16(c);
  DC_HIGH();
}

void Arduino_SWSPI::write(uint8_t d)
{
  for (uint8_t bit = 0; bit < 8; bit++)
  {
    if (d & 0x80)
      SPI_MOSI_HIGH();
    else
      SPI_MOSI_LOW();
    SPI_SCK_HIGH();
    d <<= 1;
    SPI_SCK_LOW();
  }
}

void Arduino_SWSPI::write16(uint16_t d)
{
  for (uint8_t bit = 0; bit < 16; bit++)
  {
    if (d & 0x8000)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    SPI_SCK_HIGH();
    SPI_SCK_LOW();
    d <<= 1;
  }
}

void Arduino_SWSPI::write32(uint32_t d)
{
  for (uint8_t bit = 0; bit < 32; bit++)
  {
    if (d & 0x80000000)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    SPI_SCK_HIGH();
    SPI_SCK_LOW();
    d <<= 1;
  }
}

void Arduino_SWSPI::endWrite()
{
  CS_HIGH();
}

void Arduino_SWSPI::sendCommand(uint8_t c)
{
  CS_LOW();

  writeCommand(c);

  CS_HIGH();
}

void Arduino_SWSPI::sendCommand16(uint16_t c)
{
  CS_LOW();

  writeCommand16(c);

  CS_HIGH();
}

void Arduino_SWSPI::sendData(uint8_t d)
{
  CS_LOW();

  write(d);

  CS_HIGH();
}

void Arduino_SWSPI::sendData16(uint16_t d)
{
  CS_LOW();

  write16(d);

  CS_HIGH();
}

void Arduino_SWSPI::sendData32(uint32_t d)
{
  CS_LOW();

  write32(d);

  CS_HIGH();
}

void Arduino_SWSPI::setDataMode(uint8_t dataMode)
{
  // Not Implemented
}

void Arduino_SWSPI::writeRepeat(uint16_t p, uint32_t len)
{
  while (len--)
  {
    write16(p);
  }
}

void Arduino_SWSPI::writePixels(uint16_t *data, uint32_t len)
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
void Arduino_SWSPI::writePattern(uint8_t *data, uint8_t size, uint32_t repeat)
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

inline void Arduino_SWSPI::CS_HIGH(void)
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

inline void Arduino_SWSPI::CS_LOW(void)
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

inline void Arduino_SWSPI::DC_HIGH(void)
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

inline void Arduino_SWSPI::DC_LOW(void)
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

/*!
    @brief  Set the software (bitbang) SPI MOSI line HIGH.
*/
inline void Arduino_SWSPI::SPI_MOSI_HIGH(void)
{
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *mosiPortSet = 1;
#else // !KINETISK
  *mosiPortSet = mosiPinMask;
#endif
#else  // !HAS_PORT_SET_CLR
  *mosiPort |= mosiPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_mosi, HIGH);
#if defined(ESP32)
  for (volatile uint8_t i = 0; i < 1; i++)
    ;
#endif // end ESP32
#endif // end !USE_FAST_PINIO
}

/*!
    @brief  Set the software (bitbang) SPI MOSI line LOW.
*/
inline void Arduino_SWSPI::SPI_MOSI_LOW(void)
{
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *mosiPortClr = 1;
#else // !KINETISK
  *mosiPortClr = mosiPinMask;
#endif
#else  // !HAS_PORT_SET_CLR
  *mosiPort &= mosiPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_mosi, LOW);
#if defined(ESP32)
  for (volatile uint8_t i = 0; i < 1; i++)
    ;
#endif // end ESP32
#endif // end !USE_FAST_PINIO
}

/*!
    @brief  Set the software (bitbang) SPI SCK line HIGH.
*/
inline void Arduino_SWSPI::SPI_SCK_HIGH(void)
{
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *sckPortSet = 1;
#else                                                // !KINETISK
  *sckPortSet = sckPinMask;
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
  for (volatile uint8_t i = 0; i < 1; i++)
    ;
#endif
#endif
#else  // !HAS_PORT_SET_CLR
  *sckPort |= sckPinMaskSet;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_sck, HIGH);
#if defined(ESP32)
  for (volatile uint8_t i = 0; i < 1; i++)
    ;
#endif // end ESP32
#endif // end !USE_FAST_PINIO
}

/*!
    @brief  Set the software (bitbang) SPI SCK line LOW.
*/
inline void Arduino_SWSPI::SPI_SCK_LOW(void)
{
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if defined(KINETISK)
  *sckPortClr = 1;
#else                                                // !KINETISK
  *sckPortClr = sckPinMask;
#if defined(__IMXRT1052__) || defined(__IMXRT1062__) // Teensy 4.x
  for (volatile uint8_t i = 0; i < 1; i++)
    ;
#endif
#endif
#else  // !HAS_PORT_SET_CLR
  *sckPort &= sckPinMaskClr;
#endif // end !HAS_PORT_SET_CLR
#else  // !USE_FAST_PINIO
  digitalWrite(_sck, LOW);
#if defined(ESP32)
  for (volatile uint8_t i = 0; i < 1; i++)
    ;
#endif // end ESP32
#endif // end !USE_FAST_PINIO
}

/*!
    @brief   Read the state of the software (bitbang) SPI MISO line.
    @return  true if HIGH, false if LOW.
*/
inline bool Arduino_SWSPI::SPI_MISO_READ(void)
{
#if defined(USE_FAST_PINIO)
#if defined(KINETISK)
  return *misoPort;
#else  // !KINETISK
  return *misoPort & misoPinMask;
#endif // end !KINETISK
#else  // !USE_FAST_PINIO
  return digitalRead(_miso);
#endif // end !USE_FAST_PINIO
}