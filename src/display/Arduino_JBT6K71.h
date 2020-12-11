/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/gitcnd/LCDWIKI_SPI.git
 */
#ifndef _ARDUINO_JBT6K71_H_
#define _ARDUINO_JBT6K71_H_

#include <Arduino.h>
#include <Print.h>
#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define JBT6K71_TFTWIDTH 240  ///< JBT6K71 max TFT width
#define JBT6K71_TFTHEIGHT 320 ///< JBT6K71 max TFT height

class Arduino_JBT6K71 : public Arduino_TFT
{
public:
  Arduino_JBT6K71(
      Arduino_DataBus *bus, int8_t rst = -1, uint8_t r = 0,
      bool ips = false, int16_t w = JBT6K71_TFTWIDTH, int16_t h = JBT6K71_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  virtual void begin(int32_t speed = 0);
  virtual void writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  virtual void setRotation(uint8_t r);
  virtual void invertDisplay(bool);
  virtual void displayOn();
  virtual void displayOff();

protected:
  virtual void tftInit();

private:
};

#endif
