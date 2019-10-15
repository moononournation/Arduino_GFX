/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_Canvas_Indexed.h"

Arduino_Canvas_Indexed::Arduino_Canvas_Indexed(int16_t w, int16_t h, Arduino_TFT *output)
    : Arduino_GFX(w, h), _output(output)
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
        _output->writeColor(color_index[*(fb++)]);
    }
    _output->endWrite();
}

uint8_t Arduino_Canvas_Indexed::get_color_index(uint16_t color)
{
    for (uint8_t i = 0; i < indexed_size; i++)
    {
        if (color_index[i] == color)
        {
            return i;
        }
    }
    if (indexed_size < COLOR_IDX_SIZE)
    {
        color_index[indexed_size] = color;
        // Serial.print("color_index[");
        // Serial.print(indexed_size);
        // Serial.print("] = ");
        // Serial.println(color);
        return indexed_size++;
    }
    else // overflowed
    {
        color_index[COLOR_IDX_SIZE] = color;
        return COLOR_IDX_SIZE;
    }
}

uint16_t Arduino_Canvas_Indexed::get_index_color(uint8_t idx)
{
    return color_index[idx];
}
