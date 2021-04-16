/*
 * start rewrite from:
 * https://github.com/daumemo/IPS_LCD_R61529_FT6236_Arduino_eSPI_Test
 */
#ifdef ESP32

#include "Arduino_DataBus.h"
#include "Arduino_ESP32PAR8.h"

// #define SET_DATA_AND_WR_AT_THE_SAME_TIME

Arduino_ESP32PAR8::Arduino_ESP32PAR8(
    int8_t dc, int8_t cs, int8_t wr, int8_t rd,
    int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7)
    : _dc(dc), _cs(cs), _wr(wr), _rd(rd),
      _d0(d0), _d1(d1), _d2(d2), _d3(d3), _d4(d4), _d5(d5), _d6(d6), _d7(d7)
{
}

void Arduino_ESP32PAR8::begin(int32_t speed, int8_t dataMode)
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

  // INIT 8-bit mask
  dataClrMask = (1 << _wr) | (1 << _d0) | (1 << _d1) | (1 << _d2) | (1 << _d3) | (1 << _d4) | (1 << _d5) | (1 << _d6) | (1 << _d7);
  for (int32_t c = 0; c < 256; c++)
  {
#if defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
    xset_mask[c] = (1 << _wr);
#else  // !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
    xset_mask[c] = 0;
#endif // !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
    if (c & 0x01)
    {
      xset_mask[c] |= (1 << _d0);
    }
    if (c & 0x02)
    {
      xset_mask[c] |= (1 << _d1);
    }
    if (c & 0x04)
    {
      xset_mask[c] |= (1 << _d2);
    }
    if (c & 0x08)
    {
      xset_mask[c] |= (1 << _d3);
    }
    if (c & 0x10)
    {
      xset_mask[c] |= (1 << _d4);
    }
    if (c & 0x20)
    {
      xset_mask[c] |= (1 << _d5);
    }
    if (c & 0x40)
    {
      xset_mask[c] |= (1 << _d6);
    }
    if (c & 0x80)
    {
      xset_mask[c] |= (1 << _d7);
    }
  }
  dataPortSet = (PORTreg_t)&GPIO.out_w1ts;
  dataPortClr = (PORTreg_t)&GPIO.out_w1tc;
}

void Arduino_ESP32PAR8::beginWrite()
{
  DC_HIGH();
  CS_LOW();
}

void Arduino_ESP32PAR8::endWrite()
{
  CS_HIGH();
}

void Arduino_ESP32PAR8::writeCommand(uint8_t c)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();
}

void Arduino_ESP32PAR8::writeCommand16(uint16_t c)
{
  DC_LOW();

  _data16.value = c;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);

  DC_HIGH();
}

void Arduino_ESP32PAR8::write(uint8_t d)
{
  WRITE(d);
}

void Arduino_ESP32PAR8::write16(uint16_t d)
{
  _data16.value = d;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);
}

void Arduino_ESP32PAR8::writeRepeat(uint16_t p, uint32_t len)
{
  _data16.value = p;
  if (_data16.msb == _data16.lsb)
  {
    uint32_t setMask = xset_mask[_data16.msb];
    *dataPortClr = dataClrMask;
    *dataPortSet = setMask;
    while (len--)
    {
      *wrPortClr = wrPinMask;
      *wrPortSet = wrPinMask;
      *wrPortClr = wrPinMask;
      *wrPortSet = wrPinMask;
    }
  }
  else
  {
    uint32_t hiMask = xset_mask[_data16.msb];
    uint32_t loMask = xset_mask[_data16.lsb];
    while (len--)
    {
      *dataPortClr = dataClrMask;
      *dataPortSet = hiMask;
#if !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
      *wrPortSet = wrPinMask;
#endif // !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
      *dataPortClr = dataClrMask;
      *dataPortSet = loMask;
#if !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
      *wrPortSet = wrPinMask;
#endif // !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
    }
  }
}

void Arduino_ESP32PAR8::writePixels(uint16_t *data, uint32_t len)
{
  while (len--)
  {
    _data16.value = *data++;
    WRITE(_data16.msb);
    WRITE(_data16.lsb);
  }
}

void Arduino_ESP32PAR8::writeC8D8(uint8_t c, uint8_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d);
}

void Arduino_ESP32PAR8::writeC8D16(uint8_t c, uint16_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  _data16.value = d;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);
}

void Arduino_ESP32PAR8::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  _data16.value = d1;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);
  _data16.value = d2;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);
}

void Arduino_ESP32PAR8::writeBytes(uint8_t *data, uint32_t len)
{
  while (len--)
  {
    WRITE(*data++);
  }
}

void Arduino_ESP32PAR8::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  while (repeat--)
  {
    writeBytes(data, len);
  }
}

void Arduino_ESP32PAR8::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  while (len--)
  {
    _data16.value = idx[*data++];
    WRITE(_data16.msb);
    WRITE(_data16.lsb);
  }
}

void Arduino_ESP32PAR8::writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len)
{
  while (len--)
  {
    _data16.value = idx[*data++];
    WRITE(_data16.msb);
    WRITE(_data16.lsb);
    WRITE(_data16.msb);
    WRITE(_data16.lsb);
  }
}

INLINE void Arduino_ESP32PAR8::WRITE(uint8_t d)
{
  uint32_t mask = xset_mask[d];
  *dataPortClr = dataClrMask;
  *dataPortSet = mask;
#if !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
  *wrPortSet = wrPinMask;
#endif // !defined(SET_DATA_AND_WR_AT_THE_SAME_TIME)
}

/******** low level bit twiddling **********/

INLINE void Arduino_ESP32PAR8::DC_HIGH(void)
{
  *dcPortSet = dcPinMask;
}

INLINE void Arduino_ESP32PAR8::DC_LOW(void)
{
  *dcPortClr = dcPinMask;
}

INLINE void Arduino_ESP32PAR8::CS_HIGH(void)
{
  *csPortSet = csPinMask;
}

INLINE void Arduino_ESP32PAR8::CS_LOW(void)
{
  *csPortClr = csPinMask;
}

#endif
