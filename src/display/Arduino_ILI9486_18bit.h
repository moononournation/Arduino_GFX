#pragma once

#include "Arduino_ILI9486.h"
#include "../Arduino_TFT_18bit.h"

static const uint8_t ili9486_18bit_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, ILI9486_SLPOUT,
    END_WRITE,

    DELAY, ILI9486_SLPOUT_DELAY,

    BEGIN_WRITE,
    WRITE_C8_D8, ILI9486_PIXFMT, 0x66, // 18-bit color

    WRITE_C8_D8, 0xC2, 0x44,

    WRITE_COMMAND_8, 0xC5,
    WRITE_BYTES, 4, 0x00, 0x00, 0x00, 0x00,

    WRITE_COMMAND_8, 0xE0,
    WRITE_BYTES, 15,
    0x0F, 0x1F, 0x1C, 0x0C,
    0x0F, 0x08, 0x48, 0x98,
    0x37, 0x0A, 0x13, 0x04,
    0x11, 0x0D, 0x00,

    WRITE_COMMAND_8, 0xE1,
    WRITE_BYTES, 15,
    0x0F, 0x32, 0x2E, 0x0B,
    0x0D, 0x05, 0x47, 0x75,
    0x37, 0x06, 0x10, 0x03,
    0x24, 0x20, 0x00,

    WRITE_COMMAND_8, ILI9486_DISPON,
    END_WRITE,
    DELAY, 25};

class Arduino_ILI9486_18bit : public Arduino_TFT_18bit
{
public:
  Arduino_ILI9486_18bit(Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0, bool ips = false);

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
