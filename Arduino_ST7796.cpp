/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_ST7796.h"

Arduino_ST7796::Arduino_ST7796(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_ST7796::begin(uint32_t speed)
{
#if defined(ESP8266) || defined(ESP32)
  if (speed == 0)
  {
    speed = 40000000;
  }
#endif
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ST7796::tftInit()
{
  _bus->sendCommand(ST7796_SWRESET); // 1: Software reset
  delay(ST7796_RST_DELAY);

  _bus->sendCommand(0xF0);
  _bus->sendData(0xC3);
  _bus->sendCommand(0xF0);
  _bus->sendData(0x96);
  _bus->sendCommand(0x3A);
  _bus->sendData(0x05);
  _bus->sendCommand(0xB0);
  _bus->sendData(0x80);
  _bus->sendCommand(0xB6);
  _bus->sendData(0x00);
  _bus->sendData(0x02);
  _bus->sendCommand(0xB5);
  _bus->sendData(0x02);
  _bus->sendData(0x03);
  _bus->sendData(0x00);
  _bus->sendData(0x04);
  _bus->sendCommand(0xB1);
  _bus->sendData(0x80);
  _bus->sendData(0x10);
  _bus->sendCommand(0xB4);
  _bus->sendData(0x00);
  _bus->sendCommand(0xB7);
  _bus->sendData(0xC6);
  _bus->sendCommand(0xC5);
  _bus->sendData(0x24);
  _bus->sendCommand(0xE4);
  _bus->sendData(0x31);
  _bus->sendCommand(0xE8);
  _bus->sendData(0x40);
  _bus->sendData(0x8A);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x29);
  _bus->sendData(0x19);
  _bus->sendData(0xA5);
  _bus->sendData(0x33);
  _bus->sendCommand(0xC2);
  _bus->sendCommand(0xA7);

  _bus->sendCommand(0xE0);
  _bus->sendData(0xF0);
  _bus->sendData(0x09);
  _bus->sendData(0x13);
  _bus->sendData(0x12);
  _bus->sendData(0x12);
  _bus->sendData(0x2B);
  _bus->sendData(0x3C);
  _bus->sendData(0x44);
  _bus->sendData(0x4B);
  _bus->sendData(0x1B);
  _bus->sendData(0x18);
  _bus->sendData(0x17);
  _bus->sendData(0x1D);
  _bus->sendData(0x21);

  _bus->sendCommand(0XE1);
  _bus->sendData(0xF0);
  _bus->sendData(0x09);
  _bus->sendData(0x13);
  _bus->sendData(0x0C);
  _bus->sendData(0x0D);
  _bus->sendData(0x27);
  _bus->sendData(0x3B);
  _bus->sendData(0x44);
  _bus->sendData(0x4D);
  _bus->sendData(0x0B);
  _bus->sendData(0x17);
  _bus->sendData(0x17);
  _bus->sendData(0x1D);
  _bus->sendData(0x21);

  _bus->sendCommand(0xF0);
  _bus->sendData(0xC3);
  _bus->sendCommand(0xF0);
  _bus->sendData(0x69);
  _bus->sendCommand(0X13);
  _bus->sendCommand(0X11);
  _bus->sendCommand(0X29);
}

void Arduino_ST7796::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _bus->writeC8D16D16(ST7796_CASET, x + _xStart, x + w - 1 + _xStart);

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(ST7796_RASET, y + _yStart, y + h - 1 + _yStart);

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(ST7796_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ST7796::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    // r = ST7796_MADCTL_MX | ST7796_MADCTL_MY | ST7796_MADCTL_BGR | ST7796_MADCTL_MH;
    r = ST7796_MADCTL_MY | ST7796_MADCTL_BGR;
    break;

  case 1:
    // r = ST7796_MADCTL_MY | ST7796_MADCTL_MV | ST7796_MADCTL_BGR | ST7796_MADCTL_MH;
    r = ST7796_MADCTL_MX | ST7796_MADCTL_MY | ST7796_MADCTL_MV | ST7796_MADCTL_BGR;
    break;

  case 2:
    r = ST7796_MADCTL_MX | ST7796_MADCTL_ML | ST7796_MADCTL_BGR | ST7796_MADCTL_MH;
    r = ST7796_MADCTL_MX | ST7796_MADCTL_BGR;
    break;

  case 3:
    // r = ST7796_MADCTL_MX | ST7796_MADCTL_MV | ST7796_MADCTL_BGR | ST7796_MADCTL_MH;
    r = ST7796_MADCTL_MV | ST7796_MADCTL_BGR;
    break;
  }

  _bus->beginWrite();
  _bus->writeCommand(ST7796_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_ST7796::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? ST7796_INVOFF : ST7796_INVON) : (i ? ST7796_INVON : ST7796_INVOFF));
}

void Arduino_ST7796::displayOn(void)
{
  _bus->sendCommand(ST7796_SLPOUT);
  delay(ST7796_SLPOUT_DELAY);
}

void Arduino_ST7796::displayOff(void)
{
  _bus->sendCommand(ST7796_SLPIN);
  delay(ST7796_SLPIN_DELAY);
}
