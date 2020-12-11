/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_G_H_
#define _ARDUINO_G_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include "WProgram.h"
#endif

/// A generic graphics superclass that can handle all sorts of drawing. At a minimum you can subclass and provide drawPixel(). At a maximum you can do a ton of overriding to optimize. Used for any/all Adafruit displays!
class Arduino_G
{
public:
  Arduino_G(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass:
  virtual void begin(int32_t speed = 0) = 0;

  virtual void fillScreen(uint16_t color) = 0;
  virtual void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) = 0;
  virtual void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h) = 0;
  virtual void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) = 0;
  virtual void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) = 0;

protected:
  int16_t
      WIDTH,  ///< This is the 'raw' display width - never changes
      HEIGHT; ///< This is the 'raw' display height - never changes
};

#endif // _Arduino_G_H
