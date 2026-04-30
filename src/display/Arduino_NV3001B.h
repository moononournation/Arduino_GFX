#ifndef _ARDUINO_NV3001B_H_
#define _ARDUINO_NV3001B_H_

#include "./Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define NV3001B_TFTWIDTH 128
#define NV3001B_TFTHEIGHT 220

#define NV3001B_RST_DELAY 120    ///< delay ms wait for reset finish
#define NV3001B_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define NV3001B_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define NV3001B_NOP 0x00
#define NV3001B_SWRESET 0x01
#define NV3001B_RDDID 0x04
#define NV3001B_RDDST 0x09

#define NV3001B_SLPIN 0x10
#define NV3001B_SLPOUT 0x11
#define NV3001B_PTLON 0x12
#define NV3001B_NORON 0x13

#define NV3001B_INVOFF 0x20
#define NV3001B_INVON 0x21
#define NV3001B_DISPOFF 0x28
#define NV3001B_DISPON 0x29

#define NV3001B_CASET 0x2A
#define NV3001B_RASET 0x2B
#define NV3001B_RAMWR 0x2C
#define NV3001B_RAMRD 0x2E

#define NV3001B_PTLAR 0x30
#define NV3001B_TEON 0x35
#define NV3001B_TELON NV3001B_TEON
#define NV3001B_MADCTL 0x36
#define NV3001B_COLMOD 0x3A
#define NV3001B_SCANLSET 0x44

#define NV3001B_FRMCTR1 0xB1
#define NV3001B_FRMCTR2 0xB2
#define NV3001B_FRMCTR3 0xB3

#define NV3001B_INVCTR 0xB4
#define NV3001B_VREG1CTL 0xE6
#define NV3001B_VREG2CTL 0xE7
#define NV3001B_GAMMA1 0xF0
#define NV3001B_GAMMA2 0xF1
#define NV3001B_INTERRE1 0xFE
#define NV3001B_INTERRE2 0xEF

#define NV3001B_MADCTL_MY 0x80
#define NV3001B_MADCTL_MX 0x40
#define NV3001B_MADCTL_MV 0x20
#define NV3001B_MADCTL_ML 0x10
#define NV3001B_MADCTL_BGR 0x08
#define NV3001B_MADCTL_MH 0x04
#define NV3001B_MADCTL_RGB 0x00

static const uint8_t NV3001B_init_operations[] = {
    BEGIN_WRITE,
    WRITE_C8_D8, 0xFF, 0xA5,
    WRITE_C8_D8, 0x41, 0x00,
    WRITE_C8_D8, 0x50, 0x02,
    WRITE_C8_D8, 0x52, 0x6E,
    WRITE_C8_D8, 0x57, 0x02,
    WRITE_C8_D8, 0x46, 0x11,
    WRITE_C8_D16, 0x47, 0x00, 0x01,
    WRITE_C8_D16, 0x8F, 0x22, 0x03,

    WRITE_C8_D8, 0x9A, 0x78,
    WRITE_C8_D8, 0x9B, 0x78,
    WRITE_C8_D8, 0x9C, 0xA0,
    WRITE_C8_D8, 0x9D, 0x17,
    WRITE_C8_D8, 0x9E, 0xC1,

    WRITE_C8_D8, 0x83, 0x5A,
    WRITE_C8_D8, 0x84, 0xB6,
    WRITE_C8_D8, 0xFF, 0xA5,
    WRITE_C8_D8, 0x85, 0x5F,

    WRITE_C8_D8, 0x6E, 0x0F,
    WRITE_C8_D8, 0x7E, 0x0F,
    WRITE_C8_D8, 0x60, 0x00,
    WRITE_C8_D8, 0x70, 0x00,
    WRITE_C8_D8, 0x6D, 0x33,
    WRITE_C8_D8, 0x7D, 0x37,
    WRITE_C8_D8, 0x61, 0x09,
    WRITE_C8_D8, 0x71, 0x0A,
    WRITE_C8_D8, 0x6C, 0x2A,
    WRITE_C8_D8, 0x7C, 0x36,
    WRITE_C8_D8, 0x62, 0x11,
    WRITE_C8_D8, 0x72, 0x10,
    WRITE_C8_D8, 0x68, 0x4E,
    WRITE_C8_D8, 0x78, 0x4E,
    WRITE_C8_D8, 0x66, 0x36,
    WRITE_C8_D8, 0x76, 0x3C,
    WRITE_C8_D8, 0x1A, 0x1C,
    WRITE_C8_D8, 0x7B, 0x14,
    WRITE_C8_D8, 0x63, 0x0D,
    WRITE_C8_D8, 0x73, 0x0A,
    WRITE_C8_D8, 0x6A, 0x16,
    WRITE_C8_D8, 0x7A, 0x12,
    WRITE_C8_D8, 0x64, 0x0B,
    WRITE_C8_D8, 0x74, 0x0A,
    WRITE_C8_D8, 0x69, 0x08,
    WRITE_C8_D8, 0x79, 0x0A,
    WRITE_C8_D8, 0x65, 0x06,
    WRITE_C8_D8, 0x75, 0x07,
    WRITE_C8_D8, 0x67, 0x23,
    WRITE_C8_D8, 0x77, 0x44,

    WRITE_C8_D8, 0xE0, 0x00,
    WRITE_C8_D8, 0xE9, 0x30,
    WRITE_C8_D8, 0xEB, 0xB7,
    WRITE_C8_D8, 0xEC, 0x00,
    WRITE_C8_D8, 0xED, 0x11,
    WRITE_C8_D8, 0xF0, 0xB7,

    WRITE_C8_D8, 0x53, 0x04,
    WRITE_C8_D8, 0x54, 0x04,
    WRITE_C8_D8, 0x55, 0x40,
    WRITE_C8_D8, 0x56, 0x40,

    WRITE_C8_D16, 0xA0, 0x60, 0x01,
    WRITE_C8_D8, 0xA1, 0x84,
    WRITE_C8_D8, 0xA2, 0x85,
    WRITE_C8_D16, 0xAB, 0x00, 0x02,
    WRITE_C8_D16, 0xAC, 0x00, 0x06,
    WRITE_C8_D16, 0xAD, 0x00, 0x03,
    WRITE_C8_D16, 0xAE, 0x00, 0x07,
    WRITE_C8_D8, 0xC7, 0x01,
    WRITE_C8_D8, 0xB9, 0x82,
    WRITE_C8_D8, 0xBA, 0x83,
    WRITE_C8_D8, 0xBB, 0x00,
    WRITE_C8_D8, 0xBC, 0x81,
    WRITE_C8_D8, 0xBD, 0x02,
    WRITE_C8_D8, 0xBE, 0x01,
    WRITE_C8_D8, 0xBF, 0x04,
    WRITE_C8_D8, 0xC0, 0x03,
    WRITE_C8_D8, 0xC8, 0x55,
    WRITE_C8_D8, 0xC9, 0xC9,
    WRITE_C8_D8, 0xCA, 0xC8,
    WRITE_C8_D8, 0xCB, 0xCB,
    WRITE_C8_D8, 0xCC, 0xCA,
    WRITE_C8_D8, 0xCD, 0x55,
    WRITE_C8_D8, 0xCE, 0xCE,
    WRITE_C8_D8, 0xCF, 0xCD,
    WRITE_C8_D8, 0xD0, 0xD0,
    WRITE_C8_D8, 0xD1, 0xCF,
    WRITE_C8_D8, 0xF2, 0x46,
    WRITE_C8_D8, 0xA8, 0x04,
    WRITE_C8_D8, 0xA9, 0xB0,
    WRITE_C8_D8, 0xAA, 0xA3,
    WRITE_C8_D8, 0xB6, 0x00,
    WRITE_C8_D8, 0xB7, 0xB0,
    WRITE_C8_D8, 0xB8, 0xA3,
    WRITE_C8_D8, 0xC4, 0x03,
    WRITE_C8_D8, 0xC5, 0xB0,
    WRITE_C8_D8, 0xC6, 0xA3,

    WRITE_C8_D8, 0x80, 0x10,
    WRITE_C8_D8, 0xFF, 0x00,
    WRITE_C8_D8, 0x35, 0x00,
    WRITE_COMMAND_8, NV3001B_SLPOUT,
    END_WRITE,

    DELAY, NV3001B_SLPOUT_DELAY,

    BEGIN_WRITE,
    WRITE_C8_D8, NV3001B_COLMOD, 0x05,
    WRITE_C8_D8, NV3001B_MADCTL, 0x00,
    WRITE_COMMAND_8, NV3001B_DISPON,
    END_WRITE,

    DELAY, 10};

class Arduino_NV3001B : public Arduino_TFT
{
public:
  Arduino_NV3001B(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = NV3001B_TFTWIDTH, int16_t h = NV3001B_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

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
