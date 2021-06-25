#include "Arduino_ILI9806.h"
#include "SPI.h"

Arduino_ILI9806::Arduino_ILI9806(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips)
    : Arduino_TFT(bus, rst, r, ips, ILI9806_TFTWIDTH, ILI9806_TFTHEIGHT, 0, 0, 0, 0)
{
}

void Arduino_ILI9806::begin(int32_t speed)
{
  _override_datamode = SPI_MODE0; // always use SPI_MODE0
  Arduino_TFT::begin(speed);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ILI9806::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 3:
    r = (ILI9806_MADCTL_MY | ILI9806_MADCTL_MV | ILI9806_MADCTL_RGB);
    break;
  case 2:
    r = (ILI9806_MADCTL_MX | ILI9806_MADCTL_MY | ILI9806_MADCTL_RGB);
    break;
  case 1:
    r = (ILI9806_MADCTL_MX | ILI9806_MADCTL_MV | ILI9806_MADCTL_RGB);
    break;
  default: // case 0:
    r = ILI9806_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(ILI9806_MADCTL, r);
  _bus->endWrite();
}

void Arduino_ILI9806::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16(ILI9806_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16(ILI9806_PASET, y, y + h - 1);
  }

  _bus->writeCommand(ILI9806_RAMWR); // write to RAM
}

void Arduino_ILI9806::invertDisplay(bool i)
{
  _bus->sendCommand((_ips ^ i) ? ILI9806_INVON : ILI9806_INVOFF);
}

void Arduino_ILI9806::displayOn(void)
{
  _bus->sendCommand(ILI9806_SLPOUT);
  delay(ILI9806_SLPOUT_DELAY);
}

void Arduino_ILI9806::displayOff(void)
{
  _bus->sendCommand(ILI9806_SLPIN);
  delay(ILI9806_SLPIN_DELAY);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9806::tftInit()
{
  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(ILI9806_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(ILI9806_RST_DELAY);
  }
  else
  {
    // Software Rest
    _bus->sendCommand(ILI9806_SWRESET);
    delay(ILI9806_RST_DELAY);
  }

  /***************************************/
  _bus->sendCommand(0xFF); // EXTC Command Set enable register
  _bus->sendData(0xFF);
  _bus->sendData(0x98);
  _bus->sendData(0x06);

  _bus->sendCommand(0xBA); // SPI Interface Setting
  _bus->sendData(0x60);

  _bus->sendCommand(0xBC); // GIP 1
  _bus->sendData(0x01);
  _bus->sendData(0x10);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x01);
  _bus->sendData(0x01);
  _bus->sendData(0x0B);
  _bus->sendData(0x11);
  _bus->sendData(0x32);
  _bus->sendData(0x10);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x01);
  _bus->sendData(0x01);
  _bus->sendData(0x01);
  _bus->sendData(0x01);
  _bus->sendData(0x50);
  _bus->sendData(0x52);
  _bus->sendData(0x01);
  _bus->sendData(0x00);
  _bus->sendData(0x40);

  _bus->sendCommand(0xBD); // GIP 2
  _bus->sendData(0x01);
  _bus->sendData(0x23);
  _bus->sendData(0x45);
  _bus->sendData(0x67);
  _bus->sendData(0x01);
  _bus->sendData(0x23);
  _bus->sendData(0x45);
  _bus->sendData(0x67);

  _bus->sendCommand(0xBE); // GIP 3
  _bus->sendData(0x00);
  _bus->sendData(0x21);
  _bus->sendData(0xAB);
  _bus->sendData(0x60);
  _bus->sendData(0x22);
  _bus->sendData(0x22);
  _bus->sendData(0x22);
  _bus->sendData(0x22);
  _bus->sendData(0x22);

  _bus->sendCommand(0xC7); // VCOM Control
  _bus->sendData(0x30);

  _bus->sendCommand(0xED); // EN_volt_reg
  _bus->sendData(0x7F);
  _bus->sendData(0x0F);
  _bus->sendData(0x00);

  _bus->sendCommand(0xC0); // Power Control 1
  _bus->sendData(0x03);
  _bus->sendData(0x0B);
  _bus->sendData(0x0C); //0A  VGH VGL

  _bus->sendCommand(0xFD); //External Power Selection Set
  _bus->sendData(0x0A);
  _bus->sendData(0x00);

  _bus->sendCommand(0xFC); // LVGL
  _bus->sendData(0x08);

  _bus->sendCommand(0xDF); // Engineering Setting
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x20);

  _bus->sendCommand(0xF3); // DVDD Voltage Setting
  _bus->sendData(0x74);

  _bus->sendCommand(0xB4); // Display Inversion Control
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);

  _bus->sendCommand(0xB5); //Blanking Porch Control
  _bus->sendData(0x08);
  _bus->sendData(0x15);

  _bus->sendCommand(0xF7); // 480x854
  _bus->sendData(0x81);

  _bus->sendCommand(0xB1); // Frame Rate Control
  _bus->sendData(0x00);
  _bus->sendData(0x13);
  _bus->sendData(0x13);

  _bus->sendCommand(0xF2); //Panel Timing Control
  _bus->sendData(0x80);
  _bus->sendData(0x04);
  _bus->sendData(0x40);
  _bus->sendData(0x28);

  _bus->sendCommand(0xC1); //Power Control 2
  _bus->sendData(0x17);
  _bus->sendData(0x71); //VGMP
  _bus->sendData(0x71); //VGMN

  _bus->sendCommand(0xE0); //P_Gamma
  _bus->sendData(0x00);    //P1
  _bus->sendData(0x13);    //P2
  _bus->sendData(0x1A);    //P3
  _bus->sendData(0x0C);    //P4
  _bus->sendData(0x0E);    //P5
  _bus->sendData(0x0B);    //P6
  _bus->sendData(0x07);    //P7
  _bus->sendData(0x05);    //P8
  _bus->sendData(0x05);    //P9
  _bus->sendData(0x0A);    //P10
  _bus->sendData(0x0F);    //P11
  _bus->sendData(0x0F);    //P12
  _bus->sendData(0x0E);    //P13
  _bus->sendData(0x1C);    //P14
  _bus->sendData(0x16);    //P15
  _bus->sendData(0x00);    //P16

  _bus->sendCommand(0xE1); //N_Gamma
  _bus->sendData(0x00);    //P1
  _bus->sendData(0x13);    //P2
  _bus->sendData(0x1A);    //P3
  _bus->sendData(0x0C);    //P4
  _bus->sendData(0x0E);    //P5
  _bus->sendData(0x0B);    //P6
  _bus->sendData(0x07);    //P7
  _bus->sendData(0x05);    //P8
  _bus->sendData(0x05);    //P9
  _bus->sendData(0x0A);    //P10
  _bus->sendData(0x0F);    //P11
  _bus->sendData(0x0F);    //P12
  _bus->sendData(0x0E);    //P13
  _bus->sendData(0x1C);    //P14
  _bus->sendData(0x16);    //P15
  _bus->sendData(0x00);    //P16

  _bus->sendCommand(0x3A);
  _bus->sendData(0x55); //55-16BIT,66-18BIT,77-24BIT

  _bus->sendCommand(0x36); //Memory Access Control
  _bus->sendData(0x00);    //01-180
  //*****************
  _bus->sendCommand(0x2A); //480
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x01);
  _bus->sendData(0xdf);

  _bus->sendCommand(0x2B); //854
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x03);
  _bus->sendData(0x55);
  //*****************************

  _bus->sendCommand(0x11);
  delay(120);
  _bus->sendCommand(0x29);
  delay(25);
  _bus->sendCommand(0x2C);

  if (_ips)
  {
    _bus->sendCommand(ILI9806_INVON);
  }
}
