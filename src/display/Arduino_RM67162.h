#pragma once

#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define RM67162_TFTWIDTH 240  ///< RM67162 max TFT width
#define RM67162_TFTHEIGHT 536 ///< RM67162 max TFT height

#define RM67162_RST_DELAY 120    ///< delay ms wait for reset finish
#define RM67162_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define RM67162_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define RM67162_SWRESET 0x01 ///< Software reset register

#define RM67162_SLPIN 0x10  ///< Enter Sleep Mode
#define RM67162_SLPOUT 0x11 ///< Sleep Out

#define RM67162_INVOFF 0x20 ///< Display Inversion OFF
#define RM67162_INVON 0x21  ///< Display Inversion ON

#define RM67162_DISPOFF 0x28 ///< Display OFF
#define RM67162_DISPON 0x29  ///< Display ON

#define RM67162_CASET 0x2A ///< Column Address Set
#define RM67162_PASET 0x2B ///< Page Address Set
#define RM67162_RAMWR 0x2C ///< Memory Write
#define RM67162_RAMRD 0x2E ///< Memory Read

#define RM67162_MADCTL 0x36
#define RM67162_PIXFMT 0x3A // Interface Pixel Format

#define RM67162_BRIGHTNESS 0x51 // Write Display Brightness

#define RM67162_MADCTL_MY 0x80
#define RM67162_MADCTL_MX 0x40
#define RM67162_MADCTL_MV 0x20
#define RM67162_MADCTL_ML 0x10
#define RM67162_MADCTL_RGB 0x00
#define RM67162_MADCTL_MH 0x04
#define RM67162_MADCTL_BGR 0x08

static const uint8_t rm67162_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, RM67162_SLPOUT, // Sleep Out
    END_WRITE,

    DELAY, RM67162_SLPOUT_DELAY,

    BEGIN_WRITE,
    WRITE_C8_D8, RM67162_PIXFMT, 0x55,     // Interface Pixel Format 16bit/pixel
    WRITE_COMMAND_8, RM67162_DISPON,       // Display on
    WRITE_C8_D8, RM67162_BRIGHTNESS, 0xBF, // Write Display Brightness   MAX_VAL=0XFF
    END_WRITE};

// SPI-specific init sequence for T-Display S3 AMOLED Plus (RM67162 in SPI mode)
// Uses 0x75 pixel format (16-bit for SPI) and page register setup
static const uint8_t rm67162_spi_init_operations[] = {
    BEGIN_WRITE,
    WRITE_C8_D8, 0xFE, 0x04,              // SET PAGE3
    WRITE_C8_D8, 0x6A, 0x00,
    WRITE_C8_D8, 0xFE, 0x05,              // SET PAGE4
    WRITE_C8_D8, 0xFE, 0x07,              // SET PAGE6
    WRITE_C8_D8, 0x07, 0x4F,
    WRITE_C8_D8, 0xFE, 0x01,              // SET PAGE0
    WRITE_C8_D8, 0x2A, 0x02,
    WRITE_C8_D8, 0x2B, 0x73,
    WRITE_C8_D8, 0xFE, 0x0A,              // SET PAGE9
    WRITE_C8_D8, 0x29, 0x10,
    WRITE_C8_D8, 0xFE, 0x00,              // Return to page 0
    WRITE_C8_D8, RM67162_BRIGHTNESS, 0xBF,
    WRITE_C8_D8, 0x53, 0x20,
    WRITE_C8_D8, 0x35, 0x00,
    WRITE_C8_D8, RM67162_PIXFMT, 0x75,   // Interface Pixel Format 16bit/pixel (SPI)
    WRITE_C8_D8, 0xC4, 0x80,
    WRITE_COMMAND_8, RM67162_SLPOUT,      // Sleep Out
    END_WRITE,

    DELAY, RM67162_SLPOUT_DELAY,

    BEGIN_WRITE,
    WRITE_COMMAND_8, RM67162_DISPON,      // Display on
    END_WRITE,

    DELAY, 20};

class Arduino_RM67162 : public Arduino_TFT
{
public:
  Arduino_RM67162(Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0, bool ips = false, const uint8_t *init_operations = rm67162_init_operations, int16_t init_operations_len = sizeof(rm67162_init_operations));

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;

  void setRotation(uint8_t r) override;

  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h);

  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;
  void setBrightness(uint8_t brightness);

protected:
  void tftInit() override;

private:
  const uint8_t *_init_operations;
  int16_t _init_operations_len;
};
