#include "Arduino_UNOPAR8.h"

// for MCUFriend UNO kind of shields. -jz-
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_UNOR4_WIFI)

Arduino_UNOPAR8::Arduino_UNOPAR8()
{
}

#ifdef ARDUINO_AVR_UNO
#define BMASK         0x03
#define DMASK         0xFC
#endif

bool Arduino_UNOPAR8::begin(int32_t speed, int8_t dataMode)
{
  UNUSED(speed);
  UNUSED(dataMode);
  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, HIGH); // Data mode
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH); // Disable chip select
  pinMode(_wr, OUTPUT);
  digitalWrite(_wr, HIGH); // Set write strobe high (inactive)
  pinMode(_rd, OUTPUT);
  digitalWrite(_rd, HIGH); // Disable RD
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  return true;
}

#if defined(ARDUINO_UNOR4_WIFI)
const int16_t Arduino_UNOPAR8::_p1_map[] =
{ // 0x0     0x1     0x2     0x3     0x4     0x5     0x6     0x7
  0x0000, 0x0010, 0x0020, 0x0030, 0x0040, 0x0050, 0x0060, 0x0070,
  0x0080, 0x0090, 0x00A0, 0x00B0, 0x00C0, 0x00D0, 0x00E0, 0x00F0,
  0x0800, 0x0810, 0x0820, 0x0830, 0x0840, 0x0850, 0x0860, 0x0870,
  0x0880, 0x0890, 0x08A0, 0x08B0, 0x08C0, 0x08D0, 0x08E0, 0x08F0,
  0x1000, 0x1010, 0x1020, 0x1030, 0x1040, 0x1050, 0x1060, 0x1070,
  0x1080, 0x1090, 0x10A0, 0x10B0, 0x10C0, 0x10D0, 0x10E0, 0x10F0,
  0x1800, 0x1810, 0x1820, 0x1830, 0x1840, 0x1850, 0x1860, 0x1870,
  0x1880, 0x1890, 0x18A0, 0x18B0, 0x18C0, 0x18D0, 0x18E0, 0x18F0,
};
#elif defined(ARDUINO_UNOR4_MINIMA)
const int16_t Arduino_UNOPAR8::_p1_map[] =
{ // 0x0     0x1     0x2     0x3     0x4     0x5     0x6     0x7
  0x0000, 0x0020, 0x0010, 0x0030, 0x0008, 0x0028, 0x0018, 0x0038,
  0x0004, 0x0024, 0x0014, 0x0034, 0x000C, 0x002C, 0x001C, 0x003C,
  0x0040, 0x0060, 0x0050, 0x0070, 0x0048, 0x0068, 0x0058, 0x0078,
  0x0044, 0x0064, 0x0054, 0x0074, 0x004C, 0x006C, 0x005C, 0x007C,
  0x0080, 0x00A0, 0x0090, 0x00B0, 0x0088, 0x00A8, 0x0098, 0x00B8,
  0x0084, 0x00A4, 0x0094, 0x00B4, 0x008C, 0x00AC, 0x009C, 0x00BC,
  0x00C0, 0x00E0, 0x00D0, 0x00F0, 0x00C8, 0x00E8, 0x00D8, 0x00F8,
  0x00C4, 0x00E4, 0x00D4, 0x00F4, 0x00CC, 0x00EC, 0x00DC, 0x00FC,
};
#endif

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
  volatile int wr_stretch = 0;
  _data16.value = p;
  if (_data16.msb == _data16.lsb)
  {
#ifdef ARDUINO_AVR_UNO
    PORTB = (PORTB & ~BMASK) | ((_data16.msb) & BMASK);
    PORTD = (PORTD & ~DMASK) | ((_data16.msb) & DMASK);
    while (len--)
    {
      PORTC &= ~_BV(1); PORTC |= _BV(1); // WR_STB;
      PORTC &= ~_BV(1); PORTC |= _BV(1); // WR_STB;
    }
#elif defined(ARDUINO_UNOR4_MINIMA)
    (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
    (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
    R_PORT1->PORR = 0x00FC;
    R_PORT1->POSR = _p1_map[_data16.msb / 4];
    while (len--)
    {
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
    }
#elif defined(ARDUINO_UNOR4_WIFI)
    (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
    (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
    R_PORT1->PORR = 0x18F0;
    R_PORT1->POSR = ((_data16.msb & 0x3C) << 2) | ((_data16.msb & 0xC0) << 5);
    while (len--)
    {
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
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
#ifdef ARDUINO_AVR_UNO
      PORTB = (PORTB & ~BMASK) | ((_data16.msb) & BMASK);
      PORTD = (PORTD & ~DMASK) | ((_data16.msb) & DMASK);
      PORTC &= ~_BV(1); PORTC |= _BV(1); // WR_STB;

      PORTB = (PORTB & ~BMASK) | ((_data16.lsb) & BMASK);
      PORTD = (PORTD & ~DMASK) | ((_data16.lsb) & DMASK);
      PORTC &= ~_BV(1); PORTC |= _BV(1); // WR_STB;
#elif defined(ARDUINO_UNOR4_MINIMA)
      (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      R_PORT1->PORR = 0x00FC;
      R_PORT1->POSR = _p1_map[_data16.msb / 4];
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
      (_data16.lsb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.lsb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      R_PORT1->PORR = 0x00FC;
      R_PORT1->POSR = _p1_map[_data16.lsb / 4];
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
#elif defined(ARDUINO_UNOR4_WIFI)
      (_data16.msb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.msb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      R_PORT1->PORR = 0x18F0;
      R_PORT1->POSR = ((_data16.msb & 0x3C) << 2) | ((_data16.msb & 0xC0) << 5);
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
      (_data16.lsb & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
      (_data16.lsb & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
      R_PORT1->PORR = 0x18F0;
      R_PORT1->POSR = ((_data16.lsb & 0x3C) << 2) | ((_data16.lsb & 0xC0) << 5);
      R_PORT0->PORR = bit(0); wr_stretch++;
      R_PORT0->POSR = bit(0);
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
  volatile int wr_stretch = 0;
#ifdef ARDUINO_AVR_UNO
  PORTB = (PORTB & ~BMASK) | ((d) & BMASK);
  PORTD = (PORTD & ~DMASK) | ((d) & DMASK);
  PORTC &= ~_BV(1); PORTC |= _BV(1); // WR_STB;
#elif defined(ARDUINO_UNOR4_MINIMA)
  (d & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
  (d & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
  //  (d & 0x04) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
  //  (d & 0x08) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
  //  (d & 0x10) ? R_PORT1->POSR = bit(3) : R_PORT1->PORR = bit(3);
  //  (d & 0x20) ? R_PORT1->POSR = bit(2) : R_PORT1->PORR = bit(2);
  //  (d & 0x40) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
  //  (d & 0x80) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
  R_PORT1->PORR = 0x00FC;
  R_PORT1->POSR = _p1_map[d / 4];
  R_PORT0->PORR = bit(0); wr_stretch++;
  R_PORT0->POSR = bit(0);
#elif defined(ARDUINO_UNOR4_WIFI)
  (d & 0x01) ? R_PORT3->POSR = bit(4) : R_PORT3->PORR = bit(4);
  (d & 0x02) ? R_PORT3->POSR = bit(3) : R_PORT3->PORR = bit(3);
  //  (d & 0x04) ? R_PORT1->POSR = bit(4) : R_PORT1->PORR = bit(4);
  //  (d & 0x08) ? R_PORT1->POSR = bit(5) : R_PORT1->PORR = bit(5);
  //  (d & 0x10) ? R_PORT1->POSR = bit(6) : R_PORT1->PORR = bit(6);
  //  (d & 0x20) ? R_PORT1->POSR = bit(7) : R_PORT1->PORR = bit(7);
  //  (d & 0x40) ? R_PORT1->POSR = bit(11) : R_PORT1->PORR = bit(11);
  //  (d & 0x80) ? R_PORT1->POSR = bit(12) : R_PORT1->PORR = bit(12);
  R_PORT1->PORR = 0x18F0;
  //R_PORT1->POSR = _p1_map[d / 4];
  R_PORT1->POSR = ((d & 0x3C) << 2) | ((d & 0xC0) << 5); // slightly faster
  R_PORT0->PORR = bit(0); wr_stretch++;
  R_PORT0->POSR = bit(0);
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
#ifdef ARDUINO_AVR_UNO
  PORTC |= _BV(2); // RS_H;
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  R_PORT0->POSR = bit(1);
#else
  digitalWrite(_dc, HIGH);
#endif
}

INLINE void Arduino_UNOPAR8::DC_LOW(void)
{
#ifdef ARDUINO_AVR_UNO
  PORTC &= ~_BV(2); // RS_L;
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  R_PORT0->PORR = bit(1);
#else
  digitalWrite(_dc, LOW);
#endif
}

INLINE void Arduino_UNOPAR8::CS_HIGH(void)
{
#ifdef ARDUINO_AVR_UNO
  PORTC |= _BV(3); // CS_H;
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  R_PORT0->POSR = bit(2);
#else
  digitalWrite(_cs, HIGH);
#endif
}

INLINE void Arduino_UNOPAR8::CS_LOW(void)
{
#ifdef ARDUINO_AVR_UNO
  PORTC &= ~_BV(3); // CS_L;
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  R_PORT0->PORR = bit(2);
#else
  digitalWrite(_cs, LOW);
#endif
}

#endif // #if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_UNOR4_WIFI)
