/*
 * start rewrite from:
 * https://github.com/daumemo/IPS_LCD_R61529_FT6236_Arduino_eSPI_Test
 */
#ifdef ESP32

#include "Arduino_DataBus.h"
#include "Arduino_ESP32PAR16.h"

Arduino_ESP32PAR16::Arduino_ESP32PAR16(
    int8_t dc, int8_t cs, int8_t wr, int8_t rd,
    int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7,
    int8_t d8, int8_t d9, int8_t d10, int8_t d11, int8_t d12, int8_t d13, int8_t d14, int8_t d15)
    : _dc(dc), _cs(cs), _wr(wr), _rd(rd),
      _d0(d0), _d1(d1), _d2(d2), _d3(d3), _d4(d4), _d5(d5), _d6(d6), _d7(d7),
      _d8(d8), _d9(d9), _d10(d10), _d11(d11), _d12(d12), _d13(d13), _d14(d14), _d15(d15)
{
}

void Arduino_ESP32PAR16::begin(int32_t speed, int8_t dataMode)
{
  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode
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

  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // disable chip select
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
    csPinMask = 0;
    csPortSet = dcPortSet;
    csPortClr = dcPortClr;
  }

  pinMode(_wr, OUTPUT);
  digitalWrite(_wr, HIGH); // Set write strobe high (inactive)
  if (_wr >= 32)
  {
    wrPinMask = digitalPinToBitMask(_wr);
    wrPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    wrPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else if (_wr >= 0)
  {
    wrPinMask = digitalPinToBitMask(_wr);
    wrPortSet = (PORTreg_t)&GPIO.out_w1ts;
    wrPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }

  if (_rd >= 0)
  {
    pinMode(_rd, OUTPUT);
    digitalWrite(_rd, HIGH);
  }
  if (_rd >= 32)
  {
    rdPinMask = digitalPinToBitMask(_rd);
    rdPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    rdPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else if (_rd >= 0)
  {
    rdPinMask = digitalPinToBitMask(_rd);
    rdPortSet = (PORTreg_t)&GPIO.out_w1ts;
    rdPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }
  else
  {
    rdPinMask = 0;
    rdPortSet = dcPortSet;
    rdPortClr = dcPortClr;
  }

  // TODO: check pin range 0-31
  pinMode(_d0, OUTPUT);
  digitalWrite(_d0, HIGH);
  pinMode(_d1, OUTPUT);
  digitalWrite(_d1, HIGH);
  pinMode(_d2, OUTPUT);
  digitalWrite(_d2, HIGH);
  pinMode(_d3, OUTPUT);
  digitalWrite(_d3, HIGH);
  pinMode(_d4, OUTPUT);
  digitalWrite(_d4, HIGH);
  pinMode(_d5, OUTPUT);
  digitalWrite(_d5, HIGH);
  pinMode(_d6, OUTPUT);
  digitalWrite(_d6, HIGH);
  pinMode(_d7, OUTPUT);
  digitalWrite(_d7, HIGH);
  pinMode(_d8, OUTPUT);
  digitalWrite(_d8, HIGH);
  pinMode(_d9, OUTPUT);
  digitalWrite(_d9, HIGH);
  pinMode(_d10, OUTPUT);
  digitalWrite(_d10, HIGH);
  pinMode(_d11, OUTPUT);
  digitalWrite(_d11, HIGH);
  pinMode(_d12, OUTPUT);
  digitalWrite(_d12, HIGH);
  pinMode(_d13, OUTPUT);
  digitalWrite(_d13, HIGH);
  pinMode(_d14, OUTPUT);
  digitalWrite(_d14, HIGH);
  pinMode(_d15, OUTPUT);
  digitalWrite(_d15, HIGH);

  // INIT 16-bit mask
  dataClrMask = (1 << _wr) | (1 << _d0) | (1 << _d1) | (1 << _d2) | (1 << _d3) | (1 << _d4) | (1 << _d5) | (1 << _d6) | (1 << _d7) | (1 << _d8) | (1 << _d9) | (1 << _d10) | (1 << _d11) | (1 << _d12) | (1 << _d13) | (1 << _d14) | (1 << _d15);
  for (int32_t c = 0; c < 256; c++)
  {
    xset_mask_lo[c] = (1 << _wr);
    if (c & 0x01)
    {
      xset_mask_lo[c] |= (1 << _d0);
    }
    if (c & 0x02)
    {
      xset_mask_lo[c] |= (1 << _d1);
    }
    if (c & 0x04)
    {
      xset_mask_lo[c] |= (1 << _d2);
    }
    if (c & 0x08)
    {
      xset_mask_lo[c] |= (1 << _d3);
    }
    if (c & 0x10)
    {
      xset_mask_lo[c] |= (1 << _d4);
    }
    if (c & 0x20)
    {
      xset_mask_lo[c] |= (1 << _d5);
    }
    if (c & 0x40)
    {
      xset_mask_lo[c] |= (1 << _d6);
    }
    if (c & 0x80)
    {
      xset_mask_lo[c] |= (1 << _d7);
    }
  }
  for (int32_t c = 0; c < 256; c++)
  {
    xset_mask_hi[c] = 0;
    if (c & 0x01)
    {
      xset_mask_hi[c] |= (1 << _d8);
    }
    if (c & 0x02)
    {
      xset_mask_hi[c] |= (1 << _d9);
    }
    if (c & 0x04)
    {
      xset_mask_hi[c] |= (1 << _d10);
    }
    if (c & 0x08)
    {
      xset_mask_hi[c] |= (1 << _d11);
    }
    if (c & 0x10)
    {
      xset_mask_hi[c] |= (1 << _d12);
    }
    if (c & 0x20)
    {
      xset_mask_hi[c] |= (1 << _d13);
    }
    if (c & 0x40)
    {
      xset_mask_hi[c] |= (1 << _d14);
    }
    if (c & 0x80)
    {
      xset_mask_hi[c] |= (1 << _d15);
    }
  }
  dataPortSet = (PORTreg_t)&GPIO.out_w1ts;
  dataPortClr = (PORTreg_t)&GPIO.out_w1tc;
}

void Arduino_ESP32PAR16::beginWrite()
{
  DC_HIGH();
  CS_LOW();
}

void Arduino_ESP32PAR16::writeCommand(uint8_t c)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();
}

void Arduino_ESP32PAR16::writeCommand16(uint16_t c)
{
  DC_LOW();

  WRITE(c >> 8);
  WRITE(c);

  DC_HIGH();
}

void Arduino_ESP32PAR16::writeCommand32(uint32_t c)
{
  DC_LOW();

  WRITE(c >> 24);
  WRITE(c >> 16);
  WRITE(c >> 8);
  WRITE(c);

  DC_HIGH();
}

void Arduino_ESP32PAR16::write(uint8_t d)
{
  WRITE(d);
}

void Arduino_ESP32PAR16::write16(uint16_t d)
{
  WRITE16(d);
}

void Arduino_ESP32PAR16::write32(uint32_t d)
{
  WRITE(d >> 24);
  WRITE(d >> 16);
  WRITE(d >> 8);
  WRITE(d);
}

void Arduino_ESP32PAR16::writeC8D8(uint8_t c, uint8_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d);
}

void Arduino_ESP32PAR16::writeC8D16(uint8_t c, uint16_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d >> 8);
  WRITE(d);
}

void Arduino_ESP32PAR16::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d1 >> 8);
  WRITE(d1);
  WRITE(d2 >> 8);
  WRITE(d2);
}

void Arduino_ESP32PAR16::endWrite()
{
  CS_HIGH();
}

void Arduino_ESP32PAR16::sendCommand(uint8_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_ESP32PAR16::sendCommand16(uint16_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE(c >> 8);
  WRITE(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_ESP32PAR16::sendCommand32(uint32_t c)
{
  CS_LOW();

  DC_LOW();

  WRITE(c >> 24);
  WRITE(c >> 16);
  WRITE(c >> 8);
  WRITE(c);

  DC_HIGH();

  CS_HIGH();
}

void Arduino_ESP32PAR16::sendData(uint8_t d)
{
  CS_LOW();

  WRITE(d);

  CS_HIGH();
}

void Arduino_ESP32PAR16::sendData16(uint16_t d)
{
  CS_LOW();

  WRITE(d >> 8);
  WRITE(d);

  CS_HIGH();
}

void Arduino_ESP32PAR16::sendData32(uint32_t d)
{
  CS_LOW();

  WRITE(d >> 24);
  WRITE(d >> 16);
  WRITE(d >> 8);
  WRITE(d);

  CS_HIGH();
}

void Arduino_ESP32PAR16::writeRepeat(uint16_t p, uint32_t len)
{
  uint32_t d = xset_mask_hi[p >> 8] | xset_mask_lo[p & 0xff];
  while (len--)
  {
    *dataPortClr = dataClrMask;
    *dataPortSet = d;
  }
}

void Arduino_ESP32PAR16::writeBytes(uint8_t *data, uint32_t len)
{
  while (len--)
  {
    WRITE(*data++);
  }
}

void Arduino_ESP32PAR16::writePixels(uint16_t *data, uint32_t len)
{
  while (len--)
  {
    uint16_t d = *data++;
    WRITE16(d);
  }
}

/**
 * @param data uint8_t *
 * @param len uint8_t
 * @param repeat uint32_t
 */
void Arduino_ESP32PAR16::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  while (repeat--)
  {
    writeBytes(data, len);
  }
}

/******** low level bit twiddling **********/

INLINE void Arduino_ESP32PAR16::WRITE(uint8_t d)
{
  *dataPortClr = dataClrMask;
  *dataPortSet = xset_mask_lo[d];
}

INLINE void Arduino_ESP32PAR16::WRITE16(uint16_t d)
{
  *dataPortClr = dataClrMask;
  *dataPortSet = xset_mask_hi[d >> 8] | xset_mask_lo[d & 0xff];
}

INLINE void Arduino_ESP32PAR16::DC_HIGH(void)
{
  *dcPortSet = dcPinMask;
}

INLINE void Arduino_ESP32PAR16::DC_LOW(void)
{
  *dcPortClr = dcPinMask;
}

INLINE void Arduino_ESP32PAR16::CS_HIGH(void)
{
  *csPortSet = csPinMask;
}

INLINE void Arduino_ESP32PAR16::CS_LOW(void)
{
  *csPortClr = csPinMask;
}

#endif
