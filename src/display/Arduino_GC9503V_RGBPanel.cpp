#include "../Arduino_DataBus.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)

#include "../Arduino_GFX.h"
#include "Arduino_GC9503V_RGBPanel.h"

Arduino_GC9503V_RGBPanel::Arduino_GC9503V_RGBPanel(
    Arduino_ESP32RGBPanel *bus, int8_t rst, int16_t w, int16_t h)
    : Arduino_GFX(w, h), _bus(bus), _rst(rst)
{
}

void Arduino_GC9503V_RGBPanel::begin(int32_t speed)
{
    _bus->begin(speed);

	_bus->sendCommand(0xF0);
	_bus->sendData(0x55);
	_bus->sendData(0xAA);
	_bus->sendData(0x52);
	_bus->sendData(0x08);
	_bus->sendData(0x00);

	_bus->sendCommand(0xF6);
	_bus->sendData(0x5A);
	_bus->sendData(0x87);

	_bus->sendCommand(0xC1);
	_bus->sendData(0x3F);

	_bus->sendCommand(0xC2);
	_bus->sendData(0x0E);

	_bus->sendCommand(0xC6);
	_bus->sendData(0xF8);

	_bus->sendCommand(0xC9);
	_bus->sendData(0x10);

	_bus->sendCommand(0xCD);
	_bus->sendData(0x25);

	_bus->sendCommand(0xF8);
	_bus->sendData(0x8A);

	_bus->sendCommand(0xAC);
	_bus->sendData(0x45);

	_bus->sendCommand(0xA0);
	_bus->sendData(0xDD);

	_bus->sendCommand(0xA7);
	_bus->sendData(0x47);

	_bus->sendCommand(0xFA);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);

	_bus->sendCommand(0xA3);
	_bus->sendData(0xEE);

	_bus->sendCommand(0xFD);
	_bus->sendData(0x28);
	_bus->sendData(0x28);
	_bus->sendData(0x00);

	_bus->sendCommand(0x71);
	_bus->sendData(0x48);

	_bus->sendCommand(0x72);
	_bus->sendData(0x48);

	_bus->sendCommand(0x73);
	_bus->sendData(0x00);
	_bus->sendData(0x44);

	_bus->sendCommand(0x97);
	_bus->sendData(0xEE);

	_bus->sendCommand(0x83);
	_bus->sendData(0x93);

	_bus->sendCommand(0x9A);
	_bus->sendData(0x72);

	_bus->sendCommand(0x9B);
	_bus->sendData(0x5a);

	_bus->sendCommand(0x82);
	_bus->sendData(0x2c);
	_bus->sendData(0x2c);

	_bus->sendCommand(0xB1);
	_bus->sendData(0x10);

	_bus->sendCommand(0x6D);
	_bus->sendData(0x00);
	_bus->sendData(0x1F);
	_bus->sendData(0x19);
	_bus->sendData(0x1A);
	_bus->sendData(0x10);
	_bus->sendData(0x0e);
	_bus->sendData(0x0c);
	_bus->sendData(0x0a);
	_bus->sendData(0x02);
	_bus->sendData(0x07);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x1E);
	_bus->sendData(0x08);
	_bus->sendData(0x01);
	_bus->sendData(0x09);
	_bus->sendData(0x0b);
	_bus->sendData(0x0D);
	_bus->sendData(0x0F);
	_bus->sendData(0x1a);
	_bus->sendData(0x19);
	_bus->sendData(0x1f);
	_bus->sendData(0x00);

	_bus->sendCommand(0x64);
	_bus->sendData(0x38);
	_bus->sendData(0x05);
	_bus->sendData(0x01);
	_bus->sendData(0xdb);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x38);
	_bus->sendData(0x04);
	_bus->sendData(0x01);
	_bus->sendData(0xdc);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x65);
	_bus->sendData(0x38);
	_bus->sendData(0x03);
	_bus->sendData(0x01);
	_bus->sendData(0xdd);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x38);
	_bus->sendData(0x02);
	_bus->sendData(0x01);
	_bus->sendData(0xde);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x66);
	_bus->sendData(0x38);
	_bus->sendData(0x01);
	_bus->sendData(0x01);
	_bus->sendData(0xdf);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x38);
	_bus->sendData(0x00);
	_bus->sendData(0x01);
	_bus->sendData(0xe0);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x67);
	_bus->sendData(0x30);
	_bus->sendData(0x01);
	_bus->sendData(0x01);
	_bus->sendData(0xe1);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x30);
	_bus->sendData(0x02);
	_bus->sendData(0x01);
	_bus->sendData(0xe2);
	_bus->sendData(0x03);
	_bus->sendData(0x03);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x68);
	_bus->sendData(0x00);
	_bus->sendData(0x08);
	_bus->sendData(0x15);
	_bus->sendData(0x08);
	_bus->sendData(0x15);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x08);
	_bus->sendData(0x15);
	_bus->sendData(0x08);
	_bus->sendData(0x15);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x60);
	_bus->sendData(0x38);
	_bus->sendData(0x08);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x38);
	_bus->sendData(0x09);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x63);
	_bus->sendData(0x31);
	_bus->sendData(0xe4);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);
	_bus->sendData(0x31);
	_bus->sendData(0xe5);
	_bus->sendData(0x7A);
	_bus->sendData(0x7A);

	_bus->sendCommand(0x6B);
	_bus->sendData(0x07);

	_bus->sendCommand(0x7A);
	_bus->sendData(0x08);
	_bus->sendData(0x13);

	_bus->sendCommand(0x7B);
	_bus->sendData(0x08);
	_bus->sendData(0x13);

	_bus->sendCommand(0xD1);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);
	_bus->sendData(0x00);
	_bus->sendData(0x12);
	_bus->sendData(0x00);
	_bus->sendData(0x18);
	_bus->sendData(0x00);
	_bus->sendData(0x21);
	_bus->sendData(0x00);
	_bus->sendData(0x2a);
	_bus->sendData(0x00);
	_bus->sendData(0x35);
	_bus->sendData(0x00);
	_bus->sendData(0x47);
	_bus->sendData(0x00);
	_bus->sendData(0x56);
	_bus->sendData(0x00);
	_bus->sendData(0x90);
	_bus->sendData(0x00);
	_bus->sendData(0xe5);
	_bus->sendData(0x01);
	_bus->sendData(0x68);
	_bus->sendData(0x01);
	_bus->sendData(0xd5);
	_bus->sendData(0x01);
	_bus->sendData(0xd7);
	_bus->sendData(0x02);
	_bus->sendData(0x36);
	_bus->sendData(0x02);
	_bus->sendData(0xa6);
	_bus->sendData(0x02);
	_bus->sendData(0xee);
	_bus->sendData(0x03);
	_bus->sendData(0x48);
	_bus->sendData(0x03);
	_bus->sendData(0xa0);
	_bus->sendData(0x03);
	_bus->sendData(0xba);
	_bus->sendData(0x03);
	_bus->sendData(0xc5);
	_bus->sendData(0x03);
	_bus->sendData(0xd0);
	_bus->sendData(0x03);
	_bus->sendData(0xE0);
	_bus->sendData(0x03);
	_bus->sendData(0xea);
	_bus->sendData(0x03);
	_bus->sendData(0xFa);
	_bus->sendData(0x03);
	_bus->sendData(0xFF);

	_bus->sendCommand(0xD2);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);
	_bus->sendData(0x00);
	_bus->sendData(0x12);
	_bus->sendData(0x00);
	_bus->sendData(0x18);
	_bus->sendData(0x00);
	_bus->sendData(0x21);
	_bus->sendData(0x00);
	_bus->sendData(0x2a);
	_bus->sendData(0x00);
	_bus->sendData(0x35);
	_bus->sendData(0x00);
	_bus->sendData(0x47);
	_bus->sendData(0x00);
	_bus->sendData(0x56);
	_bus->sendData(0x00);
	_bus->sendData(0x90);
	_bus->sendData(0x00);
	_bus->sendData(0xe5);
	_bus->sendData(0x01);
	_bus->sendData(0x68);
	_bus->sendData(0x01);
	_bus->sendData(0xd5);
	_bus->sendData(0x01);
	_bus->sendData(0xd7);
	_bus->sendData(0x02);
	_bus->sendData(0x36);
	_bus->sendData(0x02);
	_bus->sendData(0xa6);
	_bus->sendData(0x02);
	_bus->sendData(0xee);
	_bus->sendData(0x03);
	_bus->sendData(0x48);
	_bus->sendData(0x03);
	_bus->sendData(0xa0);
	_bus->sendData(0x03);
	_bus->sendData(0xba);
	_bus->sendData(0x03);
	_bus->sendData(0xc5);
	_bus->sendData(0x03);
	_bus->sendData(0xd0);
	_bus->sendData(0x03);
	_bus->sendData(0xE0);
	_bus->sendData(0x03);
	_bus->sendData(0xea);
	_bus->sendData(0x03);
	_bus->sendData(0xFa);
	_bus->sendData(0x03);
	_bus->sendData(0xFF);

	_bus->sendCommand(0xD3);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);
	_bus->sendData(0x00);
	_bus->sendData(0x12);
	_bus->sendData(0x00);
	_bus->sendData(0x18);
	_bus->sendData(0x00);
	_bus->sendData(0x21);
	_bus->sendData(0x00);
	_bus->sendData(0x2a);
	_bus->sendData(0x00);
	_bus->sendData(0x35);
	_bus->sendData(0x00);
	_bus->sendData(0x47);
	_bus->sendData(0x00);
	_bus->sendData(0x56);
	_bus->sendData(0x00);
	_bus->sendData(0x90);
	_bus->sendData(0x00);
	_bus->sendData(0xe5);
	_bus->sendData(0x01);
	_bus->sendData(0x68);
	_bus->sendData(0x01);
	_bus->sendData(0xd5);
	_bus->sendData(0x01);
	_bus->sendData(0xd7);
	_bus->sendData(0x02);
	_bus->sendData(0x36);
	_bus->sendData(0x02);
	_bus->sendData(0xa6);
	_bus->sendData(0x02);
	_bus->sendData(0xee);
	_bus->sendData(0x03);
	_bus->sendData(0x48);
	_bus->sendData(0x03);
	_bus->sendData(0xa0);
	_bus->sendData(0x03);
	_bus->sendData(0xba);
	_bus->sendData(0x03);
	_bus->sendData(0xc5);
	_bus->sendData(0x03);
	_bus->sendData(0xd0);
	_bus->sendData(0x03);
	_bus->sendData(0xE0);
	_bus->sendData(0x03);
	_bus->sendData(0xea);
	_bus->sendData(0x03);
	_bus->sendData(0xFa);
	_bus->sendData(0x03);
	_bus->sendData(0xFF);

	_bus->sendCommand(0xD4);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);
	_bus->sendData(0x00);
	_bus->sendData(0x12);
	_bus->sendData(0x00);
	_bus->sendData(0x18);
	_bus->sendData(0x00);
	_bus->sendData(0x21);
	_bus->sendData(0x00);
	_bus->sendData(0x2a);
	_bus->sendData(0x00);
	_bus->sendData(0x35);
	_bus->sendData(0x00);
	_bus->sendData(0x47);
	_bus->sendData(0x00);
	_bus->sendData(0x56);
	_bus->sendData(0x00);
	_bus->sendData(0x90);
	_bus->sendData(0x00);
	_bus->sendData(0xe5);
	_bus->sendData(0x01);
	_bus->sendData(0x68);
	_bus->sendData(0x01);
	_bus->sendData(0xd5);
	_bus->sendData(0x01);
	_bus->sendData(0xd7);
	_bus->sendData(0x02);
	_bus->sendData(0x36);
	_bus->sendData(0x02);
	_bus->sendData(0xa6);
	_bus->sendData(0x02);
	_bus->sendData(0xee);
	_bus->sendData(0x03);
	_bus->sendData(0x48);
	_bus->sendData(0x03);
	_bus->sendData(0xa0);
	_bus->sendData(0x03);
	_bus->sendData(0xba);
	_bus->sendData(0x03);
	_bus->sendData(0xc5);
	_bus->sendData(0x03);
	_bus->sendData(0xd0);
	_bus->sendData(0x03);
	_bus->sendData(0xE0);
	_bus->sendData(0x03);
	_bus->sendData(0xea);
	_bus->sendData(0x03);
	_bus->sendData(0xFa);
	_bus->sendData(0x03);
	_bus->sendData(0xFF);

	_bus->sendCommand(0xD5);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);
	_bus->sendData(0x00);
	_bus->sendData(0x12);
	_bus->sendData(0x00);
	_bus->sendData(0x18);
	_bus->sendData(0x00);
	_bus->sendData(0x21);
	_bus->sendData(0x00);
	_bus->sendData(0x2a);
	_bus->sendData(0x00);
	_bus->sendData(0x35);
	_bus->sendData(0x00);
	_bus->sendData(0x47);
	_bus->sendData(0x00);
	_bus->sendData(0x56);
	_bus->sendData(0x00);
	_bus->sendData(0x90);
	_bus->sendData(0x00);
	_bus->sendData(0xe5);
	_bus->sendData(0x01);
	_bus->sendData(0x68);
	_bus->sendData(0x01);
	_bus->sendData(0xd5);
	_bus->sendData(0x01);
	_bus->sendData(0xd7);
	_bus->sendData(0x02);
	_bus->sendData(0x36);
	_bus->sendData(0x02);
	_bus->sendData(0xa6);
	_bus->sendData(0x02);
	_bus->sendData(0xee);
	_bus->sendData(0x03);
	_bus->sendData(0x48);
	_bus->sendData(0x03);
	_bus->sendData(0xa0);
	_bus->sendData(0x03);
	_bus->sendData(0xba);
	_bus->sendData(0x03);
	_bus->sendData(0xc5);
	_bus->sendData(0x03);
	_bus->sendData(0xd0);
	_bus->sendData(0x03);
	_bus->sendData(0xE0);
	_bus->sendData(0x03);
	_bus->sendData(0xea);
	_bus->sendData(0x03);
	_bus->sendData(0xFa);
	_bus->sendData(0x03);
	_bus->sendData(0xFF);

	_bus->sendCommand(0xD6);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x00);
	_bus->sendData(0x04);
	_bus->sendData(0x00);
	_bus->sendData(0x12);
	_bus->sendData(0x00);
	_bus->sendData(0x18);
	_bus->sendData(0x00);
	_bus->sendData(0x21);
	_bus->sendData(0x00);
	_bus->sendData(0x2a);
	_bus->sendData(0x00);
	_bus->sendData(0x35);
	_bus->sendData(0x00);
	_bus->sendData(0x47);
	_bus->sendData(0x00);
	_bus->sendData(0x56);
	_bus->sendData(0x00);
	_bus->sendData(0x90);
	_bus->sendData(0x00);
	_bus->sendData(0xe5);
	_bus->sendData(0x01);
	_bus->sendData(0x68);
	_bus->sendData(0x01);
	_bus->sendData(0xd5);
	_bus->sendData(0x01);
	_bus->sendData(0xd7);
	_bus->sendData(0x02);
	_bus->sendData(0x36);
	_bus->sendData(0x02);
	_bus->sendData(0xa6);
	_bus->sendData(0x02);
	_bus->sendData(0xee);
	_bus->sendData(0x03);
	_bus->sendData(0x48);
	_bus->sendData(0x03);
	_bus->sendData(0xa0);
	_bus->sendData(0x03);
	_bus->sendData(0xba);
	_bus->sendData(0x03);
	_bus->sendData(0xc5);
	_bus->sendData(0x03);
	_bus->sendData(0xd0);
	_bus->sendData(0x03);
	_bus->sendData(0xE0);
	_bus->sendData(0x03);
	_bus->sendData(0xea);
	_bus->sendData(0x03);
	_bus->sendData(0xFa);
	_bus->sendData(0x03);
	_bus->sendData(0xFF);

	_bus->sendCommand(0x3a);
	_bus->sendData(0x66);

	_bus->sendCommand(0x11);
	delay(200);
	_bus->sendCommand(0x29);

    _framebuffer = _bus->getFrameBuffer(_width, _height);
}

void Arduino_GC9503V_RGBPanel::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
    uint16_t *fb = _framebuffer;
    fb += (int32_t)y * _width;
    fb += x;
    *fb = color;
    Cache_WriteBack_Addr((uint32_t)fb, 2);
}

void Arduino_GC9503V_RGBPanel::writeFastVLine(int16_t x, int16_t y,
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

void Arduino_GC9503V_RGBPanel::writeFastHLine(int16_t x, int16_t y,
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

void Arduino_GC9503V_RGBPanel::writeFillRectPreclipped(int16_t x, int16_t y,
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

void Arduino_GC9503V_RGBPanel::draw16bitRGBBitmap(int16_t x, int16_t y,
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

void Arduino_GC9503V_RGBPanel::draw16bitBeRGBBitmap(int16_t x, int16_t y,
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

uint16_t *Arduino_GC9503V_RGBPanel::getFramebuffer()
{
    return _framebuffer;
}

#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
