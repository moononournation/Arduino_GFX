#include "Arduino_SH1106.h"

Arduino_SH1106::Arduino_SH1106(Arduino_DataBus *bus, int8_t rst, int16_t w, int16_t h)
    : Arduino_G(w, h), _bus(bus), _rst(rst)
{
  _rotation = 0;
  _pages = (h + 7) / 8;
}

bool Arduino_SH1106::begin(int32_t speed)
{
  if (speed != GFX_SKIP_DATABUS_BEGIN)
  {
    if (!_bus->begin(speed))
    {
      return false;
    }
  }

  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(20);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(20);
  }

  static const uint8_t init_sequence[] = {
      BEGIN_WRITE,
      WRITE_COMMAND_BYTES, 25,
      SH110X_DISPLAYOFF,               // 0xAE
      SH110X_SETDISPLAYCLOCKDIV, 0x80, // 0xD5, 0x80
      SH110X_SETMULTIPLEX, 0x3F,       // 0xA8, 0x3F
      SH110X_SETDISPLAYOFFSET, 0x00,   // 0xD3, 0x00
      SH110X_SETSTARTLINE,             // 0x40
      SH110X_SETDCDCMODE, 0x8B,        // 0xAD, DC/DC on
      SH110X_SEGREMAP + 1,             // 0xA1
      SH110X_COMSCANDEC,               // 0xC8
      SH110X_SETCOMPINS, 0x12,         // 0xDA, 0x12
      SH110X_SETCONTRAST, _contrast,   // 0x81, contrast
      SH110X_SETPRECHARGE, 0x1F,       // 0xD9, 0x1F
      SH110X_SETVCOMDETECT, 0x40,      // 0xDB, 0x40
      0x33,                            // Set VPP to 9V
      SH110X_NORMALDISPLAY,            // 0xA6
      SH110X_MEMORYMODE, 0x10,         // 0x20, 0x10
      SH110X_DISPLAYALLON_RESUME,      // 0xA4
      END_WRITE,

      DELAY, 100,

      BEGIN_WRITE,
      WRITE_COMMAND_8, SH110X_DISPLAYON, // 0xAF
      END_WRITE};

  _bus->batchOperation(init_sequence, sizeof(init_sequence));

  return true;
}

void Arduino_SH1106::setBrightness(uint8_t brightness)
{
  _bus->beginWrite();
  _bus->writeC8D8(SH110X_SETCONTRAST, brightness);
  _bus->endWrite();
} // setBrightness()

void Arduino_SH1106::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t /* color */, uint16_t /* bg */)
{
  // printf("SH1106::drawBitmap %d/%d w:%d h:%d\n", x, y, w, h);

  // transfer the whole bitmap
  for (uint8_t p = (y / 8); p < (h / 8); p++)
  {
    uint8_t *pptr = bitmap + (p * w); // page start pointer

    // start page sequence
    _bus->beginWrite();
    // _bus->writeCommand(SH110X_SETPAGEADDR + p);
    // _bus->write(); // set column
    // _bus->write(SH110X_SETHIGHCOLUMN + 0);

    uint8_t page_sequence[] = {
        (uint8_t)(SH110X_SETPAGEADDR + p),
        SH110X_SETLOWCOLUMN + 2,
        SH110X_SETHIGHCOLUMN + 0};
    _bus->writeCommandBytes(page_sequence, sizeof(page_sequence));
    _bus->endWrite();

    _bus->beginWrite();
    _bus->writeBytes(pptr, w);
    _bus->endWrite();
  }
} // drawBitmap()

void Arduino_SH1106::drawIndexedBitmap(int16_t, int16_t, uint8_t *, uint16_t *, int16_t, int16_t, int16_t)
{
  // println("SH1106::Not Implemented drawIndexedBitmap()");
}

void Arduino_SH1106::draw3bitRGBBitmap(int16_t, int16_t, uint8_t *, int16_t, int16_t)
{
  // println("SH1106::Not Implemented draw3bitRGBBitmap()");
}

void Arduino_SH1106::draw16bitRGBBitmap(int16_t, int16_t, uint16_t *, int16_t, int16_t)
{
  // println("SH1106::Not Implemented draw16bitRGBBitmap()");
}

void Arduino_SH1106::draw24bitRGBBitmap(int16_t, int16_t, uint8_t *, int16_t, int16_t)
{
  // println("SH1106::Not Implemented draw24bitRGBBitmap()");
}

void Arduino_SH1106::invertDisplay(bool)
{
  // println("SH1106::Not Implemented invertDisplay()");
}

void Arduino_SH1106::displayOn(void)
{
  _bus->sendCommand(SH110X_DISPLAYON);
}

void Arduino_SH1106::displayOff(void)
{
  _bus->sendCommand(SH110X_DISPLAYOFF);
}
