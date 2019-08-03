/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_ILI9225.h"

Arduino_ILI9225::Arduino_ILI9225(Arduino_DataBus *bus, int8_t rst, uint8_t r)
    : Arduino_TFT(bus, rst, r, false, ILI9225_TFTWIDTH, ILI9225_TFTHEIGHT, 0, 0, 0, 0)
{
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9225::tftInit()
{
  // Power-on sequence
  _bus->sendCommand(ILI9225_POWER_CTRL2);
  _bus->sendData16(0x0018); // Set APON,PON,AON,VCI1EN,VC
  _bus->sendCommand(ILI9225_POWER_CTRL3);
  _bus->sendData16(0x6121); // Set BT,DC1,DC2,DC3
  _bus->sendCommand(ILI9225_POWER_CTRL4);
  _bus->sendData16(0x006F); // Set GVDD   /*007F 0088 */
  _bus->sendCommand(ILI9225_POWER_CTRL5);
  _bus->sendData16(0x495F); // Set VCOMH/VCOML voltage
  _bus->sendCommand(ILI9225_POWER_CTRL1);
  _bus->sendData16(0x0800); // Set SAP,DSTB,STB

  delay(10);

  _bus->sendCommand(ILI9225_POWER_CTRL2);
  _bus->sendData16(0x103B); // Set APON,PON,AON,VCI1EN,VC

  _bus->sendCommand(ILI9225_DISP_CTRL1);
  _bus->sendData16(0x0012);

  delay(50);

  _bus->sendCommand(ILI9225_DISP_CTRL1);
  _bus->sendData16(0x1017);
}

void Arduino_ILI9225::writeAddrColumn(uint16_t x, uint16_t w)
{
  uint16_t start = x + _xStart, end = x + w - 1 + _xStart;

  if (_rotation & 0x01) // Portrait
  {
    _bus->writeCommand(ILI9225_VERTICAL_WINDOW_ADDR2);
    _bus->write16(start);
    _bus->writeCommand(ILI9225_VERTICAL_WINDOW_ADDR1);
    _bus->write16(end);
    _bus->writeCommand(ILI9225_RAM_ADDR_SET2);
    _bus->write16(start);
  }
  else
  {
    _bus->writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    _bus->write16(start);
    _bus->writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    _bus->write16(end);
    _bus->writeCommand(ILI9225_RAM_ADDR_SET1);
    _bus->write16(start);
  }
}

void Arduino_ILI9225::writeAddrRow(uint16_t y, uint16_t h)
{
  uint16_t start = y + _yStart, end = y + h - 1 + _yStart;

  if (_rotation & 0x01) // Portrait
  {
    _bus->writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    _bus->write16(start);
    _bus->writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    _bus->write16(end);
    _bus->writeCommand(ILI9225_RAM_ADDR_SET1);
    _bus->write16(start);
  }
  else
  {
    _bus->writeCommand(ILI9225_VERTICAL_WINDOW_ADDR2);
    _bus->write16(start);
    _bus->writeCommand(ILI9225_VERTICAL_WINDOW_ADDR1);
    _bus->write16(end);
    _bus->writeCommand(ILI9225_RAM_ADDR_SET2);
    _bus->write16(start);
  }
}

void Arduino_ILI9225::writeAddrMemWrite()
{
  _bus->writeCommand(ILI9225_GRAM_DATA_REG); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ILI9225::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    _bus->sendCommand(ILI9225_DRIVER_OUTPUT_CTRL);
    _bus->sendData16(0x021C);
    _bus->sendCommand(ILI9225_ENTRY_MODE);
    _bus->sendData16(0x1030);
    break;
  case 1:
    _bus->sendCommand(ILI9225_DRIVER_OUTPUT_CTRL);
    _bus->sendData16(0x031C);
    _bus->sendCommand(ILI9225_ENTRY_MODE);
    _bus->sendData16(0x1038);
    break;
  case 2:
    _bus->sendCommand(ILI9225_DRIVER_OUTPUT_CTRL);
    _bus->sendData16(0x011C);
    _bus->sendCommand(ILI9225_ENTRY_MODE);
    _bus->sendData16(0x1030);
    break;
  case 3:
    _bus->sendCommand(ILI9225_DRIVER_OUTPUT_CTRL);
    _bus->sendData16(0x001C);
    _bus->sendCommand(ILI9225_ENTRY_MODE);
    _bus->sendData16(0x1038);
    break;
  }
}

void Arduino_ILI9225::invertDisplay(bool i)
{
  // Not Implemented
}

void Arduino_ILI9225::displayOn(void)
{
  _bus->sendCommand(ILI9225_POWER_CTRL1);
  _bus->sendData16(0x0800); // Set SAP,DSTB,STB
}

void Arduino_ILI9225::displayOff(void)
{
  _bus->sendCommand(ILI9225_POWER_CTRL1);
  _bus->sendData16(0x0801); // Set SAP,DSTB,STB
}
