#pragma once

#include "../Arduino_GFX.h"
#include "../databus/Arduino_Wire.h"

/** SH1106 commands, See Datasheet:
 * https://cdn.velleman.eu/downloads/29/infosheets/sh1106_datasheet.pdf
 **/
#define SH110X_SETLOWCOLUMN 0x00
#define SH110X_SETHIGHCOLUMN 0x10

#define SH110X_MEMORYMODE 0x20
#define SH110X_COLUMNADDR 0x21
#define SH110X_PAGEADDR 0x22

#define SH110X_SETPUMPV 0x30

#define SH110X_SETSTARTLINE 0x40

#define SH110X_SETCONTRAST 0x81
#define SH110X_CHARGEPUMP 0x8D

#define SH110X_SEGREMAP 0xA0

#define SH110X_DISPLAYALLON_RESUME 0xA4
#define SH110X_DISPLAYALLON 0xA5

#define SH110X_NORMALDISPLAY 0xA6
#define SH110X_INVERTDISPLAY 0xA7

#define SH110X_SETMULTIPLEX 0xA8

#define SH110X_SETDCDCMODE 0xAD
#define SH110X_DISPLAYOFF 0xAE
#define SH110X_DISPLAYON 0xAF

#define SH110X_SETPAGEADDR 0xB0

#define SH110X_COMSCANINC 0xC0
#define SH110X_COMSCANDEC 0xC8

#define SH110X_SETDISPLAYOFFSET 0xD3
#define SH110X_SETDISPLAYCLOCKDIV 0xD5
#define SH110X_SETPRECHARGE 0xD9
#define SH110X_SETCOMPINS 0xDA
#define SH110X_SETVCOMDETECT 0xDB
#define SH110X_SETDISPSTARTLINE 0xDC

#define SH110X_READMODIFYWRITE 0xE0
#define SH110X_READMODIFYWRITEEND 0xEE

#define SH110X_NOP 0xE3

class Arduino_SH1106 : public Arduino_G
{
public:
  Arduino_SH1106(Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, int16_t w = 128, int16_t h = 64);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) override;
  void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h, int16_t x_skip = 0) override;
  void draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) override;
  void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) override;
  void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) override;

  void invertDisplay(bool);
  void displayOn();
  void displayOff();
  void setBrightness(uint8_t brightness);

protected:
  Arduino_DataBus *_bus;
  int8_t _rst;
  int8_t _pages;
  uint8_t _rotation;

  uint8_t _contrast = 0x8F;

private:
};
