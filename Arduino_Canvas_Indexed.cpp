/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_Canvas_Indexed.h"

Arduino_Canvas_Indexed::Arduino_Canvas_Indexed(int16_t w, int16_t h, Arduino_TFT *output, uint16_t color_mask)
    : Arduino_GFX(w, h), _output(output), _color_mask(color_mask)
{
}

void Arduino_Canvas_Indexed::begin(uint32_t speed)
{
    _framebuffer = (uint8_t *)malloc(_width * _height);
    _output->begin(speed);
    _output->fillScreen(BLACK);
}

void Arduino_Canvas_Indexed::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
    _framebuffer[(y * _width) + x] = get_color_index(color);
}

void Arduino_Canvas_Indexed::writeFastVLine(int16_t x, int16_t y,
                                            int16_t h, uint16_t color)
{
    uint8_t idx = get_color_index(color);

    uint8_t *fb = _framebuffer + (y * _width) + x;
    while (h--)
    {
        *fb = idx;
        fb += _width;
    }
}

void Arduino_Canvas_Indexed::writeFastHLine(int16_t x, int16_t y,
                                            int16_t w, uint16_t color)
{
    uint8_t idx = get_color_index(color);

    uint8_t *fb = _framebuffer + (y * _width) + x;
    while (w--)
    {
        *(fb++) = idx;
    }
}

void Arduino_Canvas_Indexed::flush()
{
    uint32_t len = _width * _height;
    uint8_t *fb = _framebuffer;
    _output->startWrite();
    while (len--)
    {
        _output->writeColor(_color_index[*(fb++)]);
    }
    _output->endWrite();
}

uint8_t Arduino_Canvas_Indexed::get_color_index(uint16_t color)
{
    color &= _color_mask;
    for (uint8_t i = 0; i < _indexed_size; i++)
    {
        if (_color_index[i] == color)
        {
            return i;
        }
    }
    if (_indexed_size < COLOR_IDX_SIZE)
    {
        _color_index[_indexed_size] = color;
        // Serial.print("color_index[");
        // Serial.print(_indexed_size);
        // Serial.print("] = ");
        // Serial.println(color);
        return _indexed_size++;
    }
    else // overflowed
    {
        return 0;
    }
}

uint16_t Arduino_Canvas_Indexed::get_index_color(uint8_t idx)
{
    return _color_index[idx];
}
