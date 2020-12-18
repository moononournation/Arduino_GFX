/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_ILI9488_18bit.h"

Arduino_ILI9488_18bit::Arduino_ILI9488_18bit(Arduino_DataBus *bus, int8_t rst, uint8_t r)
    : Arduino_TFT_18bit(bus, rst, r, false, ILI9488_TFTWIDTH, ILI9488_TFTHEIGHT, 0, 0, 0, 0)
{
}

void Arduino_ILI9488_18bit::begin(int32_t speed)
{
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ILI9488_18bit::tftInit()
{
  if (_rst < 0)
  {
    _bus->sendCommand(ILI9488_SWRST); // 1: Software reset
    delay(ILI9488_RST_DELAY);
  }

  spi_operation_t ili9488_init_operations[] = {
      {BEGIN_WRITE, 0},

      {WRITE_COMMAND_8, 0xE0},
      {WRITE_DATA_8, 0x00},
      {WRITE_DATA_8, 0x03},
      {WRITE_DATA_8, 0x09},
      {WRITE_DATA_8, 0x08},
      {WRITE_DATA_8, 0x16},
      {WRITE_DATA_8, 0x0A},
      {WRITE_DATA_8, 0x3F},
      {WRITE_DATA_8, 0x78},
      {WRITE_DATA_8, 0x4C},
      {WRITE_DATA_8, 0x09},
      {WRITE_DATA_8, 0x0A},
      {WRITE_DATA_8, 0x08},
      {WRITE_DATA_8, 0x16},
      {WRITE_DATA_8, 0x1A},
      {WRITE_DATA_8, 0x0F},

      {WRITE_COMMAND_8, 0XE1},
      {WRITE_DATA_8, 0x00},
      {WRITE_DATA_8, 0x16},
      {WRITE_DATA_8, 0x19},
      {WRITE_DATA_8, 0x03},
      {WRITE_DATA_8, 0x0F},
      {WRITE_DATA_8, 0x05},
      {WRITE_DATA_8, 0x32},
      {WRITE_DATA_8, 0x45},
      {WRITE_DATA_8, 0x46},
      {WRITE_DATA_8, 0x04},
      {WRITE_DATA_8, 0x0E},
      {WRITE_DATA_8, 0x0D},
      {WRITE_DATA_8, 0x35},
      {WRITE_DATA_8, 0x37},
      {WRITE_DATA_8, 0x0F},

      {WRITE_COMMAND_8, 0XC0}, //Power Control 1
      {WRITE_DATA_8, 0x17},    //Vreg1out
      {WRITE_DATA_8, 0x15},    //Verg2out

      {WRITE_COMMAND_8, 0xC1}, //Power Control 2
      {WRITE_DATA_8, 0x41},    //VGH,VGL

      {WRITE_COMMAND_8, 0xC5}, //Power Control 3
      {WRITE_DATA_8, 0x00},
      {WRITE_DATA_8, 0x12}, //Vcom
      {WRITE_DATA_8, 0x80},

      {WRITE_COMMAND_8, 0x36}, //Memory Access
      {WRITE_DATA_8, 0x48},

      {WRITE_COMMAND_8, 0x3A}, // Interface Pixel Format
      {WRITE_DATA_8, 0x66},    //18 bit

      {WRITE_COMMAND_8, 0XB0}, // Interface Mode Control
      {WRITE_DATA_8, 0x80},    //SDO NOT USE

      {WRITE_COMMAND_8, 0xB1}, //Frame rate
      {WRITE_DATA_8, 0xA0},    //60Hz

      {WRITE_COMMAND_8, 0xB4}, //Display Inversion Control
      {WRITE_DATA_8, 0x02},    //2-dot

      {WRITE_COMMAND_8, 0XB6}, //Display Function Control  RGB/MCU Interface Control

      {WRITE_DATA_8, 0x02}, //MCU
      {WRITE_DATA_8, 0x02}, //Source,Gate scan dieection

      {WRITE_COMMAND_8, 0XE9}, // Set Image Functio
      {WRITE_DATA_8, 0x00},    // Disable 24 bit data

      {WRITE_COMMAND_8, 0xF7}, // Adjust Control
      {WRITE_DATA_8, 0xA9},
      {WRITE_DATA_8, 0x51},
      {WRITE_DATA_8, 0x2C},
      {WRITE_DATA_8, 0x82}, // D7 stream, loose

      {WRITE_COMMAND_8, ILI9488_SLPOUT}, //Exit Sleep
      {END_WRITE, 0},
      {DELAY, ILI9488_SLPOUT_DELAY},
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_8, ILI9488_DISPON}, //Display on
      {END_WRITE, 0},
  };

  _bus->batchOperation(ili9488_init_operations, sizeof(ili9488_init_operations) / sizeof(ili9488_init_operations[0]));
}

void Arduino_ILI9488_18bit::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    uint16_t x_start = x + _xStart, x_end = x + w - 1 + _xStart;

    _bus->writeCommand(ILI9488_CASET); // Column addr set
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

    _bus->writeCommand(ILI9488_PASET); // Row addr set
    _bus->write(y_start >> 8);
    _bus->write(y_start & 0xFF); // YSTART
    _bus->write(y_end >> 8);
    _bus->write(y_end & 0xFF); // YEND

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(ILI9488_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ILI9488_18bit::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    r = (ILI9488_MAD_BGR | ILI9488_MAD_MY);
    break;
  case 1:
    r = (ILI9488_MAD_BGR | ILI9488_MAD_MV | ILI9488_MAD_MX | ILI9488_MAD_MY);
    break;
  case 2:
    r = (ILI9488_MAD_BGR | ILI9488_MAD_MX);
    break;
  case 3:
    r = (ILI9488_MAD_BGR | ILI9488_MAD_MV);
    break;
  }

  _bus->beginWrite();
  _bus->writeC8D8(ILI9488_MADCTL, r);
  _bus->endWrite();
}

void Arduino_ILI9488_18bit::invertDisplay(bool i)
{
  _bus->sendCommand(i ? ILI9488_INVON : ILI9488_INVOFF);
}

void Arduino_ILI9488_18bit::displayOn(void)
{
  _bus->sendCommand(ILI9488_SLPOUT);
  delay(ILI9488_SLPOUT_DELAY);
}

void Arduino_ILI9488_18bit::displayOff(void)
{
  _bus->sendCommand(ILI9488_SLPIN);
  delay(ILI9488_SLPIN_DELAY);
}
