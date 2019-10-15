/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_Canvas.h"

Arduino_Canvas::Arduino_Canvas(int16_t w, int16_t h, Arduino_GFX *output)
    : Arduino_GFX(w, h), _output(output)
{
}

void Arduino_Canvas::begin(uint32_t speed)
{
#if defined(ESP32)
    if (psramFound())
    {
        _framebuffer = (uint16_t *)ps_malloc(_width * _height * 2);
    }
    else
    {
        _framebuffer = (uint16_t *)malloc(_width * _height * 2);
    }
#else
    _framebuffer = (uint16_t *)malloc(_width * _height * 2);
#endif
    _output->begin(speed);
    _output->fillScreen(BLACK);
}

void Arduino_Canvas::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
    _framebuffer[(y * _width) + x] = color;
}

void Arduino_Canvas::writeFastVLine(int16_t x, int16_t y,
                                    int16_t h, uint16_t color)
{
    uint16_t *fb = _framebuffer + (y * _width) + x;
    while (h--)
    {
        *fb = color;
        fb += _width;
    }
}

void Arduino_Canvas::writeFastHLine(int16_t x, int16_t y,
                                    int16_t w, uint16_t color)
{
    uint16_t *fb = _framebuffer + (y * _width) + x;
    while (w--)
    {
        *(fb++) = color;
    }
}

void Arduino_Canvas::flush()
{
    _output->draw16bitRGBBitmap(0, 0, _framebuffer, _width, _height);
}
