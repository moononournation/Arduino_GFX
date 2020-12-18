/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/adafruit/Adafruit_ILI9341.git
 */
#include "Arduino_ILI9341.h"

Arduino_ILI9341::Arduino_ILI9341(Arduino_DataBus *bus, int8_t rst, uint8_t r)
    : Arduino_TFT(bus, rst, r, false, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, 0, 0, 0, 0)
{
}

void Arduino_ILI9341::begin(int32_t speed)
{
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9341::tftInit()
{
  if (_rst < 0)
  {
    _bus->sendCommand(ILI9341_SWRESET); // 1: Software reset
    delay(ILI9341_RST_DELAY);
  }

  spi_operation_t ili9341_init_operations[] = {
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_8, ILI9341_PWCTR1}, // Power control VRH[5:0]
      {WRITE_DATA_8, 0x23},
      {WRITE_COMMAND_8, ILI9341_PWCTR2}, // Power control SAP[2:0];BT[3:0]
      {WRITE_DATA_8, 0x10},
      {WRITE_COMMAND_8, ILI9341_VMCTR1}, // VCM control
      {WRITE_DATA_8, 0x3e},
      {WRITE_DATA_8, 0x28},
      {WRITE_COMMAND_8, ILI9341_VMCTR2}, // VCM control2
      {WRITE_DATA_8, 0x86},
      {WRITE_COMMAND_8, ILI9341_VSCRSADD}, // Vertical scroll zero
      {WRITE_DATA_8, 0x00},
      {WRITE_COMMAND_8, ILI9341_PIXFMT},
      {WRITE_DATA_8, 0x55},
      {WRITE_COMMAND_8, ILI9341_FRMCTR1},
      {WRITE_DATA_8, 0x00},
      {WRITE_DATA_8, 0x18},
      {WRITE_COMMAND_8, ILI9341_DFUNCTR}, // Display Function Control
      {WRITE_DATA_8, 0x08},
      {WRITE_DATA_8, 0x82},
      {WRITE_DATA_8, 0x27},
      {WRITE_COMMAND_8, ILI9341_SLPOUT}, // Exit Sleep
      {END_WRITE, 0},
      {DELAY, ILI9341_SLPOUT_DELAY},
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_8, ILI9341_DISPON}, // Display on
      {END_WRITE, 0},
  };

  _bus->batchOperation(ili9341_init_operations, sizeof(ili9341_init_operations) / sizeof(ili9341_init_operations[0]));
}

void Arduino_ILI9341::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _bus->writeC8D16D16(ILI9341_CASET, x + _xStart, x + w - 1 + _xStart);

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(ILI9341_PASET, y + _yStart, y + h - 1 + _yStart);

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(ILI9341_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ILI9341::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
    break;
  case 1:
    r = (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
    break;
  case 2:
    r = (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    break;
  case 3:
    r = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
    break;
  }
  _bus->beginWrite();
  _bus->writeC8D8(ILI9341_MADCTL, r);
  _bus->endWrite();
}

void Arduino_ILI9341::invertDisplay(bool i)
{
  _bus->sendCommand(i ? ILI9341_INVON : ILI9341_INVOFF);
}

void Arduino_ILI9341::displayOn(void)
{
  _bus->sendCommand(ILI9341_SLPOUT);
  delay(ILI9341_SLPOUT_DELAY);
}

void Arduino_ILI9341::displayOff(void)
{
  _bus->sendCommand(ILI9341_SLPIN);
  delay(ILI9341_SLPIN_DELAY);
}
