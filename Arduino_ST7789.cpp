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

void Arduino_ST7789::begin(int speed)
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

  _bus->sendCommand(0x36);
  _bus->sendData(0x00);

  _bus->sendCommand(0xB2);
  _bus->sendData(0x0C);
  _bus->sendData(0x0C);
  _bus->sendData(0x00);
  _bus->sendData(0x33);
  _bus->sendData(0x33);

  _bus->sendCommand(0xB7);
  _bus->sendData(0x35);

  _bus->sendCommand(0xBB);
  _bus->sendData(0x19);

  _bus->sendCommand(0xC0);
  _bus->sendData(0x2C);

  _bus->sendCommand(0xC2);
  _bus->sendData(0x01);

  _bus->sendCommand(0xC3);
  _bus->sendData(0x12);

  _bus->sendCommand(0xC4);
  _bus->sendData(0x20);

  _bus->sendCommand(0xC6);
  _bus->sendData(0x0F);

  _bus->sendCommand(0xD0);
  _bus->sendData(0xA4);
  _bus->sendData(0xA1);

  _bus->sendCommand(0xE0);
  _bus->sendData(0b11010000); // V63P3, V63P2, V63P1, V63P0,  V0P3,  V0P2,  V0P1,  V0P0
  _bus->sendData(0b00000100); //     0,     0,  V1P5,  V1P4,  V1P3,  V1P2,  V1P1,  V1P0
  _bus->sendData(0b00001101); //     0,     0,  V2P5,  V2P4,  V2P3,  V2P2,  V2P1,  V2P0
  _bus->sendData(0b00010001); //     0,     0,     0,  V4P4,  V4P3,  V4P2,  V4P1,  V4P0
  _bus->sendData(0b00010011); //     0,     0,     0,  V6P4,  V6P3,  V6P2,  V6P1,  V6P0
  _bus->sendData(0b00101011); //     0,     0,  J0P1,  J0P0, V13P3, V13P2, V13P1, V13P0
  _bus->sendData(0b00111111); //     0, V20P6, V20P5, V20P4, V20P3, V20P2, V20P1, V20P0
  _bus->sendData(0b01010100); //     0, V36P2, V36P1, V36P0,     0, V27P2, V27P1, V27P0
  _bus->sendData(0b01001100); //     0, V43P6, V43P5, V43P4, V43P3, V43P2, V43P1, V43P0
  _bus->sendData(0b00011000); //     0,     0,  J1P1,  J1P0, V50P3, V50P2, V50P1, V50P0
  _bus->sendData(0b00001101); //     0,     0,     0, V57P4, V57P3, V57P2, V57P1, V57P0
  _bus->sendData(0b00001011); //     0,     0,     0, V59P4, V59P3, V59P2, V59P1, V59P0
  _bus->sendData(0b00011111); //     0,     0, V61P5, V61P4, V61P3, V61P2, V61P1, V61P0
  _bus->sendData(0b00100011); //     0,     0, V62P5, V62P4, V62P3, V62P2, V62P1, V62P0

  _bus->sendCommand(0xE1);
  _bus->sendData(0b11010000); // V63N3, V63N2, V63N1, V63N0,  V0N3,  V0N2,  V0N1,  V0N0
  _bus->sendData(0b00000100); //     0,     0,  V1N5,  V1N4,  V1N3,  V1N2,  V1N1,  V1N0
  _bus->sendData(0b00001100); //     0,     0,  V2N5,  V2N4,  V2N3,  V2N2,  V2N1,  V2N0
  _bus->sendData(0b00010001); //     0,     0,     0,  V4N4,  V4N3,  V4N2,  V4N1,  V4N0
  _bus->sendData(0b00010011); //     0,     0,     0,  V6N4,  V6N3,  V6N2,  V6N1,  V6N0
  _bus->sendData(0b00101100); //     0,     0,  J0N1,  J0N0, V13N3, V13N2, V13N1, V13N0
  _bus->sendData(0b00111111); //     0, V20N6, V20N5, V20N4, V20N3, V20N2, V20N1, V20N0
  _bus->sendData(0b01000100); //     0, V36N2, V36N1, V36N0,     0, V27N2, V27N1, V27N0
  _bus->sendData(0b01010001); //     0, V43N6, V43N5, V43N4, V43N3, V43N2, V43N1, V43N0
  _bus->sendData(0b00101111); //     0,     0,  J1N1,  J1N0, V50N3, V50N2, V50N1, V50N0
  _bus->sendData(0b00011111); //     0,     0,     0, V57N4, V57N3, V57N2, V57N1, V57N0
  _bus->sendData(0b00011111); //     0,     0,     0, V59N4, V59N3, V59N2, V59N1, V59N0
  _bus->sendData(0b00100000); //     0,     0, V61N5, V61N4, V61N3, V61N2, V61N1, V61N0
  _bus->sendData(0b00100011); //     0,     0, V62N5, V62N4, V62N3, V62N2, V62N1, V62N0

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
