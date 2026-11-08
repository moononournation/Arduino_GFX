#pragma once

#include "Arduino_ILI9488.h"
#include "../Arduino_TFT_18bit.h"

class Arduino_ILI9488_18bit : public Arduino_TFT_18bit
{
public:
  Arduino_ILI9488_18bit(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = ILI9488_TFTWIDTH, int16_t h = ILI9488_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;

  void setRotation(uint8_t r) override;

  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;

  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

protected:
  void tftInit() override;

private:
};
