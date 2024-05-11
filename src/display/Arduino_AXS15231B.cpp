/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/ananevilya/Arduino-AXS15231B-Library.git
 */
#include "Arduino_AXS15231B.h"

Arduino_AXS15231B::Arduino_AXS15231B(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

bool Arduino_AXS15231B::begin(int32_t speed)
{
  if (speed == GFX_NOT_DEFINED)
  {
    speed = 32000000UL; // AXS15231B Maximum supported speed
  }
  return Arduino_TFT::begin(speed);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_AXS15231B::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = AXS15231B_MADCTL_MX | AXS15231B_MADCTL_MV | AXS15231B_MADCTL_RGB;
    break;
  case 2:
    r = AXS15231B_MADCTL_MX | AXS15231B_MADCTL_MY | AXS15231B_MADCTL_RGB;
    break;
  case 3:
    r = AXS15231B_MADCTL_MY | AXS15231B_MADCTL_MV | AXS15231B_MADCTL_RGB;
    break;
  default: // case 0:
    r = AXS15231B_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(AXS15231B_MADCTL, r);
  _bus->endWrite();
}

void Arduino_AXS15231B::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16(AXS15231B_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16(AXS15231B_RASET, y, y + h - 1);
  }

  _bus->writeCommand(AXS15231B_RAMWR); // write to RAM
}

void Arduino_AXS15231B::invertDisplay(bool i)
{
  _bus->sendCommand((_ips ^ i) ? AXS15231B_INVON : AXS15231B_INVOFF);
}

void Arduino_AXS15231B::displayOn(void)
{
  _bus->sendCommand(AXS15231B_SLPOUT);
  delay(AXS15231B_SLPOUT_DELAY);
}

void Arduino_AXS15231B::displayOff(void)
{
  _bus->sendCommand(AXS15231B_SLPIN);
  delay(AXS15231B_SLPIN_DELAY);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_AXS15231B::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(AXS15231B_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(AXS15231B_RST_DELAY);
  }
  // else
  // {
  // Software Rest
  _bus->sendCommand(AXS15231B_SWRESET);
  delay(AXS15231B_RST_DELAY);
  // }

  _bus->batchOperation(axs15231b_init_operations, sizeof(axs15231b_init_operations));

  invertDisplay(false);
}
