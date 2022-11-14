#ifndef _ARDUINO_NV3041A_H_
#define _ARDUINO_NV3041A_H_

#include <Arduino.h>
#include <Print.h>
#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define NV3041A_TFTWIDTH 480
#define NV3041A_TFTHEIGHT 272

#define NV3041A_RST_DELAY 800    ///< delay ms wait for reset finish
#define NV3041A_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define NV3041A_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define NV3041A_NOP 0x00
#define NV3041A_SWRESET 0x01

#define NV3041A_SLPIN 0x10
#define NV3041A_SLPOUT 0x11

#define NV3041A_DISPOFF 0x28
#define NV3041A_DISPON 0x29

#define NV3041A_CASET 0x2A
#define NV3041A_RASET 0x2B
#define NV3041A_RAMWR 0x2C
#define NV3041A_RAMRD 0x2E

#define NV3041A_MADCTL 0x36
#define NV3041A_COLMOD 0x3A

#define NV3041A_MADCTL_MY 0x80
#define NV3041A_MADCTL_MX 0x40
#define NV3041A_MADCTL_MV 0x20
#define NV3041A_MADCTL_ML 0x10
#define NV3041A_MADCTL_RGB 0x00

class Arduino_NV3041A : public Arduino_TFT
{
public:
  Arduino_NV3041A(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = NV3041A_TFTWIDTH, int16_t h = NV3041A_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  void begin(int32_t speed = GFX_NOT_DEFINED) override;

  void setRotation(uint8_t r) override;

  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;

  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

protected:
  void tftInit() override;

private:
};

#endif
