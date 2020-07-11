/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/ananevilya/Arduino-ST7789-Library.git
 */
#include "Arduino_ST7789.h"

Arduino_ST7789::Arduino_ST7789(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_ST7789::begin(uint32_t speed)
{
#if defined(ESP8266) || defined(ESP32)
  if (speed == 0)
  {
    speed = 40000000;
  }
#endif
  _override_datamode = 3; // SPI_MODE3
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ST7789::tftInit()
{
  _bus->sendCommand(ST7789_SWRESET); // 1: Software reset
  delay(ST7789_RST_DELAY);

  _bus->sendCommand(ST7789_SLPOUT); // 2: Out of sleep mode, no args, w/delay
  delay(ST7789_SLPOUT_DELAY);
  _bus->sendCommand(ST7789_COLMOD); // 3: Set color mode, 1 arg + delay:
  _bus->sendData(0x55);             // 16-bit color
  if (_ips)
  {
    _bus->sendCommand(ST7789_INVON);
  }
  _bus->sendCommand(ST7789_NORON); // 4: Normal display on, no args, w/delay
  delay(10);
  _bus->sendCommand(ST7789_DISPON); // 5: Main screen turn on, no args, w/delay
}

void Arduino_ST7789::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _bus->writeC8D16D16(ST7789_CASET, x + _xStart, x + w - 1 + _xStart);

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(ST7789_RASET, y + _yStart, y + h - 1 + _yStart);

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(ST7789_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ST7789::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
    break;

  case 1:
    r = ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
    break;

  case 2:
    r = ST7789_MADCTL_RGB;
    break;

  case 3:
    r = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
    break;
  }

  _bus->beginWrite();
  _bus->writeCommand(ST7789_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_ST7789::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? ST7789_INVOFF : ST7789_INVON) : (i ? ST7789_INVON : ST7789_INVOFF));
}

void Arduino_ST7789::displayOn(void)
{
  _bus->sendCommand(ST7789_SLPOUT);
  delay(ST7789_SLPOUT_DELAY);
}

void Arduino_ST7789::displayOff(void)
{
  _bus->sendCommand(ST7789_SLPIN);
  delay(ST7789_SLPIN_DELAY);
}
