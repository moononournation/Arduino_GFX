// Arduino GFX display driver for SSD1306

#ifndef _Arduino_SSD1306_H_
#define _Arduino_SSD1306_H_

#include <Arduino.h>
#include <Arduino_GFX.h>

class Arduino_SSD1306 : public Arduino_G
{
public:
  Arduino_SSD1306(Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, int16_t w = 128, int16_t h = 64);

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
  uint8_t _colStart;
  uint8_t _colEnd;

private:
};

#endif // _Arduino_SSD1306_H_
