#pragma once

#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define RM690B0_TFTWIDTH 482  ///< RM690B0 max TFT width
#define RM690B0_TFTHEIGHT 600 ///< RM690B0 max TFT height

#define RM690B0_RST_DELAY 120    ///< delay ms wait for reset finish
#define RM690B0_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define RM690B0_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define RM690B0_SWRESET 0x01 ///< Software reset register

#define RM690B0_SLPIN 0x10  ///< Enter Sleep Mode
#define RM690B0_SLPOUT 0x11 ///< Sleep Out

#define RM690B0_INVOFF 0x20 ///< Display Inversion OFF
#define RM690B0_INVON 0x21  ///< Display Inversion ON

#define RM690B0_DISPOFF 0x28 ///< Display OFF
#define RM690B0_DISPON 0x29  ///< Display ON

#define RM690B0_CASET 0x2A ///< Column Address Set
#define RM690B0_PASET 0x2B ///< Page Address Set
#define RM690B0_RAMWR 0x2C ///< Memory Write
#define RM690B0_RAMRD 0x2E ///< Memory Read

#define RM690B0_MADCTL 0x36
#define RM690B0_PIXFMT 0x3A // Interface Pixel Format

#define RM690B0_BRIGHTNESS 0x51 // Write Display Brightness

#define RM690B0_MADCTL_MY 0x80
#define RM690B0_MADCTL_MX 0x40
#define RM690B0_MADCTL_MV 0x20
#define RM690B0_MADCTL_ML 0x10
#define RM690B0_MADCTL_RGB 0x00
#define RM690B0_MADCTL_MH 0x04
#define RM690B0_MADCTL_BGR 0x08

static const uint8_t rm690b0_init_operations[] = {
    BEGIN_WRITE,
    WRITE_C8_D8, 0xFE, 0x20,           // SET PAGE
    WRITE_C8_D8, 0x26, 0x0A,           // MIPI OFF
    WRITE_C8_D8, 0x24, 0x80,           // SPI write RAM
    WRITE_C8_D8, 0x5A, 0x51,           //! 230918:SWIRE FOR BV6804
    WRITE_C8_D8, 0x5B, 0x2E,           //! 230918:SWIRE FOR BV6804
    WRITE_C8_D8, 0xFE, 0x00,           // SET PAGE
    WRITE_C8_D8, RM690B0_PIXFMT, 0x55, // Interface Pixel Format    16bit/pixel
    WRITE_COMMAND_8, 0xC2,             // delay_ms(10);
    END_WRITE,
    DELAY, 10, // delay_ms(10);
    BEGIN_WRITE,
    WRITE_C8_D8, 0x35, 0x00,               // TE ON
    WRITE_C8_D8, RM690B0_BRIGHTNESS, 0x00, // Write Display Brightness  MAX_VAL=0XFF
    WRITE_COMMAND_8, RM690B0_SLPOUT,       // Sleep Out
    END_WRITE,
    DELAY, RM690B0_SLPOUT_DELAY,
    BEGIN_WRITE,
    WRITE_COMMAND_8, RM690B0_DISPON, // Display on delay_ms(10);
    END_WRITE,
    DELAY, 10,
    BEGIN_WRITE,
    WRITE_C8_D8, RM690B0_BRIGHTNESS, 0xD0, // Write Display Brightness  MAX_VAL=0XFF
    END_WRITE};

class Arduino_RM690B0 : public Arduino_TFT
{
public:
  Arduino_RM690B0(
      Arduino_DataBus *bus, int8_t rst, uint8_t r,
      int16_t w = RM690B0_TFTWIDTH, int16_t h = RM690B0_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;

  void setRotation(uint8_t r) override;

  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h);

  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

protected:
  void tftInit() override;

private:
};
