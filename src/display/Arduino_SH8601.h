#pragma once

#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"
#include "../Arduino_OLED.h"

#define SH8601_TFTWIDTH 480  ///< SH8601 max TFT width
#define SH8601_TFTHEIGHT 480 ///< SH8601 max TFT width

#define SH8601_RST_DELAY 200    ///< delay ms wait for reset finish
#define SH8601_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define SH8601_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

// User Command
#define SH8601_C_NOP 0x00          // nop
#define SH8601_C_SWRESET 0x01      // Software Reset
#define SH8601_R_RDID 0x04         // Read Display Identification Information ID/1/2/3
#define SH8601_R_RDNERRORSDSI 0x05 // Read Number of Errors on DSI
#define SH8601_R_RDPOWERMODE 0x0A  // Read Display Power Mode
#define SH8601_R_RDMADCTL 0x0B     // Read Display MADCTL
#define SH8601_R_RDPIXFMT 0x0C     // Read Display Pixel Format
#define SH8601_R_RDIMGFMT 0x0D     // Read Display Image Mode
#define SH8601_R_RDSIGMODE 0x0E    // Read Display Signal Mode
#define SH8601_R_RDSELFDIAG 0x0F   // Read Display Self-Diagnostic Result

#define SH8601_C_SLPIN 0x10  // Sleep In
#define SH8601_C_SLPOUT 0x11 // Sleep Out
#define SH8601_C_PTLON 0x12  // Partial Display On
#define SH8601_C_NORON 0x13  // Normal Display mode on

#define SH8601_C_INVOFF 0x20  // Inversion Off
#define SH8601_C_INVON 0x21   // Inversion On
#define SH8601_C_ALLPOFF 0x22 // All pixels off
#define SH8601_C_ALLPON 0x23  // All pixels on
#define SH8601_C_DISPOFF 0x28 // Display off
#define SH8601_C_DISPON 0x29  // Display on
#define SH8601_W_CASET 0x2A   // Column Address Set
#define SH8601_W_PASET 0x2B   // Page Address Set
#define SH8601_W_RAMWR 0x2C   // Memory Write Start

#define SH8601_W_PTLAR 0x30   // Partial Area Row Set
#define SH8601_W_PTLAC 0x31   // Partial Area Column Set
#define SH8601_C_TEAROFF 0x34 // Tearing effect off
#define SH8601_WC_TEARON 0x35 // Tearing effect on
#define SH8601_W_MADCTL 0x36  // Memory data access control
#define SH8601_C_IDLEOFF 0x38 // Idle Mode Off
#define SH8601_C_IDLEON 0x39  // Idle Mode On
#define SH8601_W_PIXFMT 0x3A  // Write Display Pixel Format
#define SH8601_W_WRMC 0x3C    // Memory Write Continue

#define SH8601_W_SETTSL 0x44             // Write Tearing Effect Scan Line
#define SH8601_R_GETSL 0x45              // Read Scan Line Number
#define SH8601_C_SPIROFF 0x46            // SPI read Off
#define SH8601_C_SPIRON 0x47             // SPI read On
#define SH8601_C_AODMOFF 0x48            // AOD Mode Off
#define SH8601_C_AODMON 0x49             // AOD Mode On
#define SH8601_W_WDBRIGHTNESSVALAOD 0x4A // Write Display Brightness Value in AOD Mode
#define SH8601_R_RDBRIGHTNESSVALAOD 0x4B // Read Display Brightness Value in AOD Mode
#define SH8601_W_DEEPSTMODE 0x4F         // Deep Standby Mode On

#define SH8601_W_WDBRIGHTNESSVALNOR 0x51 // Write Display Brightness Value in Normal Mode
#define SH8601_R_RDBRIGHTNESSVALNOR 0x52 // Read display brightness value in Normal Mode
#define SH8601_W_WCTRLD1 0x53            // Write CTRL Display1
#define SH8601_R_RCTRLD1 0x54            // Read CTRL Display1
#define SH8601_W_WCTRLD2 0x55            // Write CTRL Display2
#define SH8601_R_RCTRLD2 0x56            // Read CTRL Display2
#define SH8601_W_WCE 0x58                // Write CE
#define SH8601_R_RCE 0x59                // Read CE

#define SH8601_W_WDBRIGHTNESSVALHBM 0x63 // Write Display Brightness Value in HBM Mode
#define SH8601_R_WDBRIGHTNESSVALHBM 0x64 // Read Display Brightness Value in HBM Mode
#define SH8601_W_WHBMCTL 0x66            // Write HBM Control

#define SH8601_W_COLORSET0 0x70  // Color Set 0
#define SH8601_W_COLORSET1 0x71  // Color Set 1
#define SH8601_W_COLORSET2 0x72  // Color Set 2
#define SH8601_W_COLORSET3 0x73  // Color Set 3
#define SH8601_W_COLORSET4 0x74  // Color Set 4
#define SH8601_W_COLORSET5 0x75  // Color Set 5
#define SH8601_W_COLORSET6 0x76  // Color Set 6
#define SH8601_W_COLORSET7 0x77  // Color Set 7
#define SH8601_W_COLORSET8 0x78  // Color Set 8
#define SH8601_W_COLORSET9 0x79  // Color Set 9
#define SH8601_W_COLORSET10 0x7A // Color Set 10
#define SH8601_W_COLORSET11 0x7B // Color Set 11
#define SH8601_W_COLORSET12 0x7C // Color Set 12
#define SH8601_W_COLORSET13 0x7D // Color Set 13
#define SH8601_W_COLORSET14 0x7E // Color Set 14
#define SH8601_W_COLORSET15 0x7F // Color Set 15

#define SH8601_W_COLOROPTION 0x80 // Color Option

#define SH8601_R_RDDBSTART 0xA1         // Read DDB start
#define SH8601_R_DDBCONTINUE 0xA8       // Read DDB Continue
#define SH8601_R_RFIRCHECKSUN 0xAA      // Read First Checksum
#define SH8601_R_RCONTINUECHECKSUN 0xAF // Read Continue Checksum

#define SH8601_W_SPIMODECTL 0xC4 // SPI mode control

#define SH8601_R_RDID1 0xDA // Read ID1
#define SH8601_R_RDID2 0xDB // Read ID2
#define SH8601_R_RDID3 0xDC // Read ID3

// Flip
#define SH8601_MADCTL_X_AXIS_FLIP 0x02 // Flip Horizontal
#define SH8601_MADCTL_Y_AXIS_FLIP 0x05 // Flip Vertical

// Color Order
#define SH8601_MADCTL_RGB 0x00                      // Red-Green-Blue pixel order
#define SH8601_MADCTL_BGR 0x08                      // Blue-Green-Red pixel order
#define SH8601_MADCTL_COLOR_ORDER SH8601_MADCTL_RGB // RGB

enum
{
  SH8601_ContrastOff = 0,
  SH8601_LowContrast,
  SH8601_MediumContrast,
  SH8601_HighContrast
};

static const uint8_t sh8601_init_operations[] = {

    BEGIN_WRITE,

    WRITE_COMMAND_8, SH8601_C_SLPOUT,

    END_WRITE,

    DELAY, SH8601_SLPOUT_DELAY,

    BEGIN_WRITE,

    // WRITE_C8_D8, SH8601_WC_TEARON, 0x00,

    WRITE_COMMAND_8, SH8601_C_NORON,

    WRITE_COMMAND_8, SH8601_C_INVOFF,

    // WRITE_COMMAND_8, SH8601_C_ALLPON,

    // WRITE_C8_D8, SH8601_W_MADCTL, SH8601_MADCTL_COLOR_ORDER, // RGB/BGR

    WRITE_C8_D8, SH8601_W_PIXFMT, 0x05, // Interface Pixel Format 16bit/pixel
    // WRITE_C8_D8, SH8601_W_PIXFMT, 0x06, // Interface Pixel Format 18bit/pixel
    // WRITE_C8_D8, SH8601_W_PIXFMT, 0x07, // Interface Pixel Format 24bit/pixel

    WRITE_COMMAND_8, SH8601_C_DISPON,

    // WRITE_COMMAND_8, SH8601_W_WDBRIGHTNESSVALNOR,
    // WRITE_BYTES, 2,
    // 0x03, 0xFF,

    WRITE_C8_D8, SH8601_W_WCTRLD1, 0x28, // Brightness Control On and Display Dimming On

    WRITE_C8_D8, SH8601_W_WDBRIGHTNESSVALNOR, 0xD0, // Brightness adjustment

    // High contrast mode (Sunlight Readability Enhancement)
    WRITE_C8_D8, SH8601_W_WCE, 0x00, // Off
    // WRITE_C8_D8, SH8601_W_WCE, 0x05, // On Low
    // WRITE_C8_D8, SH8601_W_WCE, 0x06, // On Medium
    // WRITE_C8_D8, SH8601_W_WCE, 0x07, // On High

    END_WRITE,

    DELAY, 10};

class Arduino_SH8601 : public Arduino_OLED
{
public:
  Arduino_SH8601(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      int16_t w = SH8601_TFTWIDTH, int16_t h = SH8601_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;
  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;
  void setRotation(uint8_t r) override;
  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

  void setBrightness(uint8_t brightness) override;
  void setContrast(uint8_t contrast) override;

protected:
  void tftInit() override;

private:
};
