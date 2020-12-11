/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HX8347D_H_
#define _ARDUINO_HX8347D_H_

#include <Arduino.h>
#include <Print.h>
#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define HX8347D_TFTWIDTH 240  ///< HX8347D max TFT width
#define HX8347D_TFTHEIGHT 320 ///< HX8347D max TFT height

class Arduino_HX8347D : public Arduino_TFT
{
public:
  Arduino_HX8347D(
      Arduino_DataBus *bus, int8_t rst = -1, uint8_t r = 0,
      bool ips = false, int16_t w = HX8347D_TFTWIDTH, int16_t h = HX8347D_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  virtual void begin(int32_t speed = 0);
  virtual void writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  virtual void setRotation(uint8_t r);
  virtual void invertDisplay(bool);
  virtual void displayOn();
  virtual void displayOff();

protected:
  virtual void tftInit();
  bool _invert = false;

private:
};

#endif
