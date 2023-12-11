// Arduino GFX display driver for SSD1306

#include "Arduino_DataBus.h"
#include "Arduino_SSD1306.h"

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// SSD1306 commands, See Datasheet
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_SETCOLUMNADDR 0x21
#define SSD1306_SETPAGEADDR 0x22
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_SEGREMAPINV 0xA1
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETPAGE 0xB0
#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3


void Arduino_SSD1306::_commandList(uint8_t *commands, uint8_t len)
{
  while (len--)
  {
    _bus->sendCommand(*commands++);
  }
}

Arduino_SSD1306::Arduino_SSD1306(Arduino_DataBus *bus, int8_t rst, int16_t w, int16_t h)
    : Arduino_G(w, h), _bus(bus), _rst(rst) {}

bool Arduino_SSD1306::begin(int32_t speed)
{
  Serial.println("SSD1306::begin()");

  if (!_bus)
  {
    // println("SSD1306::bus not given");
  }
  else if (!_bus->begin(speed))
  {
    // println("SSD1306::bus not started");
    return false;
  }

  // set options in case of TWI / Wire bus in use
  _bus->setOption(Arduino_DataBus::TWI_COMMAND_PREFIX, SSD1306_COMMAND);
  _bus->setOption(Arduino_DataBus::TWI_DATA_PREFIX, SSD1306_DATA);

  // println("SSD1306::Initialize Display");

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
  uint8_t comPins = 0x12;

  if ((WIDTH == 128) && (HEIGHT == 32))
  {
    comPins = 0x02;
    _contrast = 0x8F;
  }
  else if ((WIDTH == 128) && (HEIGHT == 64))
  {
    comPins = 0x12;
    _contrast = 0x9F;
  }
  else if ((WIDTH == 72) && (HEIGHT == 40))
  {
    comPins = 0x12;
    _contrast = 0x2F; // 0x82;
  }
  else if ((WIDTH == 96) && (HEIGHT == 16))
  {
    comPins = 0x2;
    _contrast = 0x10;
  }
  else
  {
    // Other screen varieties -- TBD
  }

  uint8_t init_sequence[] = {
      SSD1306_DISPLAYOFF,                          // 0xAE
      SSD1306_SETCONTRAST, _contrast,              // 0x81 nn
      SSD1306_NORMALDISPLAY,                       // 0xA6
      SSD1306_DEACTIVATE_SCROLL,                   // 0x2E
      SSD1306_MEMORYMODE, 0x00,                    // 0x20 00 Horizontal addressing mode
      SSD1306_SEGREMAPINV,                         // 0xA1
      SSD1306_SETMULTIPLEX, (uint8_t)(HEIGHT - 1), // 0xA8 nn
      SSD1306_COMSCANDEC,                          // 0xC8
      SSD1306_SETDISPLAYOFFSET, 0x00,              // 0xD3 00   no offset
      SSD1306_SETCOMPINS, comPins,                 // 0xDA nn
      SSD1306_SETDISPLAYCLOCKDIV, 0x80,            // 0xD5 0x80
      SSD1306_SETPRECHARGE, 0x22,                  // 0xd9 0x22
      SSD1306_SETVCOMDETECT, 0x40,                 // 0xDB 0x40
      SSD1306_CHARGEPUMP, 0x14,                    // 0x8D 0x14
      SSD1306_SETSTARTLINE | 0x0,                  // 0x40       line #0
      SSD1306_DISPLAYALLON_RESUME                  // 0xA4
  };
  _commandList(init_sequence, sizeof(init_sequence));

  delay(100);
  _bus->sendCommand(SSD1306_DISPLAYON); // 0xAF

  return true;
}

void Arduino_SSD1306::setBrightness(uint8_t brightness){
    // _sendCommand(SSD1306_SETCONTRAST);
    // ??? _sendCommand((brightness < 50) ? 0 : _contrast); ???
    // _sendCommand((brightness < 50) ? 0 : 0x8f);
}; // setBrightness

void Arduino_SSD1306::drawBitmap(int16_t xStart, int16_t yStart, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  // printf("SSD1306::drawBitmap %d/%d w:%d h:%d\n", xStart, yStart, w, h);
  uint16_t bufferLength = TWI_BUFFER_LENGTH;

#if 0
  // print bitmap on Serial log
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      uint8_t b = bitmap[(y / 8) * w + x];
      if (b & (1 << (y % 8)))
      {
        Serial.print('#');
      }
      else
      {
        Serial.print('.');
      }
    }
    Serial.println();
  }
  Serial.println();
#endif

#if 1

#if defined(ESP8266)
  yield();
#endif

  // set page / column range in use for writing
  // output is written in pages with 8 bit height

  uint8_t draw_startup_sequence[] = {
      SSD1306_SETPAGEADDR,
      yStart / 8,
      (yStart / 8) + ((h + 7) / 8) - 1,

      SSD1306_SETCOLUMNADDR,
      xStart,
      xStart + w - 1};
  _commandList(draw_startup_sequence, sizeof(draw_startup_sequence));

  _bus->beginWrite();
  _bus->writePixels(bitmap, w * ((h + 7) / 8));
  _bus->endWrite();
#endif
} // drawBitmap()

void Arduino_SSD1306::drawIndexedBitmap(int16_t, int16_t, uint8_t *, uint16_t *, int16_t, int16_t, int16_t)
{
  // println("SSD1306::Not Implemented drawIndexedBitmap()");
}

void Arduino_SSD1306::draw3bitRGBBitmap(int16_t, int16_t, uint8_t *bitmap, int16_t w, int16_t h)
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

void Arduino_SSD1306::invertDisplay(bool i)
{
  // _bus->sendCommand(i ? ILI9488_INVON : ILI9488_INVOFF);
}

void Arduino_SSD1306::displayOn(void)
{
  _bus->sendCommand(SSD1306_DISPLAYON); // 0xAF
}

void Arduino_SSD1306::displayOff(void)
{
  _bus->sendCommand(SSD1306_DISPLAYOFF); // 0xAE
}
