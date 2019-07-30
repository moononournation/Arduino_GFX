/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT_18bit.h"

Arduino_TFT_18bit::Arduino_TFT_18bit(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

inline void Arduino_TFT_18bit::writeColor(uint16_t color)
{
  _bus->write((color & 0xF800) >> 8);
  _bus->write((color & 0x07E0) >> 3);
  _bus->write((color & 0x001F) << 3);
}

void Arduino_TFT_18bit::writePixels(uint16_t color, uint32_t len)
{
#if defined(ESP8266) || defined(ESP32)
  uint8_t c[3] = {(color & 0xF800) >> 8, (color & 0x07E0) >> 3, (color & 0x001F) << 3};
  _bus->writePattern(c, 3, len);
#else
  uint8_t c1 = (color & 0xF800) >> 8;
  uint8_t c2 = (color & 0x07E0) >> 3;
  uint8_t c3 = (color & 0x001F) << 3;
  while (len--) {
    _bus->write(c1);
    _bus->write(c2);
    _bus->write(c3);
  }
#endif
}
