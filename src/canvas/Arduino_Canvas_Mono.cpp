#include "../Arduino_DataBus.h"
#if !defined(LITTLE_FOOT_PRINT)

#include "../Arduino_GFX.h"
#include "Arduino_Canvas_Mono.h"

Arduino_Canvas_Mono::Arduino_Canvas_Mono(int16_t w, int16_t h, Arduino_G *output, int16_t output_x, int16_t output_y, bool verticalByte)
    : Arduino_GFX(w, h), _output(output), _output_x(output_x), _output_y(output_y), _verticalByte(verticalByte),
      _canvas_width(w), _canvas_height(h)
{
}

Arduino_Canvas_Mono::~Arduino_Canvas_Mono()
{
  if (_framebuffer)
  {
    free(_framebuffer);
  }
  if (_dither_error_buffer)
  {
    free(_dither_error_buffer);
  }
}

bool Arduino_Canvas_Mono::begin(int32_t speed)
{
  if (speed != GFX_SKIP_OUTPUT_BEGIN)
  {
    if (_output)
    {
      if (!_output->begin(speed))
      {
        return false;
      }
    }
  }

  if (!_framebuffer)
  {
    size_t s;

    // allocate memory by full bytes.
    if (_verticalByte)
    {
      s = _canvas_width * (_canvas_height + 7) / 8;
    }
    else
    {
      s = (_canvas_width + 7) / 8 * _canvas_height;
    }

#if defined(ESP32)
    if (psramFound())
    {
      _framebuffer = (uint8_t *)ps_malloc(s);
    }
    else
    {
      _framebuffer = (uint8_t *)malloc(s);
    }
#else
    _framebuffer = (uint8_t *)malloc(s);
#endif
    if (!_framebuffer)
    {
      return false;
    }
  }

  if (!_dither_error_buffer)
  {
    const size_t error_buffer_size = (_canvas_width + 2) * sizeof(int16_t) * 2;
#if defined(ESP32)
    if (psramFound())
    {
      _dither_error_buffer = (int16_t *)ps_malloc(error_buffer_size);
    }
    else
    {
      _dither_error_buffer = (int16_t *)malloc(error_buffer_size);
    }
#else
    _dither_error_buffer = (int16_t *)malloc(error_buffer_size);
#endif
    if (!_dither_error_buffer)
    {
      return false;
    }
    _dither_error_current = _dither_error_buffer;
    _dither_error_next = _dither_error_buffer + _canvas_width + 2;
  }

  return true;
}

void Arduino_Canvas_Mono::startWrite()
{
  if (_dither_error_buffer)
  {
    memset(_dither_error_buffer, 0, (_canvas_width + 2) * sizeof(int16_t) * 2);
    _dither_error_current = _dither_error_buffer;
    _dither_error_next = _dither_error_buffer + _canvas_width + 2;
  }
  _dither_started = false;
  Arduino_GFX::startWrite();
}

void Arduino_Canvas_Mono::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
  if (_framebuffer)
  {
    // change the pixel in the original orientation of the bitmap buffer
    if (_verticalByte)
    {
      // vertical buffer layout: 1 byte in the buffer contains 8 vertical pixels
      int32_t pos = x + (y / 8) * _canvas_width;

      if (color & 0b1000010000010000)
      {
        _framebuffer[pos] |= (1 << (y & 7));
      }
      else
      {
        _framebuffer[pos] &= ~(1 << (y & 7));
      }
    }
    else
    {
      // horizontal buffer layout: 1 byte in the buffer contains 8 horizontal pixels
      int16_t w = (_canvas_width + 7) / 8;
      int32_t pos = (y * w) + (x / 8);

      if (color & 0b1000010000010000)
      {
        _framebuffer[pos] |= 0x80 >> (x & 7);
      }
      else
      {
        _framebuffer[pos] &= ~(0x80 >> (x & 7));
      }
    }
  }
}

void Arduino_Canvas_Mono::writeDitheredPixel(int16_t x, int16_t y, uint16_t color)
{
  if (!_dither_error_current || (x < 0) || (x >= _canvas_width))
  {
    return;
  }

  if (!_dither_started)
  {
    _dither_y = y;
    _dither_started = true;
  }
  else if (_dither_y != y)
  {
    if (_dither_y + 1 == y)
    {
      int16_t *previous = _dither_error_current;
      _dither_error_current = _dither_error_next;
      _dither_error_next = previous;
      memset(_dither_error_next, 0, (_canvas_width + 2) * sizeof(int16_t));
    }
    else
    {
      memset(_dither_error_buffer, 0, (_canvas_width + 2) * sizeof(int16_t) * 2);
      _dither_error_current = _dither_error_buffer;
      _dither_error_next = _dither_error_buffer + _canvas_width + 2;
    }
    _dither_y = y;
  }

  const uint8_t red = ((color >> 11) & 0x1F) * 255 / 31;
  const uint8_t green = ((color >> 5) & 0x3F) * 255 / 63;
  const uint8_t blue = (color & 0x1F) * 255 / 31;
  const uint16_t luminance = (299 * red + 587 * green + 114 * blue) / 1000;
  const int16_t error_index = x + 1;
  int16_t adjusted_luminance = luminance + _dither_error_current[error_index];
  if (adjusted_luminance < 0)
  {
    adjusted_luminance = 0;
  }
  else if (adjusted_luminance > 255)
  {
    adjusted_luminance = 255;
  }

  const bool white = adjusted_luminance >= 128;
  const int16_t error = adjusted_luminance - (white ? 255 : 0);
  _dither_error_current[error_index + 1] += error * 7 / 16;
  _dither_error_next[error_index - 1] += error * 3 / 16;
  _dither_error_next[error_index] += error * 5 / 16;
  _dither_error_next[error_index + 1] += error / 16;
  writePixel(x, y, white ? RGB565_WHITE : RGB565_BLACK);
}

void Arduino_Canvas_Mono::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
  const int16_t byte_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (pgm_read_byte(&bitmap[j * byte_width + i / 8]) & (0x80 >> (i & 7)))
      {
        writeDitheredPixel(x + i, y + j, color);
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color)
{
  const int16_t byte_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (bitmap[j * byte_width + i / 8] & (0x80 >> (i & 7)))
      {
        writeDitheredPixel(x + i, y + j, color);
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  const int16_t byte_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const bool set = pgm_read_byte(&bitmap[j * byte_width + i / 8]) & (0x80 >> (i & 7));
      writeDitheredPixel(x + i, y + j, set ? color : bg);
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{
  const int16_t byte_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const bool set = bitmap[j * byte_width + i / 8] & (0x80 >> (i & 7));
      writeDitheredPixel(x + i, y + j, set ? color : bg);
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
  const int16_t byte_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (pgm_read_byte(&bitmap[j * byte_width + i / 8]) & (1 << (i & 7)))
      {
        writeDitheredPixel(x + i, y + j, color);
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const uint8_t value = pgm_read_byte(&bitmap[j * w + i]);
      writeDitheredPixel(x + i, y + j, color565(value, value, value));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const uint8_t value = bitmap[j * w + i];
      writeDitheredPixel(x + i, y + j, color565(value, value, value));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawGrayscaleBitmapWithMask(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
  const int16_t mask_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (pgm_read_byte(&mask[j * mask_width + i / 8]) & (0x80 >> (i & 7)))
      {
        const uint8_t value = pgm_read_byte(&bitmap[j * w + i]);
        writeDitheredPixel(x + i, y + j, color565(value, value, value));
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawGrayscaleBitmapWithMask(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  const int16_t mask_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (mask[j * mask_width + i / 8] & (0x80 >> (i & 7)))
      {
        const uint8_t value = bitmap[j * w + i];
        writeDitheredPixel(x + i, y + j, color565(value, value, value));
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h, int16_t x_skip)
{
  int32_t offset = 0;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      writeDitheredPixel(x + i, y + j, color_index[bitmap[offset++]]);
    }
    offset += x_skip;
  }
  endWrite();
}

void Arduino_Canvas_Mono::drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, uint8_t chroma_key, int16_t w, int16_t h, int16_t x_skip)
{
  int32_t offset = 0;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const uint8_t index = bitmap[offset++];
      if (index != chroma_key)
      {
        writeDitheredPixel(x + i, y + j, color_index[index]);
      }
    }
    offset += x_skip;
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  int32_t offset = 0;
  int32_t index = 0;
  uint8_t packed = 0;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      uint8_t color;
      if (offset & 1)
      {
        color = packed & 0x07;
      }
      else
      {
        packed = bitmap[index++];
        color = (packed >> 3) & 0x07;
      }
      const uint16_t rgb565 = ((color & 0x04) ? RGB565_RED : 0) |
                              ((color & 0x02) ? RGB565_LIME : 0) |
                              ((color & 0x01) ? RGB565_BLUE : 0);
      writeDitheredPixel(x + i, y + j, rgb565);
      ++offset;
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      writeDitheredPixel(x + i, y + j, pgm_read_word(&bitmap[(int32_t)j * w + i]));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      writeDitheredPixel(x + i, y + j, bitmap[(int32_t)j * w + i]);
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitRGBBitmapWithMask(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
  const int16_t mask_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (pgm_read_byte(&mask[j * mask_width + i / 8]) & (0x80 >> (i & 7)))
      {
        writeDitheredPixel(x + i, y + j, pgm_read_word(&bitmap[(int32_t)j * w + i]));
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitRGBBitmapWithMask(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  const int16_t mask_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (mask[j * mask_width + i / 8] & (0x80 >> (i & 7)))
      {
        writeDitheredPixel(x + i, y + j, bitmap[(int32_t)j * w + i]);
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitRGBBitmapWithTranColor(int16_t x, int16_t y, uint16_t *bitmap, uint16_t transparent_color, int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const uint16_t color = bitmap[(int32_t)j * w + i];
      if (color != transparent_color)
      {
        writeDitheredPixel(x + i, y + j, color);
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitBeRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const uint16_t color = bitmap[(int32_t)j * w + i];
      writeDitheredPixel(x + i, y + j, (color >> 8) | (color << 8));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw24bitRGBBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const int32_t offset = ((int32_t)j * w + i) * 3;
      writeDitheredPixel(x + i, y + j, color565(
          pgm_read_byte(&bitmap[offset]), pgm_read_byte(&bitmap[offset + 1]), pgm_read_byte(&bitmap[offset + 2])));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      const int32_t offset = ((int32_t)j * w + i) * 3;
      writeDitheredPixel(x + i, y + j, color565(bitmap[offset], bitmap[offset + 1], bitmap[offset + 2]));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw24bitRGBBitmapWithMask(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
  const int16_t mask_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (pgm_read_byte(&mask[j * mask_width + i / 8]) & (0x80 >> (i & 7)))
      {
        const int32_t offset = ((int32_t)j * w + i) * 3;
        writeDitheredPixel(x + i, y + j, color565(
            pgm_read_byte(&bitmap[offset]), pgm_read_byte(&bitmap[offset + 1]), pgm_read_byte(&bitmap[offset + 2])));
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw24bitRGBBitmapWithMask(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  const int16_t mask_width = (w + 7) / 8;
  startWrite();
  for (int16_t j = 0; j < h; ++j)
  {
    for (int16_t i = 0; i < w; ++i)
    {
      if (mask[j * mask_width + i / 8] & (0x80 >> (i & 7)))
      {
        const int32_t offset = ((int32_t)j * w + i) * 3;
        writeDitheredPixel(x + i, y + j, color565(bitmap[offset], bitmap[offset + 1], bitmap[offset + 2]));
      }
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::draw16bitBeRGBBitmapR1(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  for (int16_t i = 0; i < h; ++i)
  {
    for (int16_t j = w - 1; j >= 0; --j)
    {
      const uint16_t color = bitmap[(int32_t)j * w + i];
      writeDitheredPixel(x + w - j - 1, y + i, (color >> 8) | (color << 8));
    }
  }
  endWrite();
}

void Arduino_Canvas_Mono::flush(bool force_flush)
{
  if (_framebuffer && _output)
  {
    _output->drawBitmap(_output_x, _output_y, _framebuffer, _canvas_width, _canvas_height, RGB565_WHITE, RGB565_BLACK);
  }
}

uint8_t *Arduino_Canvas_Mono::getFramebuffer()
{
  return _framebuffer;
}

#endif // !defined(LITTLE_FOOT_PRINT)
