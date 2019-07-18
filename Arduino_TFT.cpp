/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT.h"

inline uint16_t swapcolor(uint16_t x)
{
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}

Arduino_TFT::Arduino_TFT(Arduino_DataBus *bus, int8_t rst, uint8_t r, int16_t w, int16_t h, uint8_t col_offset, uint8_t row_offset)
    : Arduino_GFX(w, h)
{
  _bus = bus;
  _rst = rst;
  rotation = r;
  WIDTH = w;
  HEIGHT = h;
  COL_OFFSET = col_offset;
  ROW_OFFSET = row_offset;
}

void Arduino_TFT::begin(uint32_t speed)
{
  _bus->begin(speed);

  if (_rst >= 0)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(50);
    digitalWrite(_rst, LOW);
    delay(50);
    digitalWrite(_rst, HIGH);
    delay(50);
  }

  tftInit();
  setRotation(rotation);
  setAddrWindow(_xStart, _yStart, _width, _height);
}

inline void Arduino_TFT::startWrite()
{
  _bus->beginWrite();
}

void Arduino_TFT::writeColor(uint16_t color)
{
  _bus->write16(color);
}

inline void Arduino_TFT::writePixel(int16_t x, int16_t y, uint16_t color)
{
  if (_ordered_in_range(x, 0, _max_x) && _ordered_in_range(y, 0, _max_y))
  {
    writePixelPreclipped(x, y, color);
  }
}

void Arduino_TFT::writePixelPreclipped(int16_t x, int16_t y, uint16_t color)
{
  writeAddrWindow(x, y, 1, 1);
  _bus->write16(color);
}

void Arduino_TFT::writePixels(uint16_t color, uint32_t len)
{
  _bus->writePixels(color, len);
}

/*!
    @brief  Draw a vertical line on the display. Performs edge clipping and
            rejection. Not self-contained; should follow startWrite().
            Typically used by higher-level graphics primitives; user code
            shouldn't need to call this and is likely to use the self-
            contained drawFastVLine() instead.
    @param  x      Horizontal position of first point.
    @param  y      Vertical position of first point.
    @param  h      Line height in pixels (positive = below first point,
                   negative = above first point).
    @param  color  16-bit line color in '565' RGB format.
*/
void Arduino_TFT::writeFastVLine(int16_t x, int16_t y, int16_t h,
                                 uint16_t color)
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
          h = _max_y - y - 1;
        } // Clip bottom
        writeFillRectPreclipped(x, y, 1, h, color);
      }
    }
  }
}

/*!
    @brief  Draw a horizontal line on the display. Performs edge clipping
            and rejection. Not self-contained; should follow startWrite().
            Typically used by higher-level graphics primitives; user code
            shouldn't need to call this and is likely to use the self-
            contained drawFastHLine() instead.
    @param  x      Horizontal position of first point.
    @param  y      Vertical position of first point.
    @param  w      Line width in pixels (positive = right of first point,
                   negative = point of first corner).
    @param  color  16-bit line color in '565' RGB format.
*/
void Arduino_TFT::writeFastHLine(int16_t x, int16_t y, int16_t w,
                                 uint16_t color)
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
        writeFillRectPreclipped(x, y, w, 1, color);
      }
    }
  }
}

/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Arduino_TFT::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                            uint16_t color)
{
#if defined(ESP8266)
  yield();
#endif
  bool steep = _diff(y1, y0) > _diff(x1, x0);
  if (steep)
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1)
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = _diff(y1, y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

#ifdef ESP32
#else
  uint8_t hi = color >> 8;
  uint8_t lo = color;
#endif
  if (steep)
  {
    writeAddrWindow(y0, x0, 1, x1 - x0 + 1);
  }
  else
  {
    writeAddrWindow(x0, y0, x1 - x0 + 1, 1);
  }
  for (; x0 <= x1; x0++)
  {
#ifdef ESP32
    _bus->write16(color);
#else
    _bus->write(hi);
    _bus->write(lo);
#endif
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
      if (steep)
      {
        writeAddrWindow(y0, x0 + 1, 1, x1 - x0 + 1);
      }
      else
      {
        writeAddrWindow(x0 + 1, y0, x1 - x0 + 1, 1);
      }
    }
  }
}

/*!
    @brief  Draw a filled rectangle to the display. Not self-contained;
            should follow startWrite(). Typically used by higher-level
            graphics primitives; user code shouldn't need to call this and
            is likely to use the self-contained fillRect() instead.
            writeFillRect() performs its own edge clipping and rejection;
            see writeFillRectPreclipped() for a more 'raw' implementation.
    @param  x      Horizontal position of first corner.
    @param  y      Vertical position of first corner.
    @param  w      Rectangle width in pixels (positive = right of first
                   corner, negative = left of first corner).
    @param  h      Rectangle height in pixels (positive = below first
                   corner, negative = above first corner).
    @param  color  16-bit fill color in '565' RGB format.
    @note   Written in this deep-nested way because C by definition will
            optimize for the 'if' case, not the 'else' -- avoids branches
            and rejects clipped rectangles at the least-work possibility.
*/
void Arduino_TFT::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                uint16_t color)
{
  if (w && h)
  { // Nonzero width and height?
    if (w < 0)
    {             // If negative width...
      x += w + 1; //   Move X to left edge
      w = -w;     //   Use positive width
    }
    if (x <= _max_x)
    { // Not off right
      if (h < 0)
      {             // If negative height...
        y += h + 1; //   Move Y to top edge
        h = -h;     //   Use positive height
      }
      if (y <= _max_y)
      { // Not off bottom
        int16_t x2 = x + w - 1;
        if (x2 >= 0)
        { // Not off left
          int16_t y2 = y + h - 1;
          if (y2 >= 0)
          { // Not off top
            // Rectangle partly or fully overlaps screen
            if (x < 0)
            {
              x = 0;
              w = x2 + 1;
            } // Clip left
            if (y < 0)
            {
              y = 0;
              h = y2 + 1;
            } // Clip top
            if (x2 > _max_x)
            {
              w = _max_x - x + 1;
            } // Clip right
            if (y2 > _max_y)
            {
              h = _max_y - y + 1;
            } // Clip bottom
            writeFillRectPreclipped(x, y, w, h, color);
          }
        }
      }
    }
  }
}

/*!
    @brief  A lower-level version of writeFillRect(). This version requires
            all inputs are in-bounds, that width and height are positive,
            and no part extends offscreen. NO EDGE CLIPPING OR REJECTION IS
            PERFORMED. If higher-level graphics primitives are written to
            handle their own clipping earlier in the drawing process, this
            can avoid unnecessary function calls and repeated clipping
            operations in the lower-level functions.
    @param  x      Horizontal position of first corner. MUST BE WITHIN
                   SCREEN BOUNDS.
    @param  y      Vertical position of first corner. MUST BE WITHIN SCREEN
                   BOUNDS.
    @param  w      Rectangle width in pixels. MUST BE POSITIVE AND NOT
                   EXTEND OFF SCREEN.
    @param  h      Rectangle height in pixels. MUST BE POSITIVE AND NOT
                   EXTEND OFF SCREEN.
    @param  color  16-bit fill color in '565' RGB format.
    @note   This is a new function, no graphics primitives besides rects
            and horizontal/vertical lines are written to best use this yet.
*/
void Arduino_TFT::writeFillRectPreclipped(int16_t x, int16_t y,
                                          int16_t w, int16_t h, uint16_t color)
{
  writeAddrWindow(x, y, w, h);
  writePixels(color, (uint32_t)w * h);
}

void Arduino_TFT::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (w == 1)
  {
    if (x != _currentX)
    {
      writeAddrColumn(x, w);
      _currentX = x;
    }
  }
  else
  {
    writeAddrColumn(x, w);
    _currentX = 0xFFFF;
  }
  if (h == 1)
  {
    if (y != _currentY)
    {
      writeAddrRow(y, h);
      _currentY = y;
    }
  }
  else
  {
    writeAddrRow(y, h);
    _currentY = 0xFFFF;
  }
  writeAddrMemWrite();
}

inline void Arduino_TFT::endWrite()
{
  _bus->endWrite();
}

/**************************************************************************/
/*!
   @brief    Push a pixel, overwrite in subclasses if startWrite is defined!
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Arduino_TFT::pushColor(uint16_t color)
{
  _bus->beginWrite();
  _bus->write16(color);
  _bus->endWrite();
}

/**************************************************************************/
/*!
   @brief    Write a pixel, overwrite in subclasses if startWrite is defined!
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Arduino_TFT::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if (_ordered_in_range(x, 0, _max_x) && _ordered_in_range(y, 0, _max_y))
  {
    _bus->beginWrite();
    _bus->write16(color);
    _bus->endWrite();
  }
}

void Arduino_TFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w,
                                uint16_t h)
{
  startWrite();

  writeAddrWindow(x0, y0, w, h);

  endWrite();
}
