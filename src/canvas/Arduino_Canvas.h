/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _Arduino_CANVAS_H_
#define _Arduino_CANVAS_H_

#include "../Arduino_GFX.h"

class Arduino_Canvas : public Arduino_GFX
{
public:
  Arduino_Canvas(int16_t w, int16_t h, Arduino_G *output, int16_t output_x = 0, int16_t output_y = 0);

  virtual void begin(int32_t speed = 0);
  virtual void writePixelPreclipped(int16_t x, int16_t y, uint16_t color);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void flush(void);

protected:
  uint16_t *_framebuffer;
  Arduino_G *_output;
  int16_t _output_x, _output_y;

private:
};

#endif
