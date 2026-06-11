/*
 * SSD1357 driver
 * Init sequence based on:
 *   - Solomon Systech SSD1357 Rev 1.0 datasheet
 *   - WiseChip UG-6464TDDBG01 Product Specification
 */
#include "Arduino_SSD1357.h"
#include "SPI.h"

Arduino_SSD1357::Arduino_SSD1357(
    Arduino_DataBus *bus, int8_t rst, uint8_t r, int16_t w, int16_t h,
    int16_t col_offset1, int16_t row_offset1, int16_t col_offset2, int16_t row_offset2)
    : Arduino_TFT(
          bus, rst, r, false, w, h,
          (col_offset1 == GFX_NOT_DEFINED) ? ((SSD1357_TFTWIDTH - w) / 2) : col_offset1,
          (row_offset1 == GFX_NOT_DEFINED) ? 0 : row_offset1,
          (col_offset2 == GFX_NOT_DEFINED) ? ((SSD1357_TFTWIDTH - w) / 2) : col_offset2,
          (row_offset2 == GFX_NOT_DEFINED) ? 0 : row_offset2)
{
}

bool Arduino_SSD1357::begin(int32_t speed)
{
#if defined(ESP8266) || defined(ESP32)
  if (speed == GFX_NOT_DEFINED)
  {
    speed = 10000000UL; // SSD1357 SPI clock cycle time min. 100ns
  }
// Teensy 4.x
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
  if (speed == GFX_NOT_DEFINED)
  {
    speed = 10000000UL;
  }
#endif
  _override_datamode = SPI_MODE0; // always SPI_MODE0

  return Arduino_TFT::begin(speed);
}

void Arduino_SSD1357::tftInit()
{
  // Gamma Look Up Table from WiseChip UG-6464TDDBG01 spec, Section 4.5
  static const uint8_t gamma_lut[63] = {
      0x02, 0x03, 0x04, 0x05,
      0x06, 0x07, 0x08, 0x09,
      0x0A, 0x0B, 0x0C, 0x0D,
      0x0E, 0x0F, 0x10, 0x11,
      0x12, 0x13, 0x15, 0x17,
      0x19, 0x1B, 0x1D, 0x1F,
      0x21, 0x23, 0x25, 0x27,
      0x2A, 0x2D, 0x30, 0x33,
      0x36, 0x39, 0x3C, 0x3F,
      0x42, 0x45, 0x48, 0x4C,
      0x50, 0x54, 0x58, 0x5C,
      0x60, 0x64, 0x68, 0x6C,
      0x70, 0x74, 0x78, 0x7D,
      0x82, 0x87, 0x8C, 0x91,
      0x96, 0x9B, 0xA0, 0xA5,
      0xAA, 0xAF, 0xB4};

  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(SSD1357_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(SSD1357_RST_DELAY);
  }
  else
  {
    // Software Rest
  }

  _bus->sendCommand(SSD1357_COMMANDLOCK); // 0xFD, unlock commands
  _bus->sendData(0x12);

  _bus->sendCommand(SSD1357_DISPLAYOFF); // 0xAE

  _bus->sendCommand(SSD1357_CLOCKDIV); // 0xB3
  _bus->sendData(0xB0);                // Fosc max, divide ratio 1

  _bus->sendCommand(SSD1357_MUXRATIO); // 0xCA
  _bus->sendData(HEIGHT - 1);          // e.g. 0x3F for 64, 0x7F for 128 rows

  _bus->sendCommand(SSD1357_DISPLAYOFFSET);   // 0xA2, set in setRotation() as well
  _bus->sendData(SSD1357_TFTHEIGHT - HEIGHT); // 0x40 for UG-6464TDDBG01

  _bus->sendCommand(SSD1357_STARTLINE); // 0xA1
  _bus->sendData(0x00);

  _bus->sendCommand(SSD1357_SETREMAP); // 0xA0, set in setRotation() as well
  _bus->sendData(0x70);                // 65k color depth, COM split odd/even, COM remap
  _bus->sendData(0x00);

  _bus->sendCommand(SSD1357_CONTRASTABC); // 0xC1
  _bus->sendData(0x88);
  _bus->sendData(0x32);
  _bus->sendData(0x88);

  _bus->sendCommand(SSD1357_CONTRASTMASTER); // 0xC7
  _bus->sendData(0x0F);

  _bus->sendCommand(SSD1357_PRECHARGE); // 0xB1, phase length
  _bus->sendData(0x32);

  _bus->sendCommand(SSD1357_PRECHARGE2); // 0xB6, second pre-charge period
  _bus->sendData(0x01);

  _bus->sendCommand(SSD1357_SETGRAY); // 0xB8, gamma look up table
  for (uint8_t i = 0; i < 63; i++)
  {
    _bus->sendData(gamma_lut[i]);
  }

  _bus->sendCommand(SSD1357_PRECHARGELEVEL); // 0xBB
  _bus->sendData(0x17);

  _bus->sendCommand(SSD1357_VCOMH); // 0xBE
  _bus->sendData(0x05);

  _bus->sendCommand(SSD1357_NORMALDISPLAY); // 0xA6

  _bus->sendCommand(SSD1357_DISPLAYON); // Main screen turn on
}

void Arduino_SSD1357::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  uint8_t cmd;
  if ((x != _currentX) || (w != _currentW))
  {
    cmd = (_rotation & 0x01) ? SSD1357_SETROW : SSD1357_SETCOLUMN;
    uint8_t x_start = x + _xStart, x_end = x + w - 1 + _xStart;

    _bus->writeCommand(cmd); // Column addr set
    _bus->write(x_start);    // XSTART
    _bus->write(x_end);      // XEND

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    cmd = (_rotation & 0x01) ? SSD1357_SETCOLUMN : SSD1357_SETROW;
    uint8_t y_start = y + _yStart, y_end = y + h - 1 + _yStart;

    _bus->writeCommand(cmd); // Row addr set
    _bus->write(y_start);    // YSTART
    _bus->write(y_end);      // YEND

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(SSD1357_WRITERAM); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_SSD1357::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = 0b01110001;
    break;
  case 2:
    r = 0b01100000;
    break;
  case 3:
    r = 0b01100011;
    break;
  default: // case 0:
    r = 0b01110010;
    break;
  }
  uint8_t display_offset = (_rotation < 2) ? (SSD1357_TFTHEIGHT - HEIGHT) : 0;
  _bus->beginWrite();
  _bus->writeC8D16(SSD1357_SETREMAP, ((uint16_t)r << 8) | 0x00); // A0 takes 2 data bytes on SSD1357
  _bus->writeC8D8(SSD1357_DISPLAYOFFSET, display_offset);
  _bus->endWrite();
}

void Arduino_SSD1357::invertDisplay(bool i)
{
  _bus->sendCommand(i ? SSD1357_INVERTDISPLAY : SSD1357_NORMALDISPLAY);
}

void Arduino_SSD1357::displayOn(void)
{
  _bus->sendCommand(SSD1357_DISPLAYON);
}

void Arduino_SSD1357::displayOff(void)
{
  _bus->sendCommand(SSD1357_DISPLAYOFF);
}
