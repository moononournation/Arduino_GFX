/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/adafruit/Adafruit_ILI9341.git
 */
#ifndef _ARDUINO_ILI9341_2_H_
#define _ARDUINO_ILI9341_2_H_

#include "Arduino_ILI9341.h"

static const uint8_t ili9341_2_init_operations[] = {
    BEGIN_WRITE,
    WRITE_COMMAND_8, 0xCF,
    WRITE_BYTES, 3, 0x00, 0xC1, 0x30,
    WRITE_COMMAND_8, 0xED,
    WRITE_BYTES, 4, 0x64, 0x03, 0x12, 0x81,
    WRITE_COMMAND_8, 0xE8,
    WRITE_BYTES, 3, 0x85, 0x00, 0x78,
    WRITE_COMMAND_8, 0xCB,
    WRITE_BYTES, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
    WRITE_C8_D8, 0xF7, 0x20,
    WRITE_C8_D16, 0xEA, 0x00, 0x00,
    WRITE_C8_D8, ILI9341_PWCTR1, 0x10,        // Power control VRH[5:0]
    WRITE_C8_D8, ILI9341_PWCTR2, 0x00,        // Power control SAP[2:0];BT[3:0]
    WRITE_C8_D16, ILI9341_VMCTR1, 0x30, 0x30, // VCM control
    WRITE_C8_D8, ILI9341_VMCTR2, 0xB7,        // VCM control2
    WRITE_C8_D8, ILI9341_PIXFMT, 0x55,
    WRITE_C8_D16, ILI9341_FRMCTR1, 0x00, 0x1A,
    WRITE_COMMAND_8, ILI9341_DFUNCTR,         // Display Function Control
    WRITE_BYTES, 3, 0x08, 0x82, 0x27,
    WRITE_C8_D8, 0xF2, 0x00,                    // 3Gamma Function Disable
    WRITE_C8_D8, ILI9341_GAMMASET, 0x01,      // Gamma curve selected
    WRITE_COMMAND_8, ILI9341_GMCTRP1,         // Set Gamma
    WRITE_BYTES, 15,
    0x0F, 0x31, 0x2B, 0x0C,
    0x0E, 0x08, 0x4E, 0xF1,
    0x37, 0x07, 0x10, 0x03,
    0x0E, 0x09, 0x00,
    WRITE_COMMAND_8, ILI9341_GMCTRN1, // Set Gamma
    WRITE_BYTES, 15,
    0x00, 0x0E, 0x14, 0x03,
    0x11, 0x07, 0x31, 0xC1,
    0x48, 0x08, 0x0F, 0x0C,
    0x31, 0x36, 0x0F,
    WRITE_COMMAND_8, ILI9341_PASET,
    WRITE_BYTES, 4, 0x00, 0x00, 0x01, 0x3F,
    WRITE_COMMAND_8, ILI9341_CASET,
    WRITE_BYTES, 4, 0x00, 0x00, 0x01, 0xEF,
    WRITE_COMMAND_8, ILI9341_SLPOUT, // Exit Sleep
    WRITE_COMMAND_8, ILI9341_DISPON, // Display on
    END_WRITE};

class Arduino_ILI9341_2 : public Arduino_ILI9341
{
public:
  Arduino_ILI9341_2(Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0, bool ips = false);

protected:
  void tftInit() override;

private:
};

#endif
