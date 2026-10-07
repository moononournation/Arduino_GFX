/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HX8347G_H_
#define _ARDUINO_HX8347G_H_

#include "../Arduino_GFX.h"
#include "../Arduino_TFT.h"

#define HX8347G_TFTWIDTH 240  ///< HX8347G max TFT width
#define HX8347G_TFTHEIGHT 320 ///< HX8347G max TFT height

#define HX8347G_RST_DELAY 120

#define HX8347G_DISPLAY_MODE_CONTROL 0x01 // Display Mode control

#define HX8347G_INV_OFF 0x00 // INV_ON disable
#define HX8347G_INV_ON 0x02  // INV_ON enable

static const uint8_t hx8347g_init_operations[] = {
    BEGIN_WRITE,
    // Power Voltage Setting
    WRITE_C8_D8, 0xEA, 0x00,
    WRITE_C8_D8, 0xEB, 0x20,
    WRITE_C8_D8, 0xEC, 0x0C,
    WRITE_C8_D8, 0xED, 0xC4,
    WRITE_C8_D8, 0xE8, 0x40,
    WRITE_C8_D8, 0xE9, 0x38,
    WRITE_C8_D8, 0x27, 0xA3,
    WRITE_C8_D8, 0x1B, 0x1B,
    WRITE_C8_D8, 0x1A, 0x01,
    WRITE_C8_D8, 0x24, 0x98,
    WRITE_C8_D8, 0x25, 0x57,
    WRITE_C8_D8, 0x23, 0x40,
    WRITE_C8_D8, 0x18, 0x36,
    WRITE_C8_D8, 0x19, 0x01,
    WRITE_C8_D8, 0x1C, 0x06,
    WRITE_C8_D8, 0x1F, 0x90,
    DELAY, 10,
    WRITE_C8_D8, 0x17, 0x05,
    WRITE_C8_D8, 0x36, 0x08,
    WRITE_C8_D8, 0x28, 0x38,
    DELAY, 60,
    WRITE_C8_D8, 0x28, 0x3C,
    WRITE_C8_D8, 0x01, 0x00,
    WRITE_C8_D8, 0x16, 0xA8,
    WRITE_C8_D8, 0x02, 0x00,
    WRITE_C8_D8, 0x03, 0x00,
    WRITE_C8_D8, 0x04, 0x01,
    WRITE_C8_D8, 0x05, 0x3F,
    WRITE_C8_D8, 0x06, 0x00,
    WRITE_C8_D8, 0x07, 0x00,
    WRITE_C8_D8, 0x08, 0x00,
    WRITE_C8_D8, 0x09, 0xEF,
    END_WRITE};

class Arduino_HX8347G : public Arduino_TFT
{
public:
  Arduino_HX8347G(
      Arduino_DataBus *bus, int8_t rst = GFX_NOT_DEFINED, uint8_t r = 0,
      bool ips = false, int16_t w = HX8347G_TFTWIDTH, int16_t h = HX8347G_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  bool begin(int32_t speed = GFX_NOT_DEFINED) override;
  void writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h) override;
  void setRotation(uint8_t r) override;
  void invertDisplay(bool) override;
  void displayOn() override;
  void displayOff() override;

protected:
  void tftInit() override;
  bool _invert = false;

private:
};

#endif
