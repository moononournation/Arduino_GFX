/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/adafruit/Adafruit_ILI9341.git
 */
#include "Arduino_ILI9341.h"

Arduino_ILI9341::Arduino_ILI9341(Arduino_DataBus *bus, int8_t rst, uint8_t r)
    : Arduino_TFT(bus, rst, r, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, 0, 0)
{
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9341::tftInit()
{
  if (_rst < 0)
  {
    _bus->writeCommand(ILI9341_SWRESET); // 1: Software reset
    delay(ILI9341_RST_DELAY);
  }

  _bus->writeCommand(0xEF);
  _bus->writeData(0x03);
  _bus->writeData(0x80);
  _bus->writeData(0x02);
  _bus->writeCommand(0xCF);
  _bus->writeData(0x00);
  _bus->writeData(0xC1);
  _bus->writeData(0x30);
  _bus->writeCommand(0xED);
  _bus->writeData(0x64);
  _bus->writeData(0x03);
  _bus->writeData(0x12);
  _bus->writeData(0x81);
  _bus->writeCommand(0xE8);
  _bus->writeData(0x85);
  _bus->writeData(0x00);
  _bus->writeData(0x78);
  _bus->writeCommand(0xCB);
  _bus->writeData(0x39);
  _bus->writeData(0x2C);
  _bus->writeData(0x00);
  _bus->writeData(0x34);
  _bus->writeData(0x02);
  _bus->writeCommand(0xF7);
  _bus->writeData(0x20);
  _bus->writeCommand(0xEA);
  _bus->writeData(0x00);
  _bus->writeData(0x00);
  _bus->writeCommand(ILI9341_PWCTR1); // Power control VRH[5:0]
  _bus->writeData(0x23);
  _bus->writeCommand(ILI9341_PWCTR2); // Power control SAP[2:0];BT[3:0]
  _bus->writeData(0x10);
  _bus->writeCommand(ILI9341_VMCTR1); // VCM control
  _bus->writeData(0x3e);
  _bus->writeData(0x28);
  _bus->writeCommand(ILI9341_VMCTR2); // VCM control2
  _bus->writeData(0x86);
  _bus->writeCommand(ILI9341_VSCRSADD); // Vertical scroll zero
  _bus->writeData(0x00);
  _bus->writeCommand(ILI9341_PIXFMT);
  _bus->writeData(0x55);
  _bus->writeCommand(ILI9341_FRMCTR1);
  _bus->writeData(0x00);
  _bus->writeData(0x18);
  _bus->writeCommand(ILI9341_DFUNCTR); // Display Function Control
  _bus->writeData(0x08);
  _bus->writeData(0x82);
  _bus->writeData(0x27);
  _bus->writeCommand(ILI9341_SLPOUT); // Exit Sleep
  delay(ILI9341_SLPOUT_DELAY);
  _bus->writeCommand(ILI9341_DISPON); // Display on
  delay(150);
}

void Arduino_ILI9341::writeAddrColumn(uint16_t x, uint16_t w)
{
#ifdef ESP32
  uint32_t x_range = ((uint32_t)(x + _xstart) << 16) | (x + w - 1 + _xstart);

  _bus->writeCommandCore(ILI9341_CASET); // Column addr set
  _bus->write32(x_range);
#else
  uint16_t x_start = x + _xstart, x_end = x + w - 1 + _xstart;

  _bus->writeCommandCore(ILI9341_CASET); // Column addr set
  _bus->write(x_start >> 8);
  _bus->write(x_start & 0xFF); // XSTART
  _bus->write(x_end >> 8);
  _bus->write(x_end & 0xFF); // XEND
#endif
}

void Arduino_ILI9341::writeAddrRow(uint16_t y, uint16_t h)
{
#ifdef ESP32
  uint32_t y_range = ((uint32_t)(y + _xstart) << 16) | (y + h - 1 + _ystart);

  _bus->writeCommandCore(ILI9341_PASET); // Row addr set
  _bus->write32(y_range);
#else
  uint16_t y_start = y + _ystart, y_end = y + h - 1 + _ystart;

  _bus->writeCommandCore(ILI9341_PASET); // Row addr set
  _bus->write(y_start >> 8);
  _bus->write(y_start & 0xFF); // YSTART
  _bus->write(y_end >> 8);
  _bus->write(y_end & 0xFF); // YEND
#endif
}

void Arduino_ILI9341::writeAddrMemWrite()
{
  _bus->writeCommandCore(ILI9341_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ILI9341::setRotation(uint8_t m)
{
  rotation = m % 4; // can't be higher than 3
  switch (rotation)
  {
  case 0:
    m = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);

    _width = WIDTH;
    _height = HEIGHT;
    _max_x = _width - 1;  ///< x zero base bound
    _max_y = _height - 1; ///< y zero base bound
    _xstart = COL_OFFSET;
    _ystart = ROW_OFFSET;
    break;
  case 1:
    m = (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);

    _width = HEIGHT;
    _height = WIDTH;
    _max_x = _width - 1;  ///< x zero base bound
    _max_y = _height - 1; ///< y zero base bound
    _xstart = ROW_OFFSET;
    _ystart = COL_OFFSET;
    break;
  case 2:
    m = (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);

    _width = WIDTH;
    _height = HEIGHT;
    _max_x = _width - 1;  ///< x zero base bound
    _max_y = _height - 1; ///< y zero base bound
    _xstart = ILI9341_TFTWIDTH - WIDTH - COL_OFFSET;
    _ystart = ILI9341_TFTHEIGHT - HEIGHT - ROW_OFFSET;
    break;
  case 3:
    m = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);

    _width = HEIGHT;
    _height = WIDTH;
    _max_x = _width - 1;  ///< x zero base bound
    _max_y = _height - 1; ///< y zero base bound
    _xstart = ILI9341_TFTHEIGHT - HEIGHT - ROW_OFFSET;
    _ystart = ILI9341_TFTWIDTH - WIDTH - COL_OFFSET;
    break;
  }

  _bus->beginWrite();
  _bus->writeCommandCore(ILI9341_MADCTL);
  _bus->write(m);
  _bus->endWrite();
}

void Arduino_ILI9341::invertDisplay(boolean i)
{
  _bus->writeCommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}

void Arduino_ILI9341::displayOn(void)
{
  _bus->writeCommand(ILI9341_SLPOUT);
  delay(5);
}

void Arduino_ILI9341::displayOff(void)
{
  _bus->writeCommand(ILI9341_SLPIN);
}
