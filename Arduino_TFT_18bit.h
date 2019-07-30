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

  virtual inline void writeColor(uint16_t color);
  virtual void writePixels(uint16_t color, uint32_t len);

  virtual void writeAddrColumn(uint16_t x, uint16_t w) = 0;
  virtual void writeAddrRow(uint16_t y, uint16_t h) = 0;
  virtual void writeAddrMemWrite() = 0;
  virtual void invertDisplay(bool) = 0;
  virtual void displayOn() = 0;
  virtual void displayOff() = 0;

protected:

private:
};

#endif
