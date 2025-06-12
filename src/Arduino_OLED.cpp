#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT.h"
#include "Arduino_OLED.h"

Arduino_OLED::Arduino_OLED(
    Arduino_DataBus *bus, int8_t rst, uint8_t r, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(
          bus, rst, r, false /* ips */, w, h,
          col_offset1, row_offset1, col_offset2, row_offset2)
{
}
