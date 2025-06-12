#pragma once

#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_OLED.h"

class Arduino_OLED : public Arduino_TFT
{
public:
  Arduino_OLED(Arduino_DataBus *bus, int8_t rst, uint8_t r, int16_t w, int16_t h, uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2);

  // This MUST be defined by the subclass:
  virtual void setBrightness(uint8_t brightness) = 0;
  virtual void setContrast(uint8_t contrast) = 0;

protected:
private:
};
