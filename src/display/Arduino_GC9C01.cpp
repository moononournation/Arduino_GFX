#include "Arduino_GC9C01.h"
#include "SPI.h"

Arduino_GC9C01::Arduino_GC9C01(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

bool Arduino_GC9C01::begin(int32_t speed)
{
  _override_datamode = SPI_MODE0; // always use SPI_MODE0

  return Arduino_TFT::begin(speed);
}

void Arduino_GC9C01::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW) || (y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(GC9C01_CASET, x + _xStart, x + w - 1 + _xStart);
    _bus->writeC8D16D16(GC9C01_RASET, y + _yStart, y + h - 1 + _yStart);

    _currentX = x;
    _currentY = y;
    _currentW = w;
    _currentH = h;
  }

  _bus->writeCommand(GC9C01_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_GC9C01::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
    case 1:
    r = GC9C01_MADCTL_MX | GC9C01_MADCTL_MV | GC9C01_MADCTL_RGB;
    break;
  case 2:
    r = GC9C01_MADCTL_MX | GC9C01_MADCTL_MY | GC9C01_MADCTL_RGB;
    break;
  case 3:
    r = GC9C01_MADCTL_MY | GC9C01_MADCTL_MV | GC9C01_MADCTL_RGB;
    break;
  case 4:
    r = GC9C01_MADCTL_MX | GC9C01_MADCTL_RGB;
    break;
  case 5:
    r = GC9C01_MADCTL_MX | GC9C01_MADCTL_MY | GC9C01_MADCTL_MV | GC9C01_MADCTL_RGB;
    break;
  case 6:
    r = GC9C01_MADCTL_MY | GC9C01_MADCTL_RGB;
    break;
  case 7:
    r = GC9C01_MADCTL_MV | GC9C01_MADCTL_RGB;
    break;
  default: // case 0:
    r = GC9C01_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(GC9C01_MADCTL, r);
  _bus->endWrite();
}

void Arduino_GC9C01::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? GC9C01_INVOFF : GC9C01_INVON) : (i ? GC9C01_INVON : GC9C01_INVOFF));
}

void Arduino_GC9C01::displayOn(void)
{
  _bus->sendCommand(GC9C01_SLPOUT);
  delay(GC9C01_SLPOUT_DELAY);
}

void Arduino_GC9C01::displayOff(void)
{
  _bus->sendCommand(GC9C01_SLPIN);
  delay(GC9C01_SLPIN_DELAY);
}

void Arduino_GC9C01::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(GC9C01_RST_DELAY);
    digitalWrite(_rst, LOW);
    delay(GC9C01_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(GC9C01_RST_DELAY);
  }
  else
  {
    // Software Rest
  }

  _bus->batchOperation(gc9c01_init_operations, sizeof(gc9c01_init_operations));

  invertDisplay(false);
}
