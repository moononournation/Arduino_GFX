#include "Arduino_NV3007_alternate.h"

// Alternate reset sequence and rotation commands based on example "lcd.h" code from:
// https://www.buydisplay.com/8051/ER-TFT2.79-1_8051_Tutorial.zip
// from
// https://www.buydisplay.com/wide-angle-2-79-inch-142x428-color-ips-tft-display-nv3007-controller

Arduino_NV3007_alternate::Arduino_NV3007_alternate(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_NV3007(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_NV3007_alternate::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(5);
    digitalWrite(_rst, LOW);
    delay(10);
    digitalWrite(_rst, HIGH);
    delay(50);
  }
  else
  {
    // Software Rest
  }

  _bus->batchOperation(NV3007_alternate_init_operations, sizeof(NV3007_alternate_init_operations));

  invertDisplay(false);
}

void Arduino_NV3007_alternate::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  uint8_t madctl_val = 0x00;
  switch (_rotation)
  {
  case 1:
    madctl_val = 0x60;
    break;
  case 2:
    madctl_val = 0xC0;
    break;
  case 3:
    madctl_val = 0xA0;
    break;
  default: // case 0:
    madctl_val = 0x00;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(NV3007_MADCTL, madctl_val);
  _bus->endWrite();
}
