/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HX8357B_H_
#define _ARDUINO_HX8357B_H_

#include <Arduino.h>
#include <Print.h>
#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define HX8357B_TFTWIDTH 320  ///< HX8357B max TFT width
#define HX8357B_TFTHEIGHT 480 ///< HX8357B max TFT height

#define HX8357B_RST_DELAY 120    ///< delay ms wait for reset finish
#define HX8357B_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define HX8357B_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define HX8357B_NOP 0x00     ///< No-op register
#define HX8357B_SWRESET 0x01 ///< Software reset register
#define HX8357B_RDDID 0x04   ///< Read display identification information
#define HX8357B_RDDST 0x09   ///< Read Display Status

#define HX8357B_SLPIN 0x10  ///< Enter Sleep Mode
#define HX8357B_SLPOUT 0x11 ///< Sleep Out
#define HX8357B_PTLON 0x12  ///< Partial Mode ON
#define HX8357B_NORON 0x13  ///< Normal Display Mode ON

#define HX8357B_RDMODE 0x0A     ///< Read Display Power Mode
#define HX8357B_RDMADCTL 0x0B   ///< Read Display MADCTL
#define HX8357B_RDPIXFMT 0x0C   ///< Read Display Pixel Format
#define HX8357B_RDIMGFMT 0x0D   ///< Read Display Image Format
#define HX8357B_RDSELFDIAG 0x0F ///< Read Display Self-Diagnostic Result

#define HX8357B_INVOFF 0x20   ///< Display Inversion OFF
#define HX8357B_INVON 0x21    ///< Display Inversion ON
#define HX8357B_GAMMASET 0x26 ///< Gamma Set
#define HX8357B_DISPOFF 0x28  ///< Display OFF
#define HX8357B_DISPON 0x29   ///< Display ON

#define HX8357B_CASET 0x2A ///< Column Address Set
#define HX8357B_PASET 0x2B ///< Page Address Set
#define HX8357B_RAMWR 0x2C ///< Memory Write
#define HX8357B_RAMRD 0x2E ///< Memory Read

#define HX8357B_PTLAR 0x30    ///< Partial Area
#define HX8357B_VSCRDEF 0x33  ///< Vertical Scrolling Definition
#define HX8357B_MADCTL 0x36   ///< Memory Access Control
#define HX8357B_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
#define HX8357B_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set

#define HX8357B_FRMCTR1 0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
#define HX8357B_FRMCTR2 0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
#define HX8357B_FRMCTR3 0xB3 ///< Frame Rate control (In Partial Mode/Full Colors)
#define HX8357B_INVCTR 0xB4  ///< Display Inversion Control
#define HX8357B_DFUNCTR 0xB6 ///< Display Function Control

#define HX8357B_PWCTR1 0xC0 ///< Power Control 1
#define HX8357B_PWCTR2 0xC1 ///< Power Control 2
#define HX8357B_PWCTR3 0xC2 ///< Power Control 3
#define HX8357B_PWCTR4 0xC3 ///< Power Control 4
#define HX8357B_PWCTR5 0xC4 ///< Power Control 5
#define HX8357B_VMCTR1 0xC5 ///< VCOM Control 1
#define HX8357B_VMCTR2 0xC7 ///< VCOM Control 2

#define HX8357B_RDID1 0xDA ///< Read ID 1
#define HX8357B_RDID2 0xDB ///< Read ID 2
#define HX8357B_RDID3 0xDC ///< Read ID 3
#define HX8357B_RDID4 0xDD ///< Read ID 4

#define HX8357B_GMCTRP1 0xE0 ///< Positive Gamma Correction
#define HX8357B_GMCTRN1 0xE1 ///< Negative Gamma Correction
#define HX8357B_PWCTR6 0xFC

#define HX8357B_MADCTL_MY 0x80  ///< Bottom to top
#define HX8357B_MADCTL_MX 0x40  ///< Right to left
#define HX8357B_MADCTL_MV 0x20  ///< Reverse Mode
#define HX8357B_MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define HX8357B_MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define HX8357B_MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define HX8357B_MADCTL_MH 0x04  ///< LCD refresh right to left

class Arduino_HX8357B : public Arduino_TFT
{
public:
  Arduino_HX8357B(Arduino_DataBus *bus, int8_t rst = -1, uint8_t r = 0, bool ips = false);

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
