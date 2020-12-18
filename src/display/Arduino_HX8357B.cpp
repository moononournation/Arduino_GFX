/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_HX8357B.h"

Arduino_HX8357B::Arduino_HX8357B(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips /* = false */)
    : Arduino_TFT(bus, rst, r, ips, HX8357B_TFTWIDTH, HX8357B_TFTHEIGHT, 0, 0, 0, 0)
{
}

void Arduino_HX8357B::begin(int32_t speed)
{
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_HX8357B::tftInit()
{
  if (_rst < 0)
  {
    _bus->sendCommand(HX8357B_SWRESET); // 1: Software reset
    delay(HX8357B_RST_DELAY);
  }

  //************* Start Initial Sequence **********//
  delay(10);
  _bus->sendCommand(0x11); //Sleep Out
  delay(120);

  _bus->sendCommand(0xEE); //Set EQ
  _bus->sendData(0x02);
  _bus->sendData(0x01);
  _bus->sendData(0x02);
  _bus->sendData(0x01);

  _bus->sendCommand(0xED); //Set DIR TIM
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x9A);
  _bus->sendData(0x9A);
  _bus->sendData(0x9B);
  _bus->sendData(0x9B);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0xAE);
  _bus->sendData(0xAE);
  _bus->sendData(0x01);
  _bus->sendData(0xA2);
  _bus->sendData(0x00);

  _bus->sendCommand(0xB4); //Set RM, DM
  _bus->sendData(0x00);    //

  _bus->sendCommand(0xC0); //Set Panel Driving
  _bus->sendData(0x10);    //REV SM GS
  _bus->sendData(0x3B);    // NL[5:0]
  _bus->sendData(0x00);    //SCN[6:0]
  _bus->sendData(0x02);    //NDL 0 PTS[2:0]
  _bus->sendData(0x11);    //PTG ISC[3:0]

  _bus->sendCommand(0xC1); //
  _bus->sendData(0x10);    //ne inversion

  _bus->sendCommand(0xC8); //Set Gamma
  _bus->sendData(0x00);    //KP1,KP0
  _bus->sendData(0x32);    //KP3,KP2
  _bus->sendData(0x36);    //KP5,KP4
  _bus->sendData(0x45);    //RP1,RP0
  _bus->sendData(0x06);    //VRP0  01
  _bus->sendData(0x16);    //VRP1
  _bus->sendData(0x37);    //KN1,KN0
  _bus->sendData(0x75);    //KN3,KN2
  _bus->sendData(0x77);    //KN5,KN4
  _bus->sendData(0x54);    //RN1,RN0
  _bus->sendData(0x0c);    //VRN0
  _bus->sendData(0x00);    //VRN1  01

  _bus->sendCommand(0xD0); //Set Power
  _bus->sendData(0x44);    //DDVDH :5.28
  _bus->sendData(0x42);    // BT VGH:15.84    VGL:-7.92
  _bus->sendData(0x06);    //VREG1  4.625V

  _bus->sendCommand(0xD1); //Set VCOM
  _bus->sendData(0x43);    //VCOMH
  _bus->sendData(0x16);

  _bus->sendCommand(0xD2);
  _bus->sendData(0x04);
  _bus->sendData(0x22); //12

  _bus->sendCommand(0xD3);
  _bus->sendData(0x04);
  _bus->sendData(0x12);

  _bus->sendCommand(0xD4);
  _bus->sendData(0x07);
  _bus->sendData(0x12);

  _bus->sendCommand(0xE9); //Set Panel
  _bus->sendData(0x00);

  _bus->sendCommand(0xC5); //Set Frame rate
  _bus->sendData(0x08);    //61.51Hz

  _bus->sendCommand(0x36);
  _bus->sendData(0x0a);

  _bus->sendCommand(0X3A);
  _bus->sendData(0X55);

  delay(120);

  _bus->sendCommand(0x21);

  _bus->sendCommand(0x29); //Display On
  delay(50);
}

void Arduino_HX8357B::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    uint16_t x_start = x + _xStart, x_end = x + w - 1 + _xStart;

    _bus->writeCommand(HX8357B_CASET); // Column addr set
    _bus->write(x_start >> 8);
    _bus->write(x_start & 0xFF); // XSTART
    _bus->write(x_end >> 8);
    _bus->write(x_end & 0xFF); // XEND

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    uint16_t y_start = y + _yStart, y_end = y + h - 1 + _yStart;

    _bus->writeCommand(HX8357B_PASET); // Row addr set
    _bus->write(y_start >> 8);
    _bus->write(y_start & 0xFF); // YSTART
    _bus->write(y_end >> 8);
    _bus->write(y_end & 0xFF); // YEND

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(HX8357B_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_HX8357B::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = (HX8357B_MADCTL_MX | HX8357B_MADCTL_BGR);
    break;
  case 1:
    r = (HX8357B_MADCTL_MV | HX8357B_MADCTL_BGR);
    break;
  case 2:
    r = (HX8357B_MADCTL_MY | HX8357B_MADCTL_BGR);
    break;
  case 3:
    r = (HX8357B_MADCTL_MX | HX8357B_MADCTL_MY | HX8357B_MADCTL_MV | HX8357B_MADCTL_BGR);
    break;
  }

  _bus->beginWrite();
  _bus->writeCommand(HX8357B_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_HX8357B::invertDisplay(bool i)
{
  _bus->sendCommand(i ? HX8357B_INVON : HX8357B_INVOFF);
}

void Arduino_HX8357B::displayOn(void)
{
  _bus->sendCommand(HX8357B_SLPOUT);
  delay(HX8357B_SLPOUT_DELAY);
}

void Arduino_HX8357B::displayOff(void)
{
  _bus->sendCommand(HX8357B_SLPIN);
  delay(HX8357B_SLPIN_DELAY);
}
