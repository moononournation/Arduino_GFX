/*
 * SSD1357 driver
 *
 * Supports panels up to 128x128, e.g.:
 *   - 128x128
 *   - 128x64
 *   - 64x64  (e.g. WiseChip UG-6464TDDBG01)
 */
#ifndef _ARDUINO_SSD1357_H_
#define _ARDUINO_SSD1357_H_

#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define SSD1357_TFTWIDTH 128  ///< SSD1357 max TFT width
#define SSD1357_TFTHEIGHT 128 ///< SSD1357 max TFT height

#define SSD1357_RST_DELAY 120

#define SSD1357_SETCOLUMN 0x15
#define SSD1357_SETROW 0x75
#define SSD1357_WRITERAM 0x5C
#define SSD1357_READRAM 0x5D
#define SSD1357_SETREMAP 0xA0
#define SSD1357_STARTLINE 0xA1
#define SSD1357_DISPLAYOFFSET 0xA2
#define SSD1357_DISPLAYALLOFF 0xA4
#define SSD1357_DISPLAYALLON 0xA5
#define SSD1357_NORMALDISPLAY 0xA6
#define SSD1357_INVERTDISPLAY 0xA7
#define SSD1357_FUNCTIONSELECT 0xAB
#define SSD1357_DISPLAYOFF 0xAE
#define SSD1357_DISPLAYON 0xAF
#define SSD1357_PRECHARGE 0xB1
#define SSD1357_CLOCKDIV 0xB3
#define SSD1357_PRECHARGE2 0xB6
#define SSD1357_SETGRAY 0xB8
#define SSD1357_USELUT 0xB9
#define SSD1357_PRECHARGELEVEL 0xBB
#define SSD1357_GRAYLUT_A 0xBC
#define SSD1357_GRAYLUT_C 0xBD
#define SSD1357_VCOMH 0xBE
#define SSD1357_CONTRASTABC 0xC1
#define SSD1357_CONTRASTMASTER 0xC7
#define SSD1357_MUXRATIO 0xCA
#define SSD1357_COMMANDLOCK 0xFD

class Arduino_SSD1357 : public Arduino_TFT
{
public:
  Arduino_SSD1357(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      int16_t w = SSD1357_TFTWIDTH, int16_t h = SSD1357_TFTHEIGHT,
      int16_t col_offset1 = GFX_NOT_DEFINED, int16_t row_offset1 = GFX_NOT_DEFINED,
      int16_t col_offset2 = GFX_NOT_DEFINED, int16_t row_offset2 = GFX_NOT_DEFINED);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;
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
