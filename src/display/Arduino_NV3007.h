#ifndef _ARDUINO_NV3007_H_
#define _ARDUINO_NV3007_H_

#include <Arduino.h>
#include <Print.h>
#include "./Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define NV3007_TFTWIDTH 168
#define NV3007_TFTHEIGHT 428

#define NV3007_RST_DELAY 120    ///< delay ms wait for reset finish
#define NV3007_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define NV3007_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define NV3007_SLPIN 0x10
#define NV3007_SLPOUT 0x11

#define NV3007_INVOFF 0x20
#define NV3007_INVON 0x21
#define NV3007_DISPOFF 0x28
#define NV3007_DISPON 0x29

#define NV3007_CASET 0x2A
#define NV3007_RASET 0x2B
#define NV3007_RAMWR 0x2C
#define NV3007_RAMRD 0x2E

#define NV3007_MADCTL 0x36

static const uint8_t NV3007_init_operations[] = {
    BEGIN_WRITE,

    WRITE_C8_D8, 0xFF, 0xA5,

    WRITE_COMMAND_8, 0x11,
    DELAY, 120, // 120ms

    WRITE_C8_D8, 0xff, 0xa5,
    WRITE_C8_D8, 0x9a, 0x08,
    WRITE_C8_D8, 0x9b, 0x08,
    WRITE_C8_D8, 0x9c, 0xb0,
    WRITE_C8_D8, 0x9d, 0x17,
    WRITE_C8_D8, 0x9e, 0xc2,
    WRITE_C8_D16, 0x8f, 0x22, 0x04,
    WRITE_C8_D8, 0x84, 0x90,
    WRITE_C8_D8, 0x83, 0x7B,
    WRITE_C8_D8, 0x85, 0x4F,
    ////GAMMA---------------------------------/////////////
    // V0[3:0]
    WRITE_C8_D8, 0x6e, 0x0f,
    WRITE_C8_D8, 0x7e, 0x0f,

    // V63[3:0]
    WRITE_C8_D8, 0x60, 0x00,
    WRITE_C8_D8, 0x70, 0x00,
    // V1[5:0]
    WRITE_C8_D8, 0x6d, 0x39,
    WRITE_C8_D8, 0x7d, 0x31,
    // V62[5:0]
    WRITE_C8_D8, 0x61, 0x0A,
    WRITE_C8_D8, 0x71, 0x0A,
    // V2[5:0]
    WRITE_C8_D8, 0x6c, 0x35,
    WRITE_C8_D8, 0x7c, 0x29,
    // V61[5:0]
    WRITE_C8_D8, 0x62, 0x0F,
    WRITE_C8_D8, 0x72, 0x0F,
    // V20[6:0]
    WRITE_C8_D8, 0x68, 0x4f,
    WRITE_C8_D8, 0x78, 0x45,
    // V43[6:0]
    WRITE_C8_D8, 0x66, 0x33,
    WRITE_C8_D8, 0x76, 0x33,
    // V4[4:0]
    WRITE_C8_D8, 0x6b, 0x14,
    WRITE_C8_D8, 0x7b, 0x14,
    // V59[4:0]
    WRITE_C8_D8, 0x63, 0x09,
    WRITE_C8_D8, 0x73, 0x09,
    // V6[4:0]
    WRITE_C8_D8, 0x6a, 0x13,
    WRITE_C8_D8, 0x7a, 0x16,
    // V57[4:0]
    WRITE_C8_D8, 0x64, 0x08,
    WRITE_C8_D8, 0x74, 0x08,
    WRITE_C8_D8, 0x69, 0x07,
    WRITE_C8_D8, 0x79, 0x0d,
    WRITE_C8_D8, 0x65, 0x05,
    WRITE_C8_D8, 0x75, 0x05,
    WRITE_C8_D8, 0x67, 0x33,
    WRITE_C8_D8, 0x77, 0x33,
    WRITE_C8_D8, 0x6f, 0x00,
    WRITE_C8_D8, 0x7f, 0x00,
    WRITE_C8_D8, 0x50, 0x00,
    WRITE_C8_D8, 0x52, 0xd6,
    WRITE_C8_D8, 0x53, 0x04,
    WRITE_C8_D8, 0x54, 0x04,
    WRITE_C8_D8, 0x55, 0x1b,
    WRITE_C8_D8, 0x56, 0x1b,
    WRITE_COMMAND_8, 0xa0,
    WRITE_BYTES, 3, 0x2a, 0x24, 0x00,
    WRITE_C8_D8, 0xa1, 0x84,
    WRITE_C8_D8, 0xa2, 0x85,
    WRITE_C8_D8, 0xa8, 0x34,
    WRITE_C8_D8, 0xa9, 0x80,
    WRITE_C8_D8, 0xaa, 0x73,
    WRITE_C8_D16, 0xAB, 0x03, 0x61,
    WRITE_C8_D16, 0xAC, 0x03, 0x65,
    WRITE_C8_D16, 0xAD, 0x03, 0x60,
    WRITE_C8_D16, 0xAE, 0x03, 0x64,
    WRITE_C8_D8, 0xB9, 0x82,
    WRITE_C8_D8, 0xBA, 0x83,
    WRITE_C8_D8, 0xBB, 0x80,
    WRITE_C8_D8, 0xBC, 0x81,
    WRITE_C8_D8, 0xBD, 0x02,
    WRITE_C8_D8, 0xBE, 0x01,
    WRITE_C8_D8, 0xBF, 0x04,
    WRITE_C8_D8, 0xC0, 0x03,
    WRITE_C8_D8, 0xc4, 0x33,
    WRITE_C8_D8, 0xc5, 0x80,
    WRITE_C8_D8, 0xc6, 0x73,
    WRITE_C8_D8, 0xc7, 0x00,
    WRITE_C8_D16, 0xC8, 0x33, 0x33,
    WRITE_C8_D8, 0xC9, 0x5b,
    WRITE_C8_D8, 0xCA, 0x5a,
    WRITE_C8_D8, 0xCB, 0x5d,
    WRITE_C8_D8, 0xCC, 0x5c,
    WRITE_C8_D16, 0xCD, 0x33, 0x33,
    WRITE_C8_D8, 0xCE, 0x5f,
    WRITE_C8_D8, 0xCF, 0x5e,
    WRITE_C8_D8, 0xD0, 0x61,
    WRITE_C8_D8, 0xD1, 0x60,
    WRITE_COMMAND_8, 0xB0,
    WRITE_BYTES, 4, 0x3a, 0x3a, 0x00, 0x00,
    WRITE_C8_D8, 0xB6, 0x32,
    WRITE_C8_D8, 0xB7, 0x80,
    WRITE_C8_D8, 0xB8, 0x73,
    WRITE_C8_D8, 0xe0, 0x00,
    WRITE_C8_D16, 0xe1, 0x03, 0x0f,
    WRITE_C8_D8, 0xe2, 0x04,
    WRITE_C8_D8, 0xe3, 0x01,
    WRITE_C8_D8, 0xe4, 0x0e,
    WRITE_C8_D8, 0xe5, 0x01,
    WRITE_C8_D8, 0xe6, 0x19,
    WRITE_C8_D8, 0xe7, 0x10,
    WRITE_C8_D8, 0xe8, 0x10,
    WRITE_C8_D8, 0xe9, 0x21,
    WRITE_C8_D8, 0xea, 0x12,
    WRITE_C8_D8, 0xeb, 0xd0,
    WRITE_C8_D8, 0xec, 0x04,
    WRITE_C8_D8, 0xed, 0x07,
    WRITE_C8_D8, 0xee, 0x07,
    WRITE_C8_D8, 0xef, 0x09,
    WRITE_C8_D8, 0xF0, 0xD0,
    WRITE_C8_D8, 0xF1, 0x0E,
    WRITE_C8_D8, 0xF9, 0x56,
    WRITE_COMMAND_8, 0xf2,
    WRITE_BYTES, 4, 0x26, 0x1b, 0x0b, 0x20,
    WRITE_C8_D8, 0xec, 0x04,
    WRITE_C8_D8, 0x35, 0x00,
    WRITE_C8_D16, 0x44, 0x00, 0x10,
    WRITE_C8_D8, 0x46, 0x10,
    WRITE_C8_D8, 0xff, 0x00,
    WRITE_C8_D8, 0x3a, 0x05,
    WRITE_COMMAND_8, NV3007_SLPOUT,
    END_WRITE,

    DELAY, 200,

    BEGIN_WRITE,
    WRITE_COMMAND_8, NV3007_DISPON,
    END_WRITE,

    DELAY, 150};

class Arduino_NV3007 : public Arduino_TFT
{
public:
  Arduino_NV3007(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = NV3007_TFTWIDTH, int16_t h = NV3007_TFTHEIGHT,
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
