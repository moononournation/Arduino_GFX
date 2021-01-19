/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/gitcnd/LCDWIKI_SPI.git
 */
#include "Arduino_JBT6K71.h"
#include "SPI.h"

Arduino_JBT6K71::Arduino_JBT6K71(
    Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_JBT6K71::begin(int32_t speed)
{
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_JBT6K71::tftInit()
{
  uint16_t display_mode = _ips ? 0x4004 : 0x4000; //64K Colors
  spi_operation_t jbt6k71_init_operations[] = {
      {SEND_COMMAND_16, 0x0000}, //exiting from deep standby mode
      {DELAY, 10},               //spec 1ms
      {SEND_COMMAND_16, 0x0000},
      {DELAY, 10}, //spec 1ms
      {SEND_COMMAND_16, 0x0000},
      {DELAY, 10}, //spec 1ms

      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x001d}, //mode setting
      {WRITE_DATA_16, 0x0005},    //exit standby
      {END_WRITE, 0},
      {DELAY, 100}, //spec 1ms

      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0000}, //oscillation setting
      {WRITE_DATA_16, 0x0001},    //set to on
      {END_WRITE, 0},
      {DELAY, 100}, //spec 1ms

      //Display control
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0002}, //LCD driver AC control
      {WRITE_DATA_16, 0x0200},    //line inversion
      {WRITE_COMMAND_16, 0x0007}, //Display mode
      {WRITE_DATA_16, display_mode},
      {WRITE_COMMAND_16, 0x000d}, //FR period adjustment setting
      {WRITE_DATA_16, 0x0011},    //Ffr=60Hz optimized
      {END_WRITE, 0},
      {DELAY, 100}, //spec 1ms

      //LTPS control settings
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0012}, //LTPS control setting 1
      {WRITE_DATA_16, 0x0303},
      {WRITE_COMMAND_16, 0x0013}, //LTPS control setting 2
      {WRITE_DATA_16, 0x0102},
      {WRITE_COMMAND_16, 0x001c}, //Amplifier capability setting
      {WRITE_DATA_16, 0x0000},    //Maximum

      //Power settings
      {WRITE_COMMAND_16, 0x0102}, //Power supply control (1)
      {WRITE_DATA_16, 0x00f6},    //VCOMD Output voltage: 1.4V(Initial), VCS output voltage: 4.5V, VGM output voltage: 4.3V
      {END_WRITE, 0},
      {DELAY, 500},

      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0103}, //Power Supply Control (2)
      {WRITE_DATA_16, 0x0007},    //Boosting clock mode: Dual mode, XVDD output voltage: 5.4V
      {END_WRITE, 0},
      {DELAY, 100},

      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0105}, //Power supply control (4)
      {WRITE_DATA_16, 0x0111},    //Mask period (DCEW1/DCEW2): 1.0 clock, DCCLK frequency for external regulate circuit: 1H, DCCLK frequency for XVDD regulate circuit: 1/2H, DCCLK frequency for AVDD regulate circuit: 1H
      {END_WRITE, 0},
      {DELAY, 100},

      //Gray scale settings (gamma c
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0300},
      {WRITE_DATA_16, 0x0200}, //chan
      {WRITE_COMMAND_16, 0x0301},
      {WRITE_DATA_16, 0x0002}, //
      {WRITE_COMMAND_16, 0x0302},
      {WRITE_DATA_16, 0x0000},
      {WRITE_COMMAND_16, 0x0303},
      {WRITE_DATA_16, 0x0300}, //
      {WRITE_COMMAND_16, 0x0304},
      {WRITE_DATA_16, 0x0700},
      {WRITE_COMMAND_16, 0x0305},
      {WRITE_DATA_16, 0x0070}, //

      {WRITE_COMMAND_16, 0x0402}, //First screen start
      {WRITE_DATA_16, 0x0000},    //0
      {WRITE_COMMAND_16, 0x0403}, //First screen end
      {WRITE_DATA_16, 0x013f},    //319

      //	{WRITE_COMMAND_16, 0x0404);  //Second screen start
      //  {WRITE_DATA_16, 10);  //0
      //  {WRITE_COMMAND_16, 0x0405);  //Second screen end
      //  {WRITE_DATA_16, 2);  //319

      {WRITE_COMMAND_16, 0x0100}, //Display Control
      {WRITE_DATA_16, 0xC010},
      {END_WRITE, 0},
      {DELAY, 500},

      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_16, 0x0101}, //Auto sequence Control
      {WRITE_DATA_16, 0x0001},    //AUTO
      {WRITE_COMMAND_16, 0x0100}, //Display Control
      {WRITE_DATA_16, 0xF7FE},
      {END_WRITE, 0},
      {DELAY, 800},
  };

  _bus->batchOperation(jbt6k71_init_operations, sizeof(jbt6k71_init_operations) / sizeof(jbt6k71_init_operations[0]));
}

void Arduino_JBT6K71::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t cmd1, cmd2, cmd3;

  if ((x != _currentX) || (w != _currentW))
  {
    uint16_t x_start, x_end, x_pos;

    if (_rotation & 0x01) // Landscape
    {
      cmd1 = 0x0408;
      cmd2 = 0x0409;
      cmd3 = 0x0201;
    }
    else
    {
      cmd1 = 0x0406;
      cmd2 = 0x0407;
      cmd3 = 0x0200;
    }
    if (_rotation == 1)
    {
      x_start = JBT6K71_TFTHEIGHT - x - w - _xStart;
      x_end = JBT6K71_TFTHEIGHT - x - 1 - _xStart;
      x_pos = x_end;
    }
    else
    {
      x_start = x + _xStart;
      x_end = x + w - 1 + _xStart;
      x_pos = x_start;
    }
    _bus->writeCommand16(cmd1);
    _bus->write16(x_start);
    _bus->writeCommand16(cmd2);
    _bus->write16(x_end);
    _bus->writeCommand16(cmd3);
    _bus->write16(x_pos);

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    uint16_t y_start, y_end, y_pos;

    if (_rotation & 0x01) // Portrait
    {
      cmd1 = 0x0406;
      cmd2 = 0x0407;
      cmd3 = 0x0200;
    }
    else
    {
      cmd1 = 0x0408;
      cmd2 = 0x0409;
      cmd3 = 0x0201;
    }
    if (_rotation == 0)
    {
      y_start = JBT6K71_TFTHEIGHT - y - h - _yStart;
      y_end = JBT6K71_TFTHEIGHT - y - 1 - _yStart;
      y_pos = y_end;
    }
    else
    {
      y_start = y + _yStart;
      y_end = y + h - 1 + _yStart;
      y_pos = y_start;
    }
    _bus->writeCommand16(cmd1);
    _bus->write16(y_start);
    _bus->writeCommand16(cmd2);
    _bus->write16(y_end);
    _bus->writeCommand16(cmd3);
    _bus->write16(y_pos);

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand16(0x0202); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_JBT6K71::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  uint16_t output_control, entry_mode;
  switch (_rotation)
  {
  case 0:
    output_control = 0x0027; //SS=0
    entry_mode = 0x0010;     //ID=01
    break;
  case 1:
    output_control = 0x0127; //SS=1
    entry_mode = 0x0018;
    break;
  case 2:
    output_control = 0x0127; //SS=1
    entry_mode = 0x0030;
    break;
  default: // 3
    output_control = 0x0027; //SS=0
    entry_mode = 0x0038;
    break;
  }
  _bus->beginWrite();
  _bus->writeCommand16(0x0001); //Driver output control
  _bus->write16(output_control);
  _bus->writeCommand16(0x0003); //Entry mode
  _bus->write16(entry_mode);
  _bus->endWrite();
}

void Arduino_JBT6K71::invertDisplay(bool i)
{
  // Not Implemented
  UNUSED(i);
}

void Arduino_JBT6K71::displayOn(void)
{
  // Not Implemented
}

void Arduino_JBT6K71::displayOff(void)
{
  // Not Implemented
}
