/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_TFT_18BIT_H_
#define _ARDUINO_TFT_18BIT_H_

#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT.h"

class Arduino_TFT_18bit : public Arduino_TFT
{
public:
  Arduino_TFT_18bit(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips, int16_t w, int16_t h, uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2);

  virtual void writeColor(uint16_t color);
  virtual void writeRepeat(uint16_t color, uint32_t len);
  virtual void writePixels(uint16_t *data, uint32_t len);
  virtual void writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;

  virtual void invertDisplay(bool) = 0;
  virtual void displayOn() = 0;
  virtual void displayOff() = 0;

// TFT optimization code, too big for ATMEL family
#if defined(ARDUINO_ARCH_SAMD) || defined(ESP8266) || defined(ESP32)
  virtual void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
  virtual void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
  virtual void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  virtual void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  virtual void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h);
  virtual void draw16bitRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  virtual void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  virtual void draw16bitBeRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  virtual void draw24bitRGBBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  virtual void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
#endif // defined(ARDUINO_ARCH_SAMD) || defined(ESP8266) || defined(ESP32)

protected:

private:
};

#endif
