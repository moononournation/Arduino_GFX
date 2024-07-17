#include "Arduino_ST77916.h"

Arduino_ST77916::Arduino_ST77916(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

bool Arduino_ST77916::begin(int32_t speed)
{
  return Arduino_TFT::begin(speed);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ST77916::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = ST77916_MADCTL_MX | ST77916_MADCTL_MV | ST77916_MADCTL_RGB;
    break;
  case 2:
    r = ST77916_MADCTL_MX | ST77916_MADCTL_MY | ST77916_MADCTL_RGB;
    break;
  case 3:
    r = ST77916_MADCTL_MY | ST77916_MADCTL_MV | ST77916_MADCTL_RGB;
    break;
  default: // case 0:
    r = ST77916_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(ST77916_MADCTL, r);
  _bus->endWrite();
}

void Arduino_ST77916::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16(ST77916_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16(ST77916_RASET, y, y + h - 1);
  }

  _bus->writeCommand(ST77916_RAMWR); // write to RAM
}

void Arduino_ST77916::invertDisplay(bool i)
{
  _bus->sendCommand((_ips ^ i) ? ST77916_INVON : ST77916_INVOFF);
}

void Arduino_ST77916::displayOn(void)
{
  _bus->sendCommand(ST77916_SLPOUT);
  delay(ST77916_SLPOUT_DELAY);
}

void Arduino_ST77916::displayOff(void)
{
  _bus->sendCommand(ST77916_SLPIN);
  delay(ST77916_SLPIN_DELAY);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ST77916::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(ST77916_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(ST77916_RST_DELAY);
  }
  else
  {
    // Software Rest
    _bus->sendCommand(ST77916_SWRESET);
    delay(ST77916_RST_DELAY);
  }

  _bus->batchOperation(st77916_init_operations, sizeof(st77916_init_operations));

  invertDisplay(false);
}
