/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_TFT_H_
#define _ARDUINO_TFT_H_

#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"

class Arduino_TFT : public Arduino_GFX
{

public:
  Arduino_TFT(Arduino_DataBus *bus, int8_t rst, uint8_t r, int16_t w, int16_t h, uint8_t col_offset, uint8_t row_offset);

  virtual void begin(uint32_t speed = 0);
  virtual void startWrite(void);
  virtual void writeColor(uint16_t color);
  virtual void writePixel(int16_t x, int16_t y, uint16_t color);
  virtual void writePixelPreclipped(int16_t x, int16_t y, uint16_t color);
  virtual void writePixels(uint16_t color, uint32_t len);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  virtual void endWrite(void);

  virtual void pushColor(uint16_t color);
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
  virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

  virtual void writeAddrColumn(uint16_t x, uint16_t w) = 0;
  virtual void writeAddrRow(uint16_t y, uint16_t h) = 0;
  virtual void writeAddrMemWrite() = 0;
  virtual void setRotation(uint8_t) = 0;
  virtual void invertDisplay(boolean) = 0;
  virtual void displayOn() = 0;
  virtual void displayOff() = 0;

protected:
  int8_t _rst;
  Arduino_DataBus *_bus;
  uint8_t COL_OFFSET, ROW_OFFSET;
  uint8_t _xstart, _ystart;
  virtual void tftInit() = 0;

private:
};

#endif
