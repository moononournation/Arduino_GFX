#include "Arduino_NV3007.h"
#include "SPI.h"

Arduino_NV3007::Arduino_NV3007(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2,
    const uint8_t *init_operations, size_t init_operations_len)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2),
      _init_operations(init_operations), _init_operations_len(init_operations_len)
{
}

bool Arduino_NV3007::begin(int32_t speed)
{
  _override_datamode = SPI_MODE0; // always use SPI_MODE0

  return Arduino_TFT::begin(speed);
}

void Arduino_NV3007::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW) || (y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(NV3007_CASET, x + _xStart, x + w - 1 + _xStart);
    _bus->writeC8D16D16(NV3007_RASET, y + _yStart, y + h - 1 + _yStart);

    _currentX = x;
    _currentY = y;
    _currentW = w;
    _currentH = h;
  }

  _bus->writeCommand(NV3007_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_NV3007::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = NV3007_MADCTL_MX | NV3007_MADCTL_MV | NV3007_MADCTL_RGB;
    break;
  case 2:
    r = NV3007_MADCTL_MX | NV3007_MADCTL_MY | NV3007_MADCTL_RGB;
    break;
  case 3:
    r = NV3007_MADCTL_MY | NV3007_MADCTL_MV | NV3007_MADCTL_RGB;
    break;
  case 4:
    r = NV3007_MADCTL_MX | NV3007_MADCTL_RGB;
    break;
  case 5:
    r = NV3007_MADCTL_MX | NV3007_MADCTL_MY | NV3007_MADCTL_MV | NV3007_MADCTL_RGB;
    break;
  case 6:
    r = NV3007_MADCTL_MY | NV3007_MADCTL_RGB;
    break;
  case 7:
    r = NV3007_MADCTL_MV | NV3007_MADCTL_RGB;
    break;
  default: // case 0:
    r = NV3007_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(NV3007_MADCTL, r);
  _bus->endWrite();
}

void Arduino_NV3007::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? NV3007_INVOFF : NV3007_INVON) : (i ? NV3007_INVON : NV3007_INVOFF));
}

void Arduino_NV3007::displayOn(void)
{
  _bus->sendCommand(NV3007_SLPOUT);
  delay(NV3007_SLPOUT_DELAY);
}

void Arduino_NV3007::displayOff(void)
{
  _bus->sendCommand(NV3007_SLPIN);
  delay(NV3007_SLPIN_DELAY);
}

void Arduino_NV3007::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(NV3007_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(NV3007_RST_DELAY);
  }
  else
  {
    // Software Rest
  }

  _bus->batchOperation(_init_operations, _init_operations_len);

  invertDisplay(false);
}
