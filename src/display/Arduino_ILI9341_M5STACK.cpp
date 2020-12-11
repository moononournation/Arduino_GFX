/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/adafruit/Adafruit_ILI9341.git
 */
#include "Arduino_ILI9341_M5STACK.h"

Arduino_ILI9341_M5STACK::Arduino_ILI9341_M5STACK(Arduino_DataBus *bus, int8_t rst, uint8_t r)
    : Arduino_ILI9341(bus, rst, r)
{
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9341_M5STACK::tftInit()
{
  if (_rst < 0)
  {
    _bus->sendCommand(ILI9341_SWRESET); // 1: Software reset
    delay(ILI9341_RST_DELAY);
  }

  _bus->sendCommand(0xEF);
  _bus->sendData(0x03);
  _bus->sendData(0x80);
  _bus->sendData(0x02);

  _bus->sendCommand(0xCF);
  _bus->sendData(0x00);
  _bus->sendData(0XC1);
  _bus->sendData(0X30);

  _bus->sendCommand(0xED);
  _bus->sendData(0x64);
  _bus->sendData(0x03);
  _bus->sendData(0X12);
  _bus->sendData(0X81);

  _bus->sendCommand(0xE8);
  _bus->sendData(0x85);
  _bus->sendData(0x00);
  _bus->sendData(0x78);

  _bus->sendCommand(0xCB);
  _bus->sendData(0x39);
  _bus->sendData(0x2C);
  _bus->sendData(0x00);
  _bus->sendData(0x34);
  _bus->sendData(0x02);

  _bus->sendCommand(0xF7);
  _bus->sendData(0x20);

  _bus->sendCommand(0xEA);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendCommand(ILI9341_PWCTR1); //Power control
  _bus->sendData(0x23);              //VRH[5:0]
  _bus->sendCommand(ILI9341_PWCTR2); //Power control
  _bus->sendData(0x10);              //SAP[2:0];BT[3:0]
  _bus->sendCommand(ILI9341_VMCTR1); //VCM control
  _bus->sendData(0x3e);
  _bus->sendData(0x28);
  _bus->sendCommand(ILI9341_VMCTR2); //VCM control2
  _bus->sendData(0x86);              //--

  _bus->sendCommand(ILI9341_MADCTL); // Memory Access Control
  _bus->sendData(0xA8);              // Rotation 0 (portrait mode)
  //_bus->sendData(0x48); // Rotation 0 (portrait mode)

  _bus->sendCommand(ILI9341_PIXFMT);
  _bus->sendData(0x55);
  _bus->sendCommand(ILI9341_FRMCTR1);
  _bus->sendData(0x00);
  _bus->sendData(0x13);               // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
  _bus->sendCommand(ILI9341_DFUNCTR); // Display Function Control
  _bus->sendData(0x08);
  _bus->sendData(0x82);
  _bus->sendData(0x27);
  _bus->sendCommand(0xF2); // 3Gamma Function Disable
  _bus->sendData(0x00);

  _bus->sendCommand(ILI9341_GAMMASET); //Gamma curve selected
  _bus->sendData(0x01);

  _bus->sendCommand(ILI9341_GMCTRP1); //Set Gamma
  _bus->sendData(0x0F);
  _bus->sendData(0x31);
  _bus->sendData(0x2B);
  _bus->sendData(0x0C);
  _bus->sendData(0x0E);
  _bus->sendData(0x08);
  _bus->sendData(0x4E);
  _bus->sendData(0xF1);
  _bus->sendData(0x37);
  _bus->sendData(0x07);
  _bus->sendData(0x10);
  _bus->sendData(0x03);
  _bus->sendData(0x0E);
  _bus->sendData(0x09);
  _bus->sendData(0x00);

  _bus->sendCommand(ILI9341_GMCTRN1); //Set Gamma
  _bus->sendData(0x00);
  _bus->sendData(0x0E);
  _bus->sendData(0x14);
  _bus->sendData(0x03);
  _bus->sendData(0x11);
  _bus->sendData(0x07);
  _bus->sendData(0x31);
  _bus->sendData(0xC1);
  _bus->sendData(0x48);
  _bus->sendData(0x08);
  _bus->sendData(0x0F);
  _bus->sendData(0x0C);
  _bus->sendData(0x31);
  _bus->sendData(0x36);
  _bus->sendData(0x0F);

  _bus->sendCommand(ILI9341_SLPOUT); // Exit Sleep
  delay(ILI9341_SLPOUT_DELAY);
  _bus->sendCommand(ILI9341_DISPON); // Display on
  delay(150);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ILI9341_M5STACK::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = (ILI9341_MADCTL_MV | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    break;
  case 1:
    r = (ILI9341_MADCTL_BGR);
    break;
  case 2:
    r = (ILI9341_MADCTL_MV | ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
    break;
  case 3:
    r = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    break;
  }

  _bus->beginWrite();
  _bus->writeCommand(ILI9341_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}
