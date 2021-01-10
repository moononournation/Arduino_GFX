#include "Arduino_GC9A01.h"
#include "SPI.h"

Arduino_GC9A01::Arduino_GC9A01(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_GC9A01::begin(int32_t speed)
{
  _override_datamode = SPI_MODE0; // always use SPI_MODE0
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_GC9A01::tftInit()
{
  _bus->sendCommand(0xEF);
  _bus->sendCommand(0xEB);
  _bus->sendData(0x14);

  _bus->sendCommand(0xFE);
  _bus->sendCommand(0xEF);

  _bus->sendCommand(0xEB);
  _bus->sendData(0x14);

  _bus->sendCommand(0x84);
  _bus->sendData(0x40);

  _bus->sendCommand(0x85);
  _bus->sendData(0xFF);

  _bus->sendCommand(0x86);
  _bus->sendData(0xFF);

  _bus->sendCommand(0x87);
  _bus->sendData(0xFF);

  _bus->sendCommand(0x88);
  _bus->sendData(0x0A);

  _bus->sendCommand(0x89);
  _bus->sendData(0x21);

  _bus->sendCommand(0x8A);
  _bus->sendData(0x00);

  _bus->sendCommand(0x8B);
  _bus->sendData(0x80);

  _bus->sendCommand(0x8C);
  _bus->sendData(0x01);

  _bus->sendCommand(0x8D);
  _bus->sendData(0x01);

  _bus->sendCommand(0x8E);
  _bus->sendData(0xFF);

  _bus->sendCommand(0x8F);
  _bus->sendData(0xFF);

  _bus->sendCommand(0xB6);
  _bus->sendData(0x00);
  _bus->sendData(0x20);

  _bus->sendCommand(0x3A);
  _bus->sendData(0x05);

  _bus->sendCommand(0x90);
  _bus->sendData(0x08);
  _bus->sendData(0x08);
  _bus->sendData(0x08);
  _bus->sendData(0x08);

  _bus->sendCommand(0xBD);
  _bus->sendData(0x06);

  _bus->sendCommand(0xBC);
  _bus->sendData(0x00);

  _bus->sendCommand(0xFF);
  _bus->sendData(0x60);
  _bus->sendData(0x01);
  _bus->sendData(0x04);

  _bus->sendCommand(0xC3);
  _bus->sendData(0x13);
  _bus->sendCommand(0xC4);
  _bus->sendData(0x13);

  _bus->sendCommand(0xC9);
  _bus->sendData(0x22);

  _bus->sendCommand(0xBE);
  _bus->sendData(0x11);

  _bus->sendCommand(0xE1);
  _bus->sendData(0x10);
  _bus->sendData(0x0E);

  _bus->sendCommand(0xDF);
  _bus->sendData(0x21);
  _bus->sendData(0x0c);
  _bus->sendData(0x02);

  _bus->sendCommand(0xF0);
  _bus->sendData(0x45);
  _bus->sendData(0x09);
  _bus->sendData(0x08);
  _bus->sendData(0x08);
  _bus->sendData(0x26);
  _bus->sendData(0x2A);

  _bus->sendCommand(0xF1);
  _bus->sendData(0x43);
  _bus->sendData(0x70);
  _bus->sendData(0x72);
  _bus->sendData(0x36);
  _bus->sendData(0x37);
  _bus->sendData(0x6F);

  _bus->sendCommand(0xF2);
  _bus->sendData(0x45);
  _bus->sendData(0x09);
  _bus->sendData(0x08);
  _bus->sendData(0x08);
  _bus->sendData(0x26);
  _bus->sendData(0x2A);

  _bus->sendCommand(0xF3);
  _bus->sendData(0x43);
  _bus->sendData(0x70);
  _bus->sendData(0x72);
  _bus->sendData(0x36);
  _bus->sendData(0x37);
  _bus->sendData(0x6F);

  _bus->sendCommand(0xED);
  _bus->sendData(0x1B);
  _bus->sendData(0x0B);

  _bus->sendCommand(0xAE);
  _bus->sendData(0x77);

  _bus->sendCommand(0xCD);
  _bus->sendData(0x63);

  _bus->sendCommand(0x70);
  _bus->sendData(0x07);
  _bus->sendData(0x07);
  _bus->sendData(0x04);
  _bus->sendData(0x0E);
  _bus->sendData(0x0F);
  _bus->sendData(0x09);
  _bus->sendData(0x07);
  _bus->sendData(0x08);
  _bus->sendData(0x03);

  _bus->sendCommand(0xE8);
  _bus->sendData(0x34);

  _bus->sendCommand(0x62);
  _bus->sendData(0x18);
  _bus->sendData(0x0D);
  _bus->sendData(0x71);
  _bus->sendData(0xED);
  _bus->sendData(0x70);
  _bus->sendData(0x70);
  _bus->sendData(0x18);
  _bus->sendData(0x0F);
  _bus->sendData(0x71);
  _bus->sendData(0xEF);
  _bus->sendData(0x70);
  _bus->sendData(0x70);

  _bus->sendCommand(0x63);
  _bus->sendData(0x18);
  _bus->sendData(0x11);
  _bus->sendData(0x71);
  _bus->sendData(0xF1);
  _bus->sendData(0x70);
  _bus->sendData(0x70);
  _bus->sendData(0x18);
  _bus->sendData(0x13);
  _bus->sendData(0x71);
  _bus->sendData(0xF3);
  _bus->sendData(0x70);
  _bus->sendData(0x70);

  _bus->sendCommand(0x64);
  _bus->sendData(0x28);
  _bus->sendData(0x29);
  _bus->sendData(0xF1);
  _bus->sendData(0x01);
  _bus->sendData(0xF1);
  _bus->sendData(0x00);
  _bus->sendData(0x07);

  _bus->sendCommand(0x66);
  _bus->sendData(0x3C);
  _bus->sendData(0x00);
  _bus->sendData(0xCD);
  _bus->sendData(0x67);
  _bus->sendData(0x45);
  _bus->sendData(0x45);
  _bus->sendData(0x10);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);

  _bus->sendCommand(0x67);
  _bus->sendData(0x00);
  _bus->sendData(0x3C);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x01);
  _bus->sendData(0x54);
  _bus->sendData(0x10);
  _bus->sendData(0x32);
  _bus->sendData(0x98);

  _bus->sendCommand(0x74);
  _bus->sendData(0x10);
  _bus->sendData(0x85);
  _bus->sendData(0x80);
  _bus->sendData(0x00);
  _bus->sendData(0x00);
  _bus->sendData(0x4E);
  _bus->sendData(0x00);

  _bus->sendCommand(0x98);
  _bus->sendData(0x3e);
  _bus->sendData(0x07);

  _bus->sendCommand(0x35);

  if (_ips)
  {
    _bus->sendCommand(GC9A01_INVON);
  }
  _bus->sendCommand(GC9A01_SLPOUT);
  delay(GC9A01_SLPOUT_DELAY);
  _bus->sendCommand(GC9A01_DISPON);
  delay(20);
}

void Arduino_GC9A01::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW) || (y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(GC9A01_CASET, x + _xStart, x + w - 1 + _xStart);
    _bus->writeC8D16D16(GC9A01_RASET, y + _yStart, y + h - 1 + _yStart);
    _bus->writeCommand(GC9A01_RAMWR); // write to RAM

    _currentX = x;
    _currentY = y;
    _currentW = w;
    _currentH = h;
  }
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_GC9A01::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = 0x08;
    break;

  case 1:
    r = 0xC8;
    break;

  case 2:
    r = 0x68;
    break;

  case 3:
    r = 0xA8;
    break;
  }

  _bus->beginWrite();
  _bus->writeCommand(GC9A01_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_GC9A01::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? GC9A01_INVOFF : GC9A01_INVON) : (i ? GC9A01_INVON : GC9A01_INVOFF));
}

void Arduino_GC9A01::displayOn(void)
{
  _bus->sendCommand(GC9A01_SLPOUT);
  delay(GC9A01_SLPOUT_DELAY);
}

void Arduino_GC9A01::displayOff(void)
{
  _bus->sendCommand(GC9A01_SLPIN);
  delay(GC9A01_SLPIN_DELAY);
}
