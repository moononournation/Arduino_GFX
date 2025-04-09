#pragma once

#include "./Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define GC9C01_TFTWIDTH 360
#define GC9C01_TFTHEIGHT 360

#define GC9C01_RST_DELAY 200    ///< delay ms wait for reset finish
#define GC9C01_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define GC9C01_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define GC9C01_NOP 0x00
#define GC9C01_SWRESET 0x01
#define GC9C01_RDDID 0x04
#define GC9C01_RDDST 0x09

#define GC9C01_SLPIN 0x10
#define GC9C01_SLPOUT 0x11
#define GC9C01_PTLON 0x12
#define GC9C01_NORON 0x13

#define GC9C01_INVOFF 0x20
#define GC9C01_INVON 0x21
#define GC9C01_DISPOFF 0x28
#define GC9C01_DISPON 0x29

#define GC9C01_CASET 0x2A
#define GC9C01_RASET 0x2B
#define GC9C01_RAMWR 0x2C
#define GC9C01_RAMRD 0x2E

#define GC9C01_PTLAR 0x30
#define GC9C01_COLMOD 0x3A
#define GC9C01_MADCTL 0x36

#define GC9C01_MADCTL_MY 0x80
#define GC9C01_MADCTL_MX 0x40
#define GC9C01_MADCTL_MV 0x20
#define GC9C01_MADCTL_ML 0x10
#define GC9C01_MADCTL_RGB 0x00

#define GC9C01_RDID1 0xDA
#define GC9C01_RDID2 0xDB
#define GC9C01_RDID3 0xDC
#define GC9C01_RDID4 0xDD

static const uint8_t gc9c01_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xfe,   // internal reg enable
    WRITE_COMMAND_8, 0xef,   // internal reg enable
    WRITE_C8_D8, 0x80, 0x11, // reg_en for 70\74
    WRITE_C8_D8, 0x81, 0x70, // reg_en for 7C\7D\7E
    WRITE_C8_D8, 0x82, 0x09, // reg_en for 90\93
    WRITE_C8_D8, 0x83, 0x03, // reg_en for 98\99
    WRITE_C8_D8, 0x84, 0x20, // reg_en for B5
    WRITE_C8_D8, 0x85, 0x42, // reg_en for B9\BE
    WRITE_C8_D8, 0x86, 0xfc, // reg_en for C2~7
    WRITE_C8_D8, 0x87, 0x09, // reg_en for C8\CB
    WRITE_C8_D8, 0x89, 0x10, // reg_en for EC
    WRITE_C8_D8, 0x8A, 0x4f, // reg_en for F0~3\F6
    WRITE_C8_D8, 0x8C, 0x59, // reg_en for 60\63\64\66
    WRITE_C8_D8, 0x8D, 0x51, // reg_en for 68\6C\6E
    WRITE_C8_D8, 0x8E, 0xae, // reg_en for A1~3\A5\A7
    WRITE_C8_D8, 0x8F, 0xf3, // reg_en for AC~F\A8\A9
    WRITE_C8_D8, 0x36, 0x00,
    WRITE_C8_D8, 0x3a, 0x05, // 565 frame
    WRITE_C8_D8, 0xEC, 0x77,

    WRITE_C8_BYTES, 0x74, 6, // rtn 60Hz
    0x01, 0x80, 0x00, 0x00,
    0x00, 0x00,

    WRITE_C8_D8, 0x98, 0x3E,
    WRITE_C8_D8, 0x99, 0x3E,
    WRITE_C8_D8, 0xC3, 0x2A,
    WRITE_C8_D8, 0xC4, 0x18,
    WRITE_C8_D16, 0xA1, 0x01, 0x04, // SRAM RD OPTION
    WRITE_C8_D16, 0xA2, 0x01, 0x04, // SRAM RD OPTION
    WRITE_C8_D8, 0xA9, 0x1C,        // IREF=9.8uA
    WRITE_C8_D16, 0xA5, 0x11, 0x09, // VDDML=1.24V
    WRITE_C8_D8, 0xB9, 0x8A,        // RTERM=101O
    WRITE_C8_D8, 0xA8, 0x5E,        // VBG_BUF=1.003V, DVDD=1.543V
    WRITE_C8_D8, 0xA7, 0x40,        // BIAS=10.2uA
    WRITE_C8_D8, 0xAF, 0x73,        // VDDSOU=1.715V ,VDDGM=2.002V
    WRITE_C8_D8, 0xAE, 0x44,        // VREE=2.475V,VRDD=2.335V
    WRITE_C8_D8, 0xAD, 0x38,        // VRGL=1.635V ,VDDSF=2.018V
    WRITE_C8_D8, 0xA3, 0x5D,        // OSC=53.7MHz
    WRITE_C8_D8, 0xC2, 0x02,        // VREG_VREF=2.805V
    WRITE_C8_D8, 0xC5, 0x11,        // VREG1A=5.99V
    WRITE_C8_D8, 0xC6, 0x0E,        // VREG1B=1.505V
    WRITE_C8_D8, 0xC7, 0x13,        // VREG2A=-2.995V
    WRITE_C8_D8, 0xC8, 0x0D,        // VREG2B=1.497V
    WRITE_C8_D8, 0xCB, 0x02,        // 6.09V
    WRITE_C8_D16, 0x7C, 0xB6, 0x26, // 13.12V
    WRITE_C8_D8, 0xAC, 0x24,        // VGLO=-8.35V
    WRITE_C8_D8, 0xF6, 0x80,        // EPF=2
    WRITE_C8_D16, 0xB5, 0x09, 0x09, // VBP

    WRITE_C8_BYTES, 0x60, 4, // STV1&2
    0x38, 0x0B, 0x5B, 0x56,

    WRITE_C8_BYTES, 0x63, 4, // STV3&4
    0x3A, 0xE0, 0x5B, 0x56,  // MAX=0x61

    WRITE_C8_BYTES, 0x64, 6, // CLK_group1
    0x38, 0x0D, 0x72, 0xDD,
    0x5B, 0x56,

    WRITE_C8_BYTES, 0x66, 6, // CLK_group1
    0x38, 0x11, 0x72, 0xE1,
    0x5B, 0x56,

    WRITE_C8_BYTES, 0x68, 7, // FLC&FLV 1~2
    0x3B, 0x08, 0x08, 0x00,
    0x08, 0x29, 0x5B,

    WRITE_C8_BYTES, 0x6E, 32, // gout_Mapping
    0x00, 0x00, 0x00, 0x07,   // gout4_swap_fw[4:0]
    0x01, 0x13, 0x11, 0x0B,   // gout8_swap_fw[4:0]
    0x09, 0x16, 0x15, 0x1D,   // gout12_swap_fw[4:0]
    0x1E, 0x00, 0x00, 0x00,   // gout16_swap_fw[4:0]
    0x00, 0x00, 0x00, 0x1E,   // gout20_swap_fw[4:0]
    0x1D, 0x15, 0x16, 0x0A,   // gout24_swap_fw[4:0]
    0x0C, 0x12, 0x14, 0x02,   // gout28_swap_fw[4:0]
    0x08, 0x00, 0x00, 0x00,   // gout32_swap_fw[4:0]

    WRITE_C8_D8, 0xBE, 0x11, // SOU_BIAS_FIX=1

    WRITE_C8_BYTES, 0x6C, 7, // precharge GATE
    0xCC, 0x0C, 0xCC, 0x84,
    0xCC, 0x04, 0x50,

    WRITE_C8_D8, 0x7D, 0x72,
    WRITE_C8_D8, 0x7E, 0x38, // VGL_BT=1 5X  (BT=0:6X)  RT=0

    WRITE_C8_BYTES, 0x70, 10,
    0x02, 0x03, 0x09, 0x05, // vgh_clk
    0x0C, 0x06, 0x09, 0x05, // vgh_clk_porch  0E
    0x0C, 0x06,             // vcl_clk_porch  0E

    WRITE_C8_BYTES, 0x90, 4,
    0x06, 0x06, 0x05, 0x06, // bvdd_clk1_ad1

    WRITE_C8_BYTES, 0x93, 3,
    0x45, 0xFF, 0x00,

    // gamma start
    WRITE_C8_BYTES, 0xF0, 6,
    0x45, 0x09, 0x08, 0x08,
    0x26, 0x2A,

    WRITE_C8_BYTES, 0xF1, 6,
    0x43, 0x70, 0x72, 0x36,
    0x37, 0x6F,

    WRITE_C8_BYTES, 0xF2, 6,
    0x45, 0x09, 0x08, 0x08,
    0x26, 0x2A,

    WRITE_C8_BYTES, 0xF3, 6,
    0x43, 0x70, 0x72, 0x36,
    0x37, 0x6F,

    WRITE_COMMAND_8, GC9C01_SLPOUT,
    END_WRITE,

    DELAY, GC9C01_SLPOUT_DELAY,

    BEGIN_WRITE,
    WRITE_COMMAND_8, GC9C01_DISPON, // Display on
    END_WRITE,

    DELAY, 10};

class Arduino_GC9C01 : public Arduino_TFT
{
public:
  Arduino_GC9C01(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = GC9C01_TFTWIDTH, int16_t h = GC9C01_TFTHEIGHT,
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
