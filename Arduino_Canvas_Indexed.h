/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _Arduino_CANVAS_INDEXED_H_
#define _Arduino_CANVAS_INDEXED_H_

#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT.h"

#define COLOR_IDX_SIZE 255

class Arduino_Canvas_Indexed : public Arduino_GFX
{
public:
  Arduino_Canvas_Indexed(int16_t w, int16_t h, Arduino_TFT *output, uint16_t color_mask = 0xFFFF);

  virtual void begin(uint32_t speed = 0);
  virtual void writePixelPreclipped(int16_t x, int16_t y, uint16_t color);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void flush(void);

  virtual uint8_t get_color_index(uint16_t color);
  virtual uint16_t get_index_color(uint8_t idx);

protected:
  uint8_t *_framebuffer;
  Arduino_TFT *_output;
  uint16_t _color_mask;
  uint16_t _color_index[COLOR_IDX_SIZE];
  uint8_t _indexed_size = 0;

private:
};

#endif
