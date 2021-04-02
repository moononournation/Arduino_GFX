#include "Arduino_ILI9488_3bit.h"

Arduino_ILI9488_3bit::Arduino_ILI9488_3bit(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips)
    : Arduino_G(ILI9488_TFTWIDTH, ILI9488_TFTHEIGHT), _bus(bus), _rst(rst), _rotation(r), _ips(ips)
{
}

void Arduino_ILI9488_3bit::begin(int32_t speed)
{
  _bus->begin(speed);

  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(120);
    digitalWrite(_rst, LOW);
    delay(120);
    digitalWrite(_rst, HIGH);
    delay(120);
  }
  else
  {
    _bus->sendCommand(ILI9488_SWRST); // 1: Software reset
    delay(ILI9488_RST_DELAY);
  }

  // tftInit
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
      {WRITE_DATA_8, 0x01},    // 3 bit

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

  if (_ips)
  {
    _bus->sendCommand(ILI9488_INVON);
  }
  else
  {
    _bus->sendCommand(ILI9488_INVOFF);
  }

  uint16_t r;
  // setRotation
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

void Arduino_ILI9488_3bit::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  printf("Not Implemented drawBitmap()");
}

void Arduino_ILI9488_3bit::drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h)
{
  printf("Not Implemented drawIndexedBitmap()");
}

void Arduino_ILI9488_3bit::draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  _bus->beginWrite();
  writeAddrWindow(0, 0, w, h);
  _bus->writeBytes(bitmap, w * h / 2);
  _bus->endWrite();
}

void Arduino_ILI9488_3bit::draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  printf("Not Implemented draw16bitRGBBitmap()");
}

void Arduino_ILI9488_3bit::draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  printf("Not Implemented draw24bitRGBBitmap()");
}

void Arduino_ILI9488_3bit::invertDisplay(bool i)
{
  _bus->sendCommand(i ? ILI9488_INVON : ILI9488_INVOFF);
}

void Arduino_ILI9488_3bit::displayOn(void)
{
  _bus->sendCommand(ILI9488_SLPOUT);
  delay(ILI9488_SLPOUT_DELAY);
}

void Arduino_ILI9488_3bit::displayOff(void)
{
  _bus->sendCommand(ILI9488_SLPIN);
  delay(ILI9488_SLPIN_DELAY);
}

void Arduino_ILI9488_3bit::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  _bus->writeC8D16D16(ILI9488_CASET, x, x + w - 1);
  _bus->writeC8D16D16(ILI9488_PASET, y, y + h - 1);
  _bus->writeCommand(ILI9488_RAMWR);
}