#include "Arduino_SPD2010.h"
#include "SPI.h"

Arduino_SPD2010::Arduino_SPD2010(Arduino_DataBus *bus, int8_t rst)
    : Arduino_TFT(bus, rst, 0, false, SPD2010_TFTWIDTH, SPD2010_TFTHEIGHT, 0, 0, 0, 0)
{
}

bool Arduino_SPD2010::begin(int32_t speed)
{
  _override_datamode = SPI_MODE3; // always use SPI_MODE3

  return Arduino_TFT::begin(speed);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_SPD2010::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  // not implemented
}

void Arduino_SPD2010::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16(SPD2010_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16(SPD2010_PASET, y, y + h - 1);
  }

  _bus->writeCommand(SPD2010_RAMWR); // write to RAM
}

void Arduino_SPD2010::invertDisplay(bool i)
{
  _bus->sendCommand(i ? SPD2010_INVON : SPD2010_INVOFF);
}

void Arduino_SPD2010::displayOn(void)
{
  _bus->sendCommand(SPD2010_SLPOUT);
  delay(SPD2010_SLPOUT_DELAY);
}

void Arduino_SPD2010::displayOff(void)
{
  _bus->sendCommand(SPD2010_SLPIN);
  delay(SPD2010_SLPIN_DELAY);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_SPD2010::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(SPD2010_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(SPD2010_RST_DELAY);
  }

  _bus->batchOperation(spd2010_init_operations, sizeof(spd2010_init_operations));

  invertDisplay(false);
}
