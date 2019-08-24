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
  dcPinMask = digitalPinToBitMask(_dc);
  sckPinMask = digitalPinToBitMask(_sck);
  mosiPinMask = digitalPinToBitMask(_mosi);
#endif
  dcPortSet = portSetRegister(_dc);
  dcPortClr = portClearRegister(_dc);
  sckPortSet = portSetRegister(_sck);
  sckPortClr = portClearRegister(_sck);
  mosiPortSet = portSetRegister(_mosi);
  mosiPortClr = portClearRegister(_mosi);
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
  if (_miso >= 0)
  {
    misoPort = portInputRegister(_miso);
#if !defined(KINETISK)
    misoPinMask = digitalPinToBitMask(_miso);
#endif
  }
  else
  {
    misoPort = portInputRegister(_miso);
  }
#elif defined(ESP32)
  dcPinMask = digitalPinToBitMask(_dc);
  sckPinMask = digitalPinToBitMask(_sck);
  mosiPinMask = digitalPinToBitMask(_mosi);
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
  if (_sck >= 32)
  {
    sckPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    sckPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else
  {
    sckPortSet = (PORTreg_t)&GPIO.out_w1ts;
    sckPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }
  if (_mosi >= 32)
  {
    mosiPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    mosiPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else
  {
    mosiPortSet = (PORTreg_t)&GPIO.out_w1ts;
    mosiPortClr = (PORTreg_t)&GPIO.out_w1tc;
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
  if (_miso >= 0)
  {
    misoPinMask = digitalPinToBitMask(_miso);
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(_miso));
  }
  else
  {
    misoPinMask = 0;
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(_dc));
  }
#else  // !CORE_TEENSY and !ESP32
  dcPinMask = digitalPinToBitMask(_dc);
  sckPinMask = digitalPinToBitMask(_sck);
  mosiPinMask = digitalPinToBitMask(_mosi);
  dcPortSet = &(PORT->Group[g_APinDescription[_dc].ulPort].OUTSET.reg);
  dcPortClr = &(PORT->Group[g_APinDescription[_dc].ulPort].OUTCLR.reg);
  sckPortSet = &(PORT->Group[g_APinDescription[_sck].ulPort].OUTSET.reg);
  sckPortClr = &(PORT->Group[g_APinDescription[_sck].ulPort].OUTCLR.reg);
  mosiPortSet = &(PORT->Group[g_APinDescription[_mosi].ulPort].OUTSET.reg);
  mosiPortClr = &(PORT->Group[g_APinDescription[_mosi].ulPort].OUTCLR.reg);
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
  if (_miso >= 0)
  {
    misoPinMask = digitalPinToBitMask(_miso);
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(_miso));
  }
  else
  {
    misoPinMask = 0;
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(_dc));
  }
#endif // end !CORE_TEENSY
#else  // !HAS_PORT_SET_CLR
  dcPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_dc));
  dcPinMaskSet = digitalPinToBitMask(_dc);
  sckPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_sck));
  sckPinMaskSet = digitalPinToBitMask(_sck);
  mosiPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_mosi));
  mosiPinMaskSet = digitalPinToBitMask(_mosi);
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
  if (miso >= 0)
  {
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(_miso));
    misoPinMask = digitalPinToBitMask(_miso);
  }
  else
  {
    misoPort = (PORTreg_t)portInputRegister(digitalPinToPort(_dc));
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
  uint8_t bit = 0x80;
  while (bit)
  {
    if (d & bit)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    SPI_SCK_HIGH();
    bit >>= 1;
    SPI_SCK_LOW();
  }
}

void Arduino_SWSPI::write16(uint16_t d)
{
  uint16_t bit = 0x8000;
  while (bit)
  {
    if (d & bit)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    SPI_SCK_HIGH();
    bit >>= 1;
    SPI_SCK_LOW();
  }
}

void Arduino_SWSPI::write32(uint32_t d)
{
  uint32_t bit = 0x80000000;
  while (bit)
  {
    if (d & bit)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    SPI_SCK_HIGH();
    bit >>= 1;
    SPI_SCK_LOW();
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
  if ((p == 0x0000) || (p == 0xffff)) // no need to set MOSI level while filling black or white
  {
    if (p)
    {
      SPI_MOSI_HIGH();
    }
    else
    {
      SPI_MOSI_LOW();
    }
    len *= 16;
    while (len--)
    {
      SPI_SCK_HIGH();
      SPI_SCK_LOW();
    }
  }
  else
  {
    uint16_t bit;
    while (len--)
    {
      bit = 0x8000;
      while (bit)
      {
        if (p & bit)
        {
          SPI_MOSI_HIGH();
        }
        else
        {
          SPI_MOSI_LOW();
        }
        SPI_SCK_HIGH();
        bit >>= 1;
        SPI_SCK_LOW();
      }
    }
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