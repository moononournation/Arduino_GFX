/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/adafruit/Adafruit_ILI9341.git
 */
#include "Arduino_ILI9341_2.h"

Arduino_ILI9341_2::Arduino_ILI9341_2(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips)
    : Arduino_ILI9341(bus, rst, r, ips)
{
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9341_2::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(ILI9341_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(ILI9341_RST_DELAY);
  }
  else
  {
    // Software Rest
    _bus->sendCommand(ILI9341_SWRESET);
    delay(ILI9341_RST_DELAY);
  }

  _bus->batchOperation(ili9341_2_init_operations, sizeof(ili9341_2_init_operations));
  
  invertDisplay(true);
}
