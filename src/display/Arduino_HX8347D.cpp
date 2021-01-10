/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_HX8347D.h"
#include "SPI.h"

Arduino_HX8347D::Arduino_HX8347D(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_HX8347D::begin(int32_t speed)
{
#if defined(__AVR__)
  _override_datamode = SPI_MODE0;
#endif
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_HX8347D::tftInit()
{
  spi_operation_t hx8347d_init_operations[] = {
      {BEGIN_WRITE, 0},
      {WRITE_COMMAND_8, 0xEA},
      {WRITE_DATA_8, 0x00},
      {WRITE_COMMAND_8, 0xEB},
      {WRITE_DATA_8, 0x20},
      {WRITE_COMMAND_8, 0xEC},
      {WRITE_DATA_8, 0x0C},
      {WRITE_COMMAND_8, 0xED},
      {WRITE_DATA_8, 0xC4},
      {WRITE_COMMAND_8, 0xE8},
      {WRITE_DATA_8, 0x38},
      {WRITE_COMMAND_8, 0xE9},
      {WRITE_DATA_8, 0x10},
      {WRITE_COMMAND_8, 0xF1},
      {WRITE_DATA_8, 0x01},
      {WRITE_COMMAND_8, 0xF2},
      {WRITE_DATA_8, 0x10},
      {WRITE_COMMAND_8, 0x40},
      {WRITE_DATA_8, 0x01},
      {WRITE_COMMAND_8, 0x41},
      {WRITE_DATA_8, 0x00},
      {WRITE_COMMAND_8, 0x42},
      {WRITE_DATA_8, 0x00},
      {WRITE_COMMAND_8, 0x43},
      {WRITE_DATA_8, 0x10},
      {WRITE_COMMAND_8, 0x44},
      {WRITE_DATA_8, 0x0E},
      {WRITE_COMMAND_8, 0x45},
      {WRITE_DATA_8, 0x24},
      {WRITE_COMMAND_8, 0x46},
      {WRITE_DATA_8, 0x04},
      {WRITE_COMMAND_8, 0x47},
      {WRITE_DATA_8, 0x50},
      {WRITE_COMMAND_8, 0x48},
      {WRITE_DATA_8, 0x02},
      {WRITE_COMMAND_8, 0x49},
      {WRITE_DATA_8, 0x13},
      {WRITE_COMMAND_8, 0x4A},
      {WRITE_DATA_8, 0x19},
      {WRITE_COMMAND_8, 0x4B},
      {WRITE_DATA_8, 0x19},
      {WRITE_COMMAND_8, 0x4C},
      {WRITE_DATA_8, 0x16},
      {WRITE_COMMAND_8, 0x50},
      {WRITE_DATA_8, 0x1B},
      {WRITE_COMMAND_8, 0x51},
      {WRITE_DATA_8, 0x31},
      {WRITE_COMMAND_8, 0x52},
      {WRITE_DATA_8, 0x2F},
      {WRITE_COMMAND_8, 0x53},
      {WRITE_DATA_8, 0x3F},
      {WRITE_COMMAND_8, 0x54},
      {WRITE_DATA_8, 0x3F},
      {WRITE_COMMAND_8, 0x55},
      {WRITE_DATA_8, 0x3E},
      {WRITE_COMMAND_8, 0x56},
      {WRITE_DATA_8, 0x2F},
      {WRITE_COMMAND_8, 0x57},
      {WRITE_DATA_8, 0x7B},
      {WRITE_COMMAND_8, 0x58},
      {WRITE_DATA_8, 0x09},
      {WRITE_COMMAND_8, 0x59},
      {WRITE_DATA_8, 0x06},
      {WRITE_COMMAND_8, 0x5A},
      {WRITE_DATA_8, 0x06},
      {WRITE_COMMAND_8, 0x5B},
      {WRITE_DATA_8, 0x0C},
      {WRITE_COMMAND_8, 0x5C},
      {WRITE_DATA_8, 0x1D},
      {WRITE_COMMAND_8, 0x5D},
      {WRITE_DATA_8, 0xCC},
      {WRITE_COMMAND_8, 0x1B},
      {WRITE_DATA_8, 0x1B},
      {WRITE_COMMAND_8, 0x1A},
      {WRITE_DATA_8, 0x01},
      {WRITE_COMMAND_8, 0x24},
      {WRITE_DATA_8, 0x2F},
      {WRITE_COMMAND_8, 0x25},
      {WRITE_DATA_8, 0x57},
      {WRITE_COMMAND_8, 0x23},
      {WRITE_DATA_8, 0x88},
      {WRITE_COMMAND_8, 0x18},
      {WRITE_DATA_8, 0x34},
      {WRITE_COMMAND_8, 0x19},
      {WRITE_DATA_8, 0x01},
      {WRITE_COMMAND_8, 0x1F},
      {WRITE_DATA_8, 0x88},
      {WRITE_COMMAND_8, 0x1F},
      {WRITE_DATA_8, 0x80},
      {WRITE_COMMAND_8, 0x1F},
      {WRITE_DATA_8, 0x90},
      {WRITE_COMMAND_8, 0x1F},
      {WRITE_DATA_8, 0xD0},
      {WRITE_COMMAND_8, 0x17},
      {WRITE_DATA_8, 0x05},
      {WRITE_COMMAND_8, 0x28},
      {WRITE_DATA_8, 0x38},
      {WRITE_COMMAND_8, 0x28},
      {WRITE_DATA_8, 0x3F},
      {WRITE_COMMAND_8, 0x16},
      {WRITE_DATA_8, 0x18},
      {END_WRITE, 0},
  };

  _bus->batchOperation(hx8347d_init_operations, sizeof(hx8347d_init_operations) / sizeof(hx8347d_init_operations[0]));

  if (_ips)
  {
    _bus->beginWrite();
    _bus->writeC8D8(0x01, 0x02);
    _bus->endWrite();
  }
}

void Arduino_HX8347D::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    uint16_t x_start = x + _xStart, x_end = x + w - 1 + _xStart;
    _bus->writeC8D8(0x02, x_start >> 8);
    _bus->writeC8D8(0x03, x_start & 0xFF);
    _bus->writeC8D8(0x04, x_end >> 8);
    _bus->writeC8D8(0x05, x_end & 0xFF);

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    uint16_t y_start = y + _yStart, y_end = y + h - 1 + _yStart;
    _bus->writeC8D8(0x06, y_start >> 8);
    _bus->writeC8D8(0x07, y_start & 0xFF);
    _bus->writeC8D8(0x08, y_end >> 8);
    _bus->writeC8D8(0x09, y_end & 0xFF);

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(0x22); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_HX8347D::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  _bus->beginWrite();
  switch (_rotation)
  {
  case 0:
    _bus->writeC8D8(0x36, 0x07);
    _bus->writeC8D8(0x16, 0x40);
    break;
  case 1:
    _bus->writeC8D8(0x36, 0x07);
    _bus->writeC8D8(0x16, 0x20);
    break;
  case 2:
    _bus->writeC8D8(0x36, 0x03);
    _bus->writeC8D8(0x16, 0x00);
    break;
  case 3:
    _bus->writeC8D8(0x36, 0x03);
    _bus->writeC8D8(0x16, 0x60);
    break;
  }
  _bus->endWrite();
}

void Arduino_HX8347D::invertDisplay(bool i)
{
  _bus->beginWrite();
  if (_ips && i)
  {
    _bus->writeC8D8(0x01, 0x00);
  }
  else
  {
    _bus->writeC8D8(0x01, 0x02);
  }
  _bus->endWrite();
}

void Arduino_HX8347D::displayOn(void)
{
  _bus->sendCommand(0x28);
  _bus->sendData(0x3C); //GON=1, DTE=1, D=11
}

void Arduino_HX8347D::displayOff(void)
{
  _bus->sendCommand(0x28);
  _bus->sendData(0x34); //GON=1, DTE=1, D=01
  delay(40);
}
