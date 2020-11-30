#include "Arduino_ST7701.h"

#define DISPLAY_CMD(cmd, params...)                   \
{                                                     \
    const uint8_t d[] = {params};                     \
    display_cmd(cmd, sizeof(d) / sizeof(uint8_t), d); \
}

Arduino_ST7701::Arduino_ST7701(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_ST7701::begin(int speed)
{
#if defined(ESP8266) || defined(ESP32)
  if (speed == 0)
  {
    speed = 40000000;
  }
#endif
  Arduino_TFT::begin(speed);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ST7701::tftInit()
{
  Serial.println("Init Display ST7701"); 

  // Software Reset
  _bus->sendCommand(ST7701_SWRESET);
  delay(ST7701_RST_DELAY);

  // Sleep mode Off
  _bus->sendCommand(ST7701_SLPOUT);
  delay(ST7701_SLPOUT_DELAY);

  // Command2 BK0
  DISPLAY_CMD(DSI_CMD2BKX_SEL, 0x77, 0x01, 0x00, 0x00, ST7701_CMD2BK0SEL);
  DISPLAY_CMD(ST7701_BK0_LNESET, 0x3B, 0x00);
  DISPLAY_CMD(ST7701_BK0_PORCTRL, 0x0D, 0x02);  // vbp, vfp
  DISPLAY_CMD(ST7701_BK0_INVSEL, 0x21, 0x08);
  //DISPLAY_CMD(0xCD, 0x21, 0x08); // ???
  DISPLAY_CMD(ST7701_BK0_PVGAMCTRL, 0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12, 0x0F, 0xAA, 0x31, 0x18);
  DISPLAY_CMD(ST7701_BK0_NVGAMCTRL, 0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x18, 0x08, 0x08, 0x22, 0x04, 0x11, 0x11, 0xA9, 0x32, 0x18);

  // Command2 BK1
  DISPLAY_CMD(DSI_CMD2BKX_SEL, 0x77, 0x01, 0x00, 0x00, ST7701_CMD2BK1SEL);
  DISPLAY_CMD(ST7701_BK1_VRHS, 0x60);
  DISPLAY_CMD(ST7701_BK1_VCOM, 0x30);
  DISPLAY_CMD(ST7701_BK1_VGHSS, 0x87);
  DISPLAY_CMD(ST7701_BK1_TESTCMD, 0x80);
  DISPLAY_CMD(ST7701_BK1_VGLS, 0x49);
  DISPLAY_CMD(ST7701_BK1_PWCTLR1, 0x85);
  DISPLAY_CMD(ST7701_BK1_PWCTLR2, 0x21);
  DISPLAY_CMD(ST7701_BK1_SPD1, 0x78);
  DISPLAY_CMD(ST7701_BK1_SPD2, 0x78);
  DISPLAY_CMD(0xD0, 0x88);

  DISPLAY_CMD(0xE0, 0x00, 0x1B, 0x02);
  DISPLAY_CMD(0xE1, 0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44);
  DISPLAY_CMD(0xE2, 0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00);
  DISPLAY_CMD(0xE3, 0x00, 0x00, 0x11, 0x11);
  DISPLAY_CMD(0xE4, 0x44, 0x44);
  DISPLAY_CMD(0xE5, 0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0, 0x10, 0xEF, 0xD8, 0xA0);
  DISPLAY_CMD(0xE6, 0x00, 0x00, 0x11, 0x11);
  DISPLAY_CMD(0xE7, 0x44, 0x44);
  DISPLAY_CMD(0xE8, 0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0, 0x0F, 0xEE, 0xD8, 0xA0);
  DISPLAY_CMD(0xEB, 0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40);
  DISPLAY_CMD(0xEC, 0x3C, 0x00);
  DISPLAY_CMD(0xED, 0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x45, 0x67, 0x98, 0xBA);
  DISPLAY_CMD(0xEF, 0x10, 0x0D, 0x04, 0x08, 0x3F, 0x1F);

  // Command2 BK2
  DISPLAY_CMD(DSI_CMD2BKX_SEL, 0x77, 0x01, 0x00, 0x00, ST7701_CMD2BKxSEL_NONE);

  // set pixel format
  DISPLAY_CMD(ST7701_COLMOD, 0x66);

  // Normal Mode On
  _bus->sendCommand(ST7701_NORON);

  // Display On
  _bus->sendCommand(ST7701_DISPON);

  if (_ips)
  {
    _bus->sendCommand(ST7701_INVON);
  }

  // All Pixels on
  //_bus->sendCommand(ST7701_ALLPON);

  Serial.println("End Display ST7701 initialization");
}
 // TODO: it does not work
void Arduino_ST7701::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  Serial.print("writeAddrWindow: ");
  if ((x != _currentX) || (w != _currentW))
  {
    _bus->writeC8D16D16(ST7701_CASET, x + _xStart, x + w - 1 + _xStart);

    _currentX = x;
    _currentW = w;
  }
  if ((y != _currentY) || (h != _currentH))
  {
    _bus->writeC8D16D16(ST7701_RASET, y + _yStart, y + h - 1 + _yStart);

    _currentY = y;
    _currentH = h;
  }

  _bus->writeCommand(ST7701_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ST7701::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 0:
    // r = ST7701_MADCTL_MX | ST7701_MADCTL_MY | ST7701_MADCTL_BGR | ST7701_MADCTL_MH;
    r = ST7701_MADCTL_MY | ST7701_MADCTL_BGR;
    break;

  case 1:
    // r = ST7701_MADCTL_MY | ST7701_MADCTL_MV | ST7701_MADCTL_BGR | ST7701_MADCTL_MH;
    r = ST7701_MADCTL_MX | ST7701_MADCTL_MY | ST7701_MADCTL_MV | ST7701_MADCTL_BGR;
    break;

  case 2:
    r = ST7701_MADCTL_MX | ST7701_MADCTL_ML | ST7701_MADCTL_BGR | ST7701_MADCTL_MH;
    r = ST7701_MADCTL_MX | ST7701_MADCTL_BGR;
    break;

  case 3:
    // r = ST7701_MADCTL_MX | ST7701_MADCTL_MV | ST7701_MADCTL_BGR | ST7701_MADCTL_MH;
    r = ST7701_MADCTL_MV | ST7701_MADCTL_BGR;
    break;
  }

  _bus->beginWrite();
  _bus->writeCommand(ST7701_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_ST7701::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? ST7701_INVOFF : ST7701_INVON) : (i ? ST7701_INVON : ST7701_INVOFF));
}

void Arduino_ST7701::displayOn(void)
{
  _bus->sendCommand(ST7701_SLPOUT);
  delay(ST7701_SLPOUT_DELAY);
}

void Arduino_ST7701::displayOff(void)
{
  _bus->sendCommand(ST7701_SLPIN);
  delay(ST7701_SLPIN_DELAY);
}

void Arduino_ST7701::display_cmd(const uint8_t cmd, const uint8_t count, const uint8_t *data) {
  _bus->beginWrite();
  _bus->writeCommand(cmd);
  if (count > 0) {
    for (uint8_t i = 0; i < count; i++) {
      _bus->write(data[i]);
    }
  }
  _bus->endWrite();
}
