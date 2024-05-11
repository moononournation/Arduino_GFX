#include "Arduino_RM690B0.h"
#include "SPI.h"

Arduino_RM690B0::Arduino_RM690B0(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, false, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

bool Arduino_RM690B0::begin(int32_t speed)
{
  return Arduino_TFT::begin(speed);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_RM690B0::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = RM690B0_MADCTL_MX | RM690B0_MADCTL_MV | RM690B0_MADCTL_RGB;
    break;
  case 2:
    r = RM690B0_MADCTL_MX | RM690B0_MADCTL_MY | RM690B0_MADCTL_RGB;
    break;
  case 3:
    r = RM690B0_MADCTL_MV | RM690B0_MADCTL_MY | RM690B0_MADCTL_RGB;
    break;
  default: // case 0:
    r = RM690B0_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(RM690B0_MADCTL, r);
  _bus->endWrite();
}

void Arduino_RM690B0::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16(RM690B0_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16(RM690B0_PASET, y, y + h - 1);
  }

  _bus->writeCommand(RM690B0_RAMWR); // write to RAM
}

void Arduino_RM690B0::invertDisplay(bool i)
{
  _bus->sendCommand((_ips ^ i) ? RM690B0_INVON : RM690B0_INVOFF);
}

void Arduino_RM690B0::displayOn(void)
{
  _bus->sendCommand(RM690B0_SLPOUT);
  delay(RM690B0_SLPOUT_DELAY);
}

void Arduino_RM690B0::displayOff(void)
{
  _bus->sendCommand(RM690B0_SLPIN);
  delay(RM690B0_SLPIN_DELAY);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_RM690B0::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(RM690B0_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(RM690B0_RST_DELAY);
  }
  else
  {
    // Software Rest
    _bus->sendCommand(RM690B0_SWRESET);
    delay(RM690B0_RST_DELAY);
  }

  _bus->batchOperation(rm690b0_init_operations, sizeof(rm690b0_init_operations));

  invertDisplay(false);
}
