#include "../Arduino_DataBus.h"
#if !defined(LITTLE_FOOT_PRINT)

#ifndef _ARDUINO_CANVAS_MONO_H_
#define _ARDUINO_CANVAS_MONO_H_

#include "../Arduino_GFX.h"

class Arduino_Canvas_Mono : public Arduino_GFX
{
public:
  Arduino_Canvas_Mono(int16_t w, int16_t h, Arduino_G *output, int16_t output_x = 0, int16_t output_y = 0, bool verticalByte = false);
  ~Arduino_Canvas_Mono();

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;
  void startWrite() override;
  void writePixelPreclipped(int16_t x, int16_t y, uint16_t color) override;
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) override;
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) override;
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg) override;
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) override;
  void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) override;
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h) override;
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) override;
  void drawGrayscaleBitmapWithMask(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h) override;
  void drawGrayscaleBitmapWithMask(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h) override;
  void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h, int16_t x_skip = 0) override;
  void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, uint8_t chroma_key, int16_t w, int16_t h, int16_t x_skip = 0) override;
  void draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) override;
  void draw16bitRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h) override;
  void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) override;
  void draw16bitRGBBitmapWithMask(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h) override;
  void draw16bitRGBBitmapWithMask(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h) override;
  void draw16bitRGBBitmapWithTranColor(int16_t x, int16_t y, uint16_t *bitmap, uint16_t transparent_color, int16_t w, int16_t h) override;
  void draw16bitBeRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) override;
  void draw24bitRGBBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h) override;
  void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h) override;
  void draw24bitRGBBitmapWithMask(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h) override;
  void draw24bitRGBBitmapWithMask(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h) override;
  void draw16bitBeRGBBitmapR1(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h) override;
  void flush(bool force_flush = false) override;

  uint8_t *getFramebuffer();

protected:
  uint8_t *_framebuffer = nullptr;
  Arduino_G *_output = nullptr;
  int16_t _output_x, _output_y;
  bool _verticalByte;
  int16_t _canvas_width, _canvas_height;  // width and height of canvas buffer
  int16_t *_dither_error_buffer = nullptr;
  int16_t *_dither_error_current = nullptr;
  int16_t *_dither_error_next = nullptr;
  int16_t _dither_y = 0;
  bool _dither_started = false;

private:
  void writeDitheredPixel(int16_t x, int16_t y, uint16_t color);
};

#endif // _ARDUINO_CANVAS_MONO_H_

#endif // !defined(LITTLE_FOOT_PRINT)
