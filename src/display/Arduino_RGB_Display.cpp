#include "../Arduino_DataBus.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)

#include "../Arduino_GFX.h"
#include "Arduino_RGB_Display.h"

Arduino_RGB_Display::Arduino_RGB_Display(
    int16_t w, int16_t h, Arduino_ESP32RGBPanel *rgbpanel, uint8_t r, bool auto_flush,
    Arduino_DataBus *bus, int8_t rst, const uint8_t *init_operations, size_t init_operations_len)
    : Arduino_GFX(w, h), _rgbpanel(rgbpanel), _auto_flush(auto_flush),
      _bus(bus), _rst(rst), _init_operations(init_operations), _init_operations_len(init_operations_len)
{
  _framebuffer_size = w * h * 2;
  _rotation = r;
}

bool Arduino_RGB_Display::begin(int32_t speed)
{
  if (_bus)
  {
    if (!_bus->begin())
    {
      return false;
    }
  }

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
  else
  {
    if (_bus)
    {
      // Software Rest
      _bus->sendCommand(0x01);
      delay(120);
    }
  }

  if (_bus)
  {
    if (_init_operations_len > 0)
    {
      _bus->batchOperation((uint8_t *)_init_operations, _init_operations_len);
    }
  }

  _rgbpanel->begin(speed);
  _framebuffer = _rgbpanel->getFrameBuffer(WIDTH, HEIGHT);

  if (!_framebuffer)
  {
    return false;
  }

  return true;
}

void Arduino_RGB_Display::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
  uint16_t *fb = _framebuffer;
  fb += (int32_t)y * _width;
  fb += x;
  *fb = color;
  if (_auto_flush)
  {
    Cache_WriteBack_Addr((uint32_t)fb, 2);
  }
}

void Arduino_RGB_Display::writeFastVLine(int16_t x, int16_t y,
                                         int16_t h, uint16_t color)
{
  if (_ordered_in_range(x, 0, _max_x) && h)
  { // X on screen, nonzero height
    if (h < 0)
    {             // If negative height...
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
        if (_auto_flush)
        {
          while (h--)
          {
            *fb = color;
            Cache_WriteBack_Addr((uint32_t)fb, 2);
            fb += _width;
          }
        }
        else
        {
          while (h--)
          {
            *fb = color;
            fb += _width;
          }
        }
      }
    }
  }
}

void Arduino_RGB_Display::writeFastHLine(int16_t x, int16_t y,
                                         int16_t w, uint16_t color)
{
  if (_ordered_in_range(y, 0, _max_y) && w)
  { // Y on screen, nonzero width
    if (w < 0)
    {             // If negative width...
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
        if (_auto_flush)
        {
          Cache_WriteBack_Addr(cachePos, writeSize);
        }
      }
    }
  }
}

void Arduino_RGB_Display::writeFillRectPreclipped(int16_t x, int16_t y,
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
  if (_auto_flush)
  {
    Cache_WriteBack_Addr(cachePos, _width * h * 2);
  }
}

void Arduino_RGB_Display::drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h)
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
        row[i] = color_index[*bitmap++];
      }
      bitmap += xskip;
      row += _width;
    }
    if (_auto_flush)
    {
      Cache_WriteBack_Addr(cachePos, _width * h * 2);
    }
  }
}

void Arduino_RGB_Display::draw16bitRGBBitmap(int16_t x, int16_t y,
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
    if (((_width & 1) == 0) && ((xskip & 1) == 0) && ((w & 1) == 0))
    {
      uint32_t *row2 = (uint32_t *)row;
      uint32_t *bitmap2 = (uint32_t *)bitmap;
      int16_t _width2 = _width >> 1;
      int16_t xskip2 = xskip >> 1;
      int16_t w2 = w >> 1;

      for (int16_t j = 0; j < h; j++)
      {
        for (int16_t i = 0; i < w2; i++)
        {
          row2[i] = *bitmap2++;
        }
        bitmap2 += xskip2;
        row2 += _width2;
      }
    }
    else
    {
      for (int j = 0; j < h; j++)
      {
        for (int i = 0; i < w; i++)
        {
          row[i] = *bitmap++;
        }
        bitmap += xskip;
        row += _width;
      }
    }
    if (_auto_flush)
    {
      Cache_WriteBack_Addr(cachePos, _width * h * 2);
    }
  }
}

void Arduino_RGB_Display::draw16bitBeRGBBitmap(int16_t x, int16_t y,
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
    if (_auto_flush)
    {
      Cache_WriteBack_Addr(cachePos, _width * h * 2);
    }
  }
}

void Arduino_RGB_Display::flush(void)
{
  if (!_auto_flush)
  {
    Cache_WriteBack_Addr((uint32_t)_framebuffer, _framebuffer_size);
  }
}

uint16_t *Arduino_RGB_Display::getFramebuffer()
{
  return _framebuffer;
}

#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32S3)
