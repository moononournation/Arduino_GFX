#include "../Arduino_DataBus.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)

#include "../Arduino_GFX.h"
#include "Arduino_ST7701_RGBPanel.h"

Arduino_ST7701_RGBPanel::Arduino_ST7701_RGBPanel(
    Arduino_ESP32RGBPanel *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    const uint8_t *init_operations, size_t init_operations_len,
    bool bgr)
    : Arduino_GFX(w, h), _bus(bus), _rst(rst), _ips(ips),
      _init_operations(init_operations), _init_operations_len(init_operations_len),
      _bgr(bgr)
{
    _rotation = r;
}

void Arduino_ST7701_RGBPanel::begin(int32_t speed)
{
    _bus->begin(speed);

    if (_rst != GFX_NOT_DEFINED)
    {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(100);
        digitalWrite(_rst, LOW);
        delay(120);
        digitalWrite(_rst, HIGH);
        delay(120);
    }

#if 1 
_bus->sendCommand(0xFF);
_bus->sendData(0x77);
_bus->sendData(0x01);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x10);

_bus->sendCommand(0xC0);
_bus->sendData(0x3B);
_bus->sendData(0x00);

_bus->sendCommand(0xC1);
_bus->sendData(0x0B); //VBP
_bus->sendData(0x02);

_bus->sendCommand(0xC2);
_bus->sendData(0x00);
_bus->sendData(0x02);

_bus->sendCommand(0xCC);
_bus->sendData(0x10);

_bus->sendCommand(0xCD);
_bus->sendData(0x08);

_bus->sendCommand ( 0xB0); //Positive Voltage Gamma Control
_bus->sendData ( 0x02);
_bus->sendData ( 0x13);
_bus->sendData ( 0x1B);
_bus->sendData ( 0x0D);
_bus->sendData ( 0x10);
_bus->sendData ( 0x05);
_bus->sendData ( 0x08);
_bus->sendData ( 0x07);
_bus->sendData ( 0x07);
_bus->sendData ( 0x24);
_bus->sendData ( 0x04);
_bus->sendData ( 0x11);
_bus->sendData ( 0x0E);
_bus->sendData ( 0x2C);
_bus->sendData ( 0x33);
_bus->sendData ( 0x1D);

_bus->sendCommand ( 0xB1); //Negative Voltage Gamma Control
_bus->sendData ( 0x05);
_bus->sendData ( 0x13);
_bus->sendData ( 0x1B);
_bus->sendData ( 0x0D);
_bus->sendData ( 0x11);
_bus->sendData ( 0x05);
_bus->sendData ( 0x08);
_bus->sendData ( 0x07);
_bus->sendData ( 0x07);
_bus->sendData ( 0x24);
_bus->sendData ( 0x04);
_bus->sendData ( 0x11);
_bus->sendData ( 0x0E);
_bus->sendData ( 0x2C);
_bus->sendData ( 0x33);
_bus->sendData ( 0x1D);

_bus->sendCommand(0xFF);
_bus->sendData(0x77);
_bus->sendData(0x01);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x11);

_bus->sendCommand(0xB0);
_bus->sendData(0x5d);//5d

_bus->sendCommand(0xB1); //VCOM amplitude setting
_bus->sendData(0x43); //43

_bus->sendCommand(0xB2); //VGH Voltage setting
_bus->sendData(0x81); //12V

_bus->sendCommand(0xB3);
_bus->sendData(0x80);

_bus->sendCommand(0xB5); //VGL Voltage setting
_bus->sendData(0x43); //-8.3V

_bus->sendCommand(0xB7);
_bus->sendData(0x85);

_bus->sendCommand(0xB8);
_bus->sendData(0x20);

_bus->sendCommand(0xC1);
_bus->sendData(0x78);

_bus->sendCommand(0xC2);
_bus->sendData(0x78);

_bus->sendCommand(0xD0);
_bus->sendData(0x88);

_bus->sendCommand(0xE0);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x02);

_bus->sendCommand(0xE1);
_bus->sendData(0x03);
_bus->sendData(0xA0);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x04);
_bus->sendData(0xA0);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x20);
_bus->sendData(0x20);

_bus->sendCommand(0xE2);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);

_bus->sendCommand(0xE3);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x11);
_bus->sendData(0x00);

_bus->sendCommand(0xE4);
_bus->sendData(0x22);
_bus->sendData(0x00);

_bus->sendCommand(0xE5);
_bus->sendData(0x05);
_bus->sendData(0xEC);
_bus->sendData(0xA0);
_bus->sendData(0xA0);
_bus->sendData(0x07);
_bus->sendData(0xEE);
_bus->sendData(0xA0);
_bus->sendData(0xA0);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);

_bus->sendCommand(0xE6);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x11);
_bus->sendData(0x00);

_bus->sendCommand(0xE7);
_bus->sendData(0x22);
_bus->sendData(0x00);

_bus->sendCommand(0xE8);
_bus->sendData(0x06);
_bus->sendData(0xED);
_bus->sendData(0xA0);
_bus->sendData(0xA0);
_bus->sendData(0x08);
_bus->sendData(0xEF);
_bus->sendData(0xA0);
_bus->sendData(0xA0);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);

_bus->sendCommand(0xEB);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x40);
_bus->sendData(0x40);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);

_bus->sendCommand(0xED);
_bus->sendData(0xFF);
_bus->sendData(0xFF);
_bus->sendData(0xFF);
_bus->sendData(0xBA);
_bus->sendData(0x0A);
_bus->sendData(0xBF);
_bus->sendData(0x45);
_bus->sendData(0xFF);
_bus->sendData(0xFF);
_bus->sendData(0x54);
_bus->sendData(0xFB);
_bus->sendData(0xA0);
_bus->sendData(0xAB);
_bus->sendData(0xFF);
_bus->sendData(0xFF);
_bus->sendData(0xFF);

_bus->sendCommand(0xEF);
_bus->sendData(0x10);
_bus->sendData(0x0D);
_bus->sendData(0x04);
_bus->sendData(0x08);
_bus->sendData(0x3F);
_bus->sendData(0x1F);

_bus->sendCommand(0xFF);
_bus->sendData(0x77);
_bus->sendData(0x01);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x13);

_bus->sendCommand(0xEF);
_bus->sendData(0x08);

_bus->sendCommand(0xFF);
_bus->sendData(0x77);
_bus->sendData(0x01);
_bus->sendData(0x00);
_bus->sendData(0x00);
_bus->sendData(0x00);

#if 0
_bus->_bus->sendCommand (0xFF);
WriteData (0x77);
WriteData (0x01);
WriteData (0x00);
WriteData (0x00);
WriteData (0x12);
_bus->_bus->sendCommand (0xD1);
WriteData (0x81);
WriteData (0x08);
WriteData (0x03);
WriteData (0x20);
WriteData (0x08);
WriteData (0x01);
WriteData (0xA0);
WriteData (0x01);
WriteData (0xE0);
WriteData (0xA0);
WriteData (0x01);
WriteData (0xE0);
WriteData (0x03);
WriteData (0x20);
_bus->_bus->sendCommand (0xD2);
WriteData (0x08);
#endif
/////////////////Bring up the internal test picture///////////////////////////////////

_bus->sendCommand(0x11);

delay(120);

_bus->sendCommand(0x29);

_bus->sendCommand(0x36);
_bus->sendData(0x00);

_bus->sendCommand(0x3A);
_bus->sendData(0x60);//0x60 18bit 0x50 16bit
#endif

    // _bus->batchOperation((uint8_t *)_init_operations, _init_operations_len);

    // if (_ips)
    // {
    //     _bus->beginWrite();
    //     _bus->writeCommand(0x21);
    //     _bus->endWrite();
    // }

    setRotation(_rotation);

    _framebuffer = _bus->getFrameBuffer(_width, _height);
}

void Arduino_ST7701_RGBPanel::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
    uint16_t *fb = _framebuffer;
    fb += (int32_t)y * _width;
    fb += x;
    *fb = color;
    Cache_WriteBack_Addr((uint32_t)fb, 2);
}

void Arduino_ST7701_RGBPanel::writeFastVLine(int16_t x, int16_t y,
                                             int16_t h, uint16_t color)
{
    if (_ordered_in_range(x, 0, _max_x) && h)
    { // X on screen, nonzero height
        if (h < 0)
        {               // If negative height...
            y += h + 1; //   Move Y to top edge
            h = -h;     //   Use positive height
        }
        if (y <= _max_y)
        { // Not off bottom
            int16_t y2 = y + h - 1;
            if (y2 >= 0)
            { // Not off top
                // Line partly or fully overlaps screen
                if (y < 0)
                {
                    y = 0;
                    h = y2 + 1;
                } // Clip top
                if (y2 > _max_y)
                {
                    h = _max_y - y + 1;
                } // Clip bottom

                uint16_t *fb = _framebuffer + ((int32_t)y * _width) + x;
                while (h--)
                {
                    *fb = color;
                    Cache_WriteBack_Addr((uint32_t)fb, 2);
                    fb += _width;
                }
            }
        }
    }
}

void Arduino_ST7701_RGBPanel::writeFastHLine(int16_t x, int16_t y,
                                             int16_t w, uint16_t color)
{
    if (_ordered_in_range(y, 0, _max_y) && w)
    { // Y on screen, nonzero width
        if (w < 0)
        {               // If negative width...
            x += w + 1; //   Move X to left edge
            w = -w;     //   Use positive width
        }
        if (x <= _max_x)
        { // Not off right
            int16_t x2 = x + w - 1;
            if (x2 >= 0)
            { // Not off left
                // Line partly or fully overlaps screen
                if (x < 0)
                {
                    x = 0;
                    w = x2 + 1;
                } // Clip left
                if (x2 > _max_x)
                {
                    w = _max_x - x + 1;
                } // Clip right

                uint16_t *fb = _framebuffer + ((int32_t)y * _width) + x;
                uint32_t cachePos = (uint32_t)fb;
                int16_t writeSize = w * 2;
                while (w--)
                {
                    *(fb++) = color;
                }
                Cache_WriteBack_Addr(cachePos, writeSize);
            }
        }
    }
}

void Arduino_ST7701_RGBPanel::writeFillRectPreclipped(int16_t x, int16_t y,
                                                      int16_t w, int16_t h, uint16_t color)
{
    uint16_t *row = _framebuffer;
    row += y * _width;
    uint32_t cachePos = (uint32_t)row;
    row += x;
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            row[i] = color;
        }
        row += _width;
    }
    Cache_WriteBack_Addr(cachePos, _width * h * 2);
}

void Arduino_ST7701_RGBPanel::draw16bitRGBBitmap(int16_t x, int16_t y,
                                                 uint16_t *bitmap, int16_t w, int16_t h)
{
    if (
        ((x + w - 1) < 0) || // Outside left
        ((y + h - 1) < 0) || // Outside top
        (x > _max_x) ||      // Outside right
        (y > _max_y)         // Outside bottom
    )
    {
        return;
    }
    else
    {
        int16_t xskip = 0;
        if ((y + h - 1) > _max_y)
        {
            h -= (y + h - 1) - _max_y;
        }
        if (y < 0)
        {
            bitmap -= y * w;
            h += y;
            y = 0;
        }
        if ((x + w - 1) > _max_x)
        {
            xskip = (x + w - 1) - _max_x;
            w -= xskip;
        }
        if (x < 0)
        {
            bitmap -= x;
            xskip -= x;
            w += x;
            x = 0;
        }
        uint16_t *row = _framebuffer;
        row += y * _width;
        uint32_t cachePos = (uint32_t)row;
        row += x;
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                row[i] = *bitmap++;
            }
            bitmap += xskip;
            row += _width;
        }
        Cache_WriteBack_Addr(cachePos, _width * h * 2);
    }
}

void Arduino_ST7701_RGBPanel::draw16bitBeRGBBitmap(int16_t x, int16_t y,
                                                   uint16_t *bitmap, int16_t w, int16_t h)
{
    if (
        ((x + w - 1) < 0) || // Outside left
        ((y + h - 1) < 0) || // Outside top
        (x > _max_x) ||      // Outside right
        (y > _max_y)         // Outside bottom
    )
    {
        return;
    }
    else
    {
        int16_t xskip = 0;
        if ((y + h - 1) > _max_y)
        {
            h -= (y + h - 1) - _max_y;
        }
        if (y < 0)
        {
            bitmap -= y * w;
            h += y;
            y = 0;
        }
        if ((x + w - 1) > _max_x)
        {
            xskip = (x + w - 1) - _max_x;
            w -= xskip;
        }
        if (x < 0)
        {
            bitmap -= x;
            xskip -= x;
            w += x;
            x = 0;
        }
        uint16_t *row = _framebuffer;
        row += y * _width;
        uint32_t cachePos = (uint32_t)row;
        row += x;
        uint16_t color;
        for (int j = 0; j < h; j++)
        {
            for (int i = 0; i < w; i++)
            {
                color = *bitmap++;
                MSB_16_SET(row[i], color);
            }
            bitmap += xskip;
            row += _width;
        }
        Cache_WriteBack_Addr(cachePos, _width * h * 2);
    }
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_ST7701_RGBPanel::setRotation(uint8_t r)
{
    Arduino_GFX::setRotation(r);
    _bus->beginWrite();
    switch (_rotation)
    {
    case 1:
        // not implemented
        break;
    case 2:
        // Y direction
        _bus->writeCommand(0xFF);
        _bus->write(0x77);
        _bus->write(0x01);
        _bus->write(0x00);
        _bus->write(0x00);
        _bus->write(0x10);
        _bus->writeCommand(0xC7);
        _bus->write(0x04);
        // X Direction and color order
        _bus->writeCommand(0xFF);
        _bus->write(0x77);
        _bus->write(0x01);
        _bus->write(0x00);
        _bus->write(0x00);
        _bus->write(0x00);
        _bus->writeCommand(0x36);
        _bus->write(_bgr ? 0x10 : 0x18);
        break;
    case 3:
        // not implemented
        break;
    default: // case 0:
        // Y direction
        _bus->writeCommand(0xFF);
        _bus->write(0x77);
        _bus->write(0x01);
        _bus->write(0x00);
        _bus->write(0x00);
        _bus->write(0x10);
        _bus->writeCommand(0xC7);
        _bus->write(0x00);
        // X Direction and color order
        _bus->writeCommand(0xFF);
        _bus->write(0x77);
        _bus->write(0x01);
        _bus->write(0x00);
        _bus->write(0x00);
        _bus->write(0x00);
        _bus->writeCommand(0x36);
        _bus->write(_bgr ? 0x00 : 0x08);
        break;
    }
    _bus->endWrite();
}

void Arduino_ST7701_RGBPanel::invertDisplay(bool i)
{
  _bus->sendCommand(_ips ? (i ? 0x20 : 0x21) : (i ? 0x21 : 0x20));
}

uint16_t *Arduino_ST7701_RGBPanel::getFramebuffer()
{
    return _framebuffer;
}

#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
