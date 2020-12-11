/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/adafruit/Adafruit_ILI9341.git
 */
#ifndef _ARDUINO_ILI9341_M5STACK_H_
#define _ARDUINO_ILI9341_M5STACK_H_

#include <Arduino.h>
#include <Print.h>
#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"
#include "Arduino_ILI9341.h"

class Arduino_ILI9341_M5STACK : public Arduino_ILI9341
{
public:
  Arduino_ILI9341_M5STACK(Arduino_DataBus *bus, int8_t rst = -1, uint8_t r = 0);

  virtual void setRotation(uint8_t r);

protected:
  virtual void tftInit();

private:
};

#endif
