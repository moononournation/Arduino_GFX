#include "Arduino_UNOPAR8.h"

// for MCUFriend UNO kind of shields. -jz-

Arduino_UNOPAR8::Arduino_UNOPAR8(int8_t dc, int8_t cs, int8_t wr, int8_t rd)
  : _dc(dc), _cs(cs), _wr(wr), _rd(rd)
{
}

bool Arduino_UNOPAR8::begin(int32_t speed, int8_t dataMode)
{
  UNUSED(speed);
  UNUSED(dataMode);

  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode
#ifdef __AVR__
  _dcPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_dc));
  _dcPinMaskSet = digitalPinToBitMask(_dc);
  _dcPinMaskClr = ~_dcPinMaskSet;
#endif

  if (_cs != GFX_NOT_DEFINED)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // Disable chip select
#ifdef __AVR__
    _csPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_cs));
    _csPinMaskSet = digitalPinToBitMask(_cs);
#endif
  }
#ifdef __AVR__
  _csPinMaskClr = ~_csPinMaskSet;
#endif

  pinMode(_wr, OUTPUT);
  digitalWrite(_wr, HIGH); // Set write strobe high (inactive)
#ifdef __AVR__
  _wrPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_wr));
  _wrPinMaskSet = digitalPinToBitMask(_wr);
  _wrPinMaskClr = ~_wrPinMaskSet;
#endif

  if (_rd != GFX_NOT_DEFINED)
  {
    pinMode(_rd, OUTPUT);
    digitalWrite(_rd, HIGH); // Disable RD
#ifdef __AVR__
    _rdPort = (PORTreg_t)portOutputRegister(digitalPinToPort(_rd));
    _rdPinMaskSet = digitalPinToBitMask(_rd);
#endif
  }
  else
  {
#ifdef __AVR__
    _rdPort = _dcPort;
    _rdPinMaskSet = 0;
#endif
  }
#ifdef __AVR__
  _rdPinMaskClr = ~_rdPinMaskSet;
#endif

#ifdef __AVR__
  *(portModeRegister(4)) |= 0xFC;
  _dataPort_D = portOutputRegister(4);
  *_dataPort_D |= 0xFC;
  *(portModeRegister(2)) |= 0x03;
  _dataPort_B = portOutputRegister(2);
  *_dataPort_B |= 0x03;
#else
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
#endif

  return true;
}

void Arduino_UNOPAR8::beginWrite()
{
  DC_HIGH();
  CS_LOW();
}

void Arduino_UNOPAR8::endWrite()
{
  CS_HIGH();
}

void Arduino_UNOPAR8::writeCommand(uint8_t c)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();
}

void Arduino_UNOPAR8::writeCommand16(uint16_t c)
{
  DC_LOW();

  _data16.value = c;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);

  DC_HIGH();
}

void Arduino_UNOPAR8::write(uint8_t d)
{
  WRITE(d);
}

void Arduino_UNOPAR8::write16(uint16_t d)
{
  _data16.value = d;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);
}

void Arduino_UNOPAR8::writeRepeat(uint16_t p, uint32_t len)
{
#ifdef __AVR__
  uint8_t wrMaskBase = *_wrPort & _wrPinMaskClr;
  uint8_t wrMaskSet = wrMaskBase | _wrPinMaskSet;
#endif
  _data16.value = p;
  if (_data16.msb == _data16.lsb)
  {
#ifdef __AVR__
    *_dataPort_B = (*_dataPort_B & 0xFC) | (_data16.msb & 0x03);
    *_dataPort_D = (*_dataPort_D & 0x03) | (_data16.msb & 0xFC);
    while (len--)
    {
      *_wrPort = wrMaskBase;
      *_wrPort = wrMaskSet;
      *_wrPort = wrMaskBase;
      *_wrPort = wrMaskSet;
    }
#elif defined(ARDUINO_UNOR4_MINIMA)
    (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
    (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
    (_data16.msb & 0x04) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
    (_data16.msb & 0x08) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
    (_data16.msb & 0x10) ? R_PORT1->POSR = bit(3) : R_PORT1->PORR = bit(3);
    (_data16.msb & 0x20) ? R_PORT1->POSR = bit(2) : R_PORT1->PORR = bit(2);
    (_data16.msb & 0x40) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
    (_data16.msb & 0x80) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
    if (_wr == A1)
    {
      while (len--)
      {
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
      }
    }
    else
    {
      while (len--)
      {
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
      }
    }
#elif defined(ARDUINO_UNOR4_WIFI)
    (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
    (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
    (_data16.msb & 0x04) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
    (_data16.msb & 0x08) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
    (_data16.msb & 0x10) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
    (_data16.msb & 0x20) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
    (_data16.msb & 0x40) ? R_PORT1->POSR = bit(11) : R_PORT1->PORR = bit(11);
    (_data16.msb & 0x80) ? R_PORT1->POSR = bit(12) : R_PORT1->PORR = bit(12);
    if (_wr == A1)
    {
      while (len--)
      {
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
      }
    }
    else
    {
      while (len--)
      {
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
      }
    }
#else
    digitalWrite(8, (_data16.msb & 0x01) ? HIGH : LOW);
    digitalWrite(9, (_data16.msb & 0x02) ? HIGH : LOW);
    digitalWrite(2, (_data16.msb & 0x04) ? HIGH : LOW);
    digitalWrite(3, (_data16.msb & 0x08) ? HIGH : LOW);
    digitalWrite(4, (_data16.msb & 0x10) ? HIGH : LOW);
    digitalWrite(5, (_data16.msb & 0x20) ? HIGH : LOW);
    digitalWrite(6, (_data16.msb & 0x40) ? HIGH : LOW);
    digitalWrite(7, (_data16.msb & 0x80) ? HIGH : LOW);
    while (len--)
    {
      digitalWrite(_wr, LOW);
      digitalWrite(_wr, HIGH);
      digitalWrite(_wr, LOW);
      digitalWrite(_wr, HIGH);
    }
#endif
  }
  else
  {
    while (len--)
    {
#ifdef __AVR__
      *_dataPort_B = (*_dataPort_B & 0xFC) | (_data16.msb & 0x03);
      *_dataPort_D = (*_dataPort_D & 0x03) | (_data16.msb & 0xFC);
      *_wrPort = wrMaskBase;
      *_wrPort = wrMaskSet;

      *_dataPort_B = (*_dataPort_B & 0xFC) | (_data16.lsb & 0x03);
      *_dataPort_D = (*_dataPort_D & 0x03) | (_data16.lsb & 0xFC);
      *_wrPort = wrMaskBase;
      *_wrPort = wrMaskSet;
#elif defined(ARDUINO_UNOR4_MINIMA)
      (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      (_data16.msb & 0x04) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
      (_data16.msb & 0x08) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
      (_data16.msb & 0x10) ? R_PORT1->POSR = bit(3) : R_PORT1->PORR = bit(3);
      (_data16.msb & 0x20) ? R_PORT1->POSR = bit(2) : R_PORT1->PORR = bit(2);
      (_data16.msb & 0x40) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
      (_data16.msb & 0x80) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
      if (_wr == A1)
      {
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
      }
      else
      {
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
      }
      (_data16.lsb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.lsb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      (_data16.lsb & 0x04) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
      (_data16.lsb & 0x08) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
      (_data16.lsb & 0x10) ? R_PORT1->POSR = bit(3) : R_PORT1->PORR = bit(3);
      (_data16.lsb & 0x20) ? R_PORT1->POSR = bit(2) : R_PORT1->PORR = bit(2);
      (_data16.lsb & 0x40) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
      (_data16.lsb & 0x80) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
      if (_wr == A1)
      {
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
      }
      else
      {
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
      }
#elif defined(ARDUINO_UNOR4_WIFI)
      (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      (_data16.msb & 0x04) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
      (_data16.msb & 0x08) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
      (_data16.msb & 0x10) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
      (_data16.msb & 0x20) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
      (_data16.msb & 0x40) ? R_PORT1->POSR = bit(11) : R_PORT1->PORR = bit(11);
      (_data16.msb & 0x80) ? R_PORT1->POSR = bit(12) : R_PORT1->PORR = bit(12);
      if (_wr == A1)
      {
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
      }
      else
      {
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
      }
      (_data16.lsb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.lsb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      (_data16.lsb & 0x04) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
      (_data16.lsb & 0x08) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
      (_data16.lsb & 0x10) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
      (_data16.lsb & 0x20) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
      (_data16.lsb & 0x40) ? R_PORT1->POSR = bit(11) : R_PORT1->PORR = bit(11);
      (_data16.lsb & 0x80) ? R_PORT1->POSR = bit(12) : R_PORT1->PORR = bit(12);
      if (_wr == A1)
      {
        R_PORT0->PORR = bit(0);
        R_PORT0->POSR = bit(0);
      }
      else
      {
        digitalWrite(_wr, LOW);
        digitalWrite(_wr, HIGH);
      }
#else
      digitalWrite(8, (_data16.msb & 0x01) ? HIGH : LOW);
      digitalWrite(9, (_data16.msb & 0x02) ? HIGH : LOW);
      digitalWrite(2, (_data16.msb & 0x04) ? HIGH : LOW);
      digitalWrite(3, (_data16.msb & 0x08) ? HIGH : LOW);
      digitalWrite(4, (_data16.msb & 0x10) ? HIGH : LOW);
      digitalWrite(5, (_data16.msb & 0x20) ? HIGH : LOW);
      digitalWrite(6, (_data16.msb & 0x40) ? HIGH : LOW);
      digitalWrite(7, (_data16.msb & 0x80) ? HIGH : LOW);
      digitalWrite(_wr, LOW);
      digitalWrite(_wr, HIGH);
      digitalWrite(8, (_data16.lsb & 0x01) ? HIGH : LOW);
      digitalWrite(9, (_data16.lsb & 0x02) ? HIGH : LOW);
      digitalWrite(2, (_data16.lsb & 0x04) ? HIGH : LOW);
      digitalWrite(3, (_data16.lsb & 0x08) ? HIGH : LOW);
      digitalWrite(4, (_data16.lsb & 0x10) ? HIGH : LOW);
      digitalWrite(5, (_data16.lsb & 0x20) ? HIGH : LOW);
      digitalWrite(6, (_data16.lsb & 0x40) ? HIGH : LOW);
      digitalWrite(7, (_data16.lsb & 0x80) ? HIGH : LOW);
      digitalWrite(_wr, LOW);
      digitalWrite(_wr, HIGH);
#endif
    }
  }
}

void Arduino_UNOPAR8::writePixels(uint16_t *data, uint32_t len)
{
  while (len--)
  {
    _data16.value = *data++;
    WRITE(_data16.msb);
    WRITE(_data16.lsb);
  }
}

void Arduino_UNOPAR8::writeC8D8(uint8_t c, uint8_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  WRITE(d);
}

void Arduino_UNOPAR8::writeC8D16(uint8_t c, uint16_t d)
{
  DC_LOW();

  WRITE(c);

  DC_HIGH();

  _data16.value = d;
  WRITE(_data16.msb);
  WRITE(_data16.lsb);
}

void Arduino_UNOPAR8::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
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

#if !defined(LITTLE_FOOT_PRINT)
void Arduino_UNOPAR8::writeBytes(uint8_t *data, uint32_t len)
{
  while (len > 1)
  {
    WRITE(*data++);
    len -= 1;
  }
}
#endif

INLINE void Arduino_UNOPAR8::WRITE(uint8_t d)
{
#ifdef __AVR__
  uint8_t wrMaskBase = *_wrPort & _wrPinMaskClr;
  *_dataPort_B = (*_dataPort_B & 0xFC) | (d & 0x03);
  *_dataPort_D = (*_dataPort_D & 0x03) | (d & 0xFC);
  *_wrPort = wrMaskBase;
  *_wrPort = wrMaskBase | _wrPinMaskSet;
#elif defined(ARDUINO_UNOR4_MINIMA)
  (d & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
  (d & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
  (d & 0x04) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
  (d & 0x08) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
  (d & 0x10) ? R_PORT1->POSR = bit(3) : R_PORT1->PORR = bit(3);
  (d & 0x20) ? R_PORT1->POSR = bit(2) : R_PORT1->PORR = bit(2);
  (d & 0x40) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
  (d & 0x80) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
  if (_wr == A1)
  {
    R_PORT0->PORR = bit(0);
    R_PORT0->POSR = bit(0);
  }
  else
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
  }
#elif defined(ARDUINO_UNOR4_WIFI)
  (d & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
  (d & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
  (d & 0x04) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
  (d & 0x08) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
  (d & 0x10) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
  (d & 0x20) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
  (d & 0x40) ? R_PORT1->POSR = bit(11) : R_PORT1->PORR = bit(11);
  (d & 0x80) ? R_PORT1->POSR = bit(12) : R_PORT1->PORR = bit(12);
  if (_wr == A1)
  {
    R_PORT0->PORR = bit(0);
    R_PORT0->POSR = bit(0);
  }
  else
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
  }
#else
  digitalWrite(8, (d & 0x01) ? HIGH : LOW);
  digitalWrite(9, (d & 0x02) ? HIGH : LOW);
  digitalWrite(2, (d & 0x04) ? HIGH : LOW);
  digitalWrite(3, (d & 0x08) ? HIGH : LOW);
  digitalWrite(4, (d & 0x10) ? HIGH : LOW);
  digitalWrite(5, (d & 0x20) ? HIGH : LOW);
  digitalWrite(6, (d & 0x40) ? HIGH : LOW);
  digitalWrite(7, (d & 0x80) ? HIGH : LOW);
  digitalWrite(_wr, LOW);
  digitalWrite(_wr, HIGH);
#endif
}

/******** low level bit twiddling **********/

INLINE void Arduino_UNOPAR8::DC_HIGH(void)
{
#ifdef __AVR__
  *_dcPort |= _dcPinMaskSet;
#else
  digitalWrite(_dc, HIGH);
#endif
}

INLINE void Arduino_UNOPAR8::DC_LOW(void)
{
#ifdef __AVR__
  *_dcPort &= _dcPinMaskClr;
#else
  digitalWrite(_dc, LOW);
#endif
}

INLINE void Arduino_UNOPAR8::CS_HIGH(void)
{
  if (_cs != GFX_NOT_DEFINED)
  {
#ifdef __AVR__
    *_csPort |= _csPinMaskSet;
#else
    digitalWrite(_cs, HIGH);
#endif
  }
}

INLINE void Arduino_UNOPAR8::CS_LOW(void)
{
  if (_cs != GFX_NOT_DEFINED)
  {
#ifdef __AVR__
    *_csPort &= _csPinMaskClr;
#else
    digitalWrite(_cs, LOW);
#endif
  }
}
