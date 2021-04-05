/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _Arduino_CANVAS_INDEXED_H_
#define _Arduino_CANVAS_INDEXED_H_

#include "../Arduino_GFX.h"
#include <unordered_map>

#define COLOR_IDX_SIZE 256

class Arduino_Canvas_Indexed : public Arduino_GFX
{
public:
  Arduino_Canvas_Indexed(int16_t w, int16_t h, Arduino_G *output, int16_t output_x = 0, int16_t output_y = 0, uint8_t mask_level = 0);

  virtual void begin(int32_t speed = 0);
  virtual void writePixelPreclipped(int16_t x, int16_t y, uint16_t color);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void flush(void);

  uint8_t get_color_index(uint16_t color);
  uint16_t get_index_color(uint8_t idx);
  void raise_mask_level();

protected:
  uint8_t *_framebuffer;
  Arduino_G *_output;
  int16_t _output_x, _output_y;
  uint16_t _color_index[COLOR_IDX_SIZE];
  std::unordered_map<uint16_t, uint8_t> _color_to_index_map(COLOR_IDX_SIZE);
  uint8_t _indexed_size = 0;
  uint8_t _current_mask_level;
  uint16_t _color_mask;
#define MAXMASKLEVEL 3
  uint16_t mask_level_list[MAXMASKLEVEL] = {
      0b1111111111111111, // 16-bit, 65536 colors
      0b1111011110011110, // 12-bit, 4096 colors
      0b1100011000011000  // 7-bit, 128 colors
  };

private:
};

#endif
