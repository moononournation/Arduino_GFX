#include "Arduino_JD9613.h"
#include "SPI.h"

Arduino_JD9613::Arduino_JD9613(
    Arduino_DataBus *bus, int8_t rst, uint8_t r, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_OLED(
          bus, rst, r, w, h,
          col_offset1, row_offset1, col_offset2, row_offset2)
{
}

bool Arduino_JD9613::begin(int32_t speed)
{
  _override_datamode = SPI_MODE0; // always use SPI_MODE0

  return Arduino_TFT::begin(speed);
}

void Arduino_JD9613::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16(JD9613_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16(JD9613_RASET, y, y + h - 1);
  }

  _bus->writeCommand(JD9613_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_JD9613::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = JD9613_MADCTL_MX | JD9613_MADCTL_MV | JD9613_MADCTL_RGB;
    break;
  case 2:
    r = JD9613_MADCTL_MY | JD9613_MADCTL_MX | JD9613_MADCTL_RGB;
    break;
  case 3:
    r = JD9613_MADCTL_MY | JD9613_MADCTL_MV | JD9613_MADCTL_RGB;
    break;
  default: // case 0:
    r = JD9613_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(JD9613_MADCTL, r);
  _bus->endWrite();
}

void Arduino_JD9613::invertDisplay(bool i)
{
  _bus->sendCommand((_ips ^ i) ? JD9613_INVON : JD9613_INVOFF);
}

void Arduino_JD9613::displayOn(void)
{
  _bus->sendCommand(JD9613_SLPOUT);
  delay(JD9613_SLPOUT_DELAY);
}

void Arduino_JD9613::displayOff(void)
{
  _bus->sendCommand(JD9613_SLPIN);
  delay(JD9613_SLPIN_DELAY);
}

void Arduino_JD9613::setBrightness(uint8_t brightness)
{
  _bus->beginWrite();
  _bus->writeC8D8(0x51, brightness);
  _bus->endWrite();
}

void Arduino_JD9613::setContrast(uint8_t contrast)
{
  // not implemented.
}

void Arduino_JD9613::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(JD9613_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(JD9613_RST_DELAY);
  }
  else
  {
    // Software Rest
    _bus->sendCommand(JD9613_SWRESET);
    delay(JD9613_RST_DELAY);
  }

  _bus->batchOperation(jd9613_init_operations, sizeof(jd9613_init_operations));

  invertDisplay(false);
}
