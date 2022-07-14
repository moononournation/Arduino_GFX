#ifndef _ARDUINO_GC9107_H_
#define _ARDUINO_GC9107_H_

#include <Arduino.h>
#include <Print.h>
#include "./Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define GC9107_TFTWIDTH 128
#define GC9107_TFTHEIGHT 128

#define GC9107_RST_DELAY 120    ///< delay ms wait for reset finish
#define GC9107_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define GC9107_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define GC9107_NOP 0x00
#define GC9107_SWRESET 0x01
#define GC9107_RDDID 0x04
#define GC9107_RDDST 0x09

#define GC9107_SLPIN 0x10
#define GC9107_SLPOUT 0x11
#define GC9107_PTLON 0x12
#define GC9107_NORON 0x13

#define GC9107_INVOFF 0x20
#define GC9107_INVON 0x21
#define GC9107_DISPOFF 0x28
#define GC9107_DISPON 0x29

#define GC9107_CASET 0x2A
#define GC9107_RASET 0x2B
#define GC9107_RAMWR 0x2C
#define GC9107_RAMRD 0x2E

#define GC9107_PTLAR 0x30
#define GC9107_TELON 0x35
#define GC9107_MADCTL 0x36
#define GC9107_COLMOD 0x3A
#define GC9107_SCANLSET 0x44

#define GC9107_FRMCTR1 0xB1
#define GC9107_FRMCTR2 0xB2
#define GC9107_FRMCTR3 0xB3

#define GC9107_INVCTR 0xB4
#define GC9107_VREG1CTL 0xE6
#define GC9107_VREG2CTL 0xE7
#define GC9107_GAMMA1 0xF0
#define GC9107_GAMMA2 0xF1
#define GC9107_INTERRE1 0xFE
#define GC9107_INTERRE2 0xEF

#define GC9107_MADCTL_MY 0x80
#define GC9107_MADCTL_MX 0x40
#define GC9107_MADCTL_MV 0x20
#define GC9107_MADCTL_ML 0x10
#define GC9107_MADCTL_BGR 0x08
#define GC9107_MADCTL_MH 0x04
#define GC9107_MADCTL_RGB 0x00


class Arduino_GC9107 : public Arduino_TFT
{
public:
  Arduino_GC9107(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = GC9107_TFTWIDTH, int16_t h = GC9107_TFTHEIGHT,
      uint8_t col_offset1 = 2, uint8_t row_offset1 = 1, uint8_t col_offset2 = 2, uint8_t row_offset2 = 1);

  void begin(int32_t speed = GFX_NOT_DEFINED) override;
  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;
  void setRotation(uint8_t r) override;
  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

protected:
  void tftInit() override;

private:
};

#endif
