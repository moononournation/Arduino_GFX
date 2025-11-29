#include "Arduino_SSD1306.h"

Arduino_SSD1306::Arduino_SSD1306(Arduino_DataBus *bus, int8_t rst, int16_t w, int16_t h)
    : Arduino_G(w, h), _bus(bus), _rst(rst)
{
  _rotation = 0;
  _pages = (h + 7) / 8;
}

bool Arduino_SSD1306::begin(int32_t speed)
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

  // display parameter default values
  uint8_t comPins;
  if ((WIDTH == 128) && (HEIGHT == 64))
  {
    comPins = 0x12;
    _contrast = 0x9F;
    _colStart = 0;
    _colEnd = 128 - 1;
  }
  else if ((WIDTH == 128) && (HEIGHT == 32))
  {
    comPins = 0x02;
    _contrast = 0x8F;
    _colStart = 0;
    _colEnd = 128 - 1;
  }
  else if ((WIDTH == 96) && (HEIGHT == 16))
  {
    comPins = 0x02;
    _contrast = 0x10;
    _colStart = 0;
    _colEnd = 96 - 1;
  }
  else if ((WIDTH == 72) && (HEIGHT == 40))
  {
    comPins = 0x12;
    _contrast = 0x82;
    _colStart = 28;
    _colEnd = 28 + 72 - 1;
  }
  else if ((WIDTH == 64) && (HEIGHT == 48))
  {
    comPins = 0x12;
    _contrast = 0x82;
    _colStart = 32;
    _colEnd = 32 + 64 - 1;
  }
  else if ((WIDTH == 64) && (HEIGHT == 32))
  {
    comPins = 0x12;
    _contrast = 0x82;
    _colStart = 32;
    _colEnd = 32 + 64 - 1;
  }
  else
  {
    // Other screen varieties -- TBD
    comPins = 0x12;
    _colStart = 0;
    _colEnd = WIDTH - 1;
  }

  static const uint8_t init_sequence[] = {
      BEGIN_WRITE,
      WRITE_COMMAND_BYTES, 25,
      SSD1306_DISPLAYOFF,                          // 0xAE
      SSD1306_SETCONTRAST, _contrast,              // 0x81, contrast
      SSD1306_NORMALDISPLAY,                       // 0xA6
      SSD1306_DEACTIVATE_SCROLL,                   // 0x2E
      SSD1306_MEMORYMODE, 0x00,                    // 0x20, 00 Horizontal addressing mode
      SSD1306_SEGREMAPINV,                         // 0xA1
      SSD1306_SETMULTIPLEX, (uint8_t)(HEIGHT - 1), // 0xA8, nn
      SSD1306_COMSCANDEC,                          // 0xC8
      SSD1306_SETDISPLAYOFFSET, 0x00,              // 0xD3, 00   no offset
      SSD1306_SETCOMPINS, comPins,                 // 0xDA, nn
      SSD1306_SETDISPLAYCLOCKDIV, 0x80,            // 0xD5, 0x80
      SSD1306_SETPRECHARGE, 0x22,                  // 0xd9, 0x22
      SSD1306_SETVCOMDETECT, 0x40,                 // 0xDB, 0x40
      SSD1306_CHARGEPUMP, 0x14,                    // 0x8D, 0x14
      SSD1306_SETSTARTLINE | 0x0,                  // 0x40       line #0
      SSD1306_DISPLAYALLON_RESUME,                 // 0xA4
      END_WRITE,

      DELAY, 100,

      BEGIN_WRITE,
      WRITE_COMMAND_8, SSD1306_DISPLAYON, // 0xAF
      END_WRITE};

  _bus->batchOperation(init_sequence, sizeof(init_sequence));

  return true;
}

void Arduino_SSD1306::setBrightness(uint8_t brightness)
{
  _bus->beginWrite();
  _bus->writeC8D8(SSD1306_SETCONTRAST, brightness);
  _bus->endWrite();
} // setBrightness()

void Arduino_SSD1306::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t /* color */, uint16_t /* bg */)
{
  // printf("SSD1306::drawBitmap %d/%d w:%d h:%d\n", x, y, w, h);
  uint16_t count = w * ((h + 7) / 8);

  // start page sequence
  _bus->beginWrite();
  uint8_t page_sequence[] = {
      SSD1306_PAGEADDR, 0, 0xFF,
      SSD1306_COLUMNADDR, _colStart, _colEnd};
  _bus->writeCommandBytes(page_sequence, sizeof(page_sequence));
  _bus->endWrite();

  _bus->beginWrite();
  _bus->writeBytes(bitmap, count);
  _bus->endWrite();
} // drawBitmap()

void Arduino_SSD1306::drawIndexedBitmap(int16_t, int16_t, uint8_t *, uint16_t *, int16_t, int16_t, int16_t)
{
  // println("SSD1306::Not Implemented drawIndexedBitmap()");
}

void Arduino_SSD1306::draw3bitRGBBitmap(int16_t, int16_t, uint8_t *, int16_t, int16_t)
{
  // println("SSD1306::Not Implemented draw3bitRGBBitmap()");
}

void Arduino_SSD1306::draw16bitRGBBitmap(int16_t, int16_t, uint16_t *, int16_t, int16_t)
{
  // println("SSD1306::Not Implemented draw16bitRGBBitmap()");
}

void Arduino_SSD1306::draw24bitRGBBitmap(int16_t, int16_t, uint8_t *, int16_t, int16_t)
{
  // println("SSD1306::Not Implemented draw24bitRGBBitmap()");
}

void Arduino_SSD1306::invertDisplay(bool)
{
  // println("SSD1306::Not Implemented invertDisplay()");
}

void Arduino_SSD1306::displayOn(void)
{
  _bus->sendCommand(SSD1306_DISPLAYON);
}

void Arduino_SSD1306::displayOff(void)
{
  _bus->sendCommand(SSD1306_DISPLAYOFF);
}
