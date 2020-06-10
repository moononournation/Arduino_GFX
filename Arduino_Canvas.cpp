/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_Canvas.h"

Arduino_Canvas::Arduino_Canvas(int16_t w, int16_t h, Arduino_G *output, int16_t output_x, int16_t output_y)
    : Arduino_GFX(w, h), _output(output), _output_x(output_x), _output_y(output_y)
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
        // _framebuffer = (uint16_t *)malloc(_width * _height * 2);
        // hack for allocate memory over 63,360 pixels
        uint16_t *tmp;
        _framebuffer = (uint16_t *)malloc(_width * _height);
        tmp = (uint16_t *)malloc(_width * _height);
        log_v("_framebuffer delta: %d", tmp - _framebuffer);
    }
#else
    _framebuffer = (uint16_t *)malloc(_width * _height * 2);
#endif
    if (!_framebuffer)
    {
        Serial.println(F("_framebuffer allocation failed."));
    }
    _output->begin(speed);
    _output->fillScreen(BLACK);
}

void Arduino_Canvas::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
    _framebuffer[((int32_t)y * _width) + x] = color;
}

void Arduino_Canvas::writeFastVLine(int16_t x, int16_t y,
                                    int16_t h, uint16_t color)
{
    uint16_t *fb = _framebuffer + ((int32_t)y * _width) + x;
    while (h--)
    {
        *fb = color;
        fb += _width;
    }
}

void Arduino_Canvas::writeFastHLine(int16_t x, int16_t y,
                                    int16_t w, uint16_t color)
{
    uint16_t *fb = _framebuffer + ((int32_t)y * _width) + x;
    while (w--)
    {
        *(fb++) = color;
    }
}

void Arduino_Canvas::flush()
{
    _output->draw16bitRGBBitmap(_output_x, _output_y, _framebuffer, _width, _height);
}
