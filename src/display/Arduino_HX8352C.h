/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HX8352C_H_
#define _ARDUINO_HX8352C_H_

#include <Arduino.h>
#include <Print.h>
#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define HX8352C_TFTWIDTH 240  ///< HX8352C max TFT width
#define HX8352C_TFTHEIGHT 400 ///< HX8352C max TFT height

class Arduino_HX8352C : public Arduino_TFT
{
public:
  Arduino_HX8352C(
      Arduino_DataBus *bus, int8_t rst = -1, uint8_t r = 0,
      bool ips = false, int16_t w = HX8352C_TFTWIDTH, int16_t h = HX8352C_TFTHEIGHT,
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
