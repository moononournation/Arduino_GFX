/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT.h"
#include "glcdfont.c"

Arduino_TFT::Arduino_TFT(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_GFX(w, h)
{
  _bus = bus;
  _rst = rst;
  _rotation = r;
  _ips = ips;
  WIDTH = w;
  HEIGHT = h;
  COL_OFFSET1 = col_offset1;
  ROW_OFFSET1 = row_offset1;
  COL_OFFSET2 = col_offset2;
  ROW_OFFSET2 = row_offset2;
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
  setRotation(_rotation); // apply the setting rotation to the display
  setAddrWindow(0, 0, _width, _height);
}

inline void Arduino_TFT::startWrite()
{
  _bus->beginWrite();
}

inline void Arduino_TFT::writeColor(uint16_t color)
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
  writeColor(color);
}

void Arduino_TFT::writeRepeat(uint16_t color, uint32_t len)
{
  _bus->writeRepeat(color, len);
}

void Arduino_TFT::writePixels(uint16_t *data, uint32_t size)
{
  _bus->writePixels(data, size);
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
  int16_t len = 0;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0 <= x1; x0++)
  {
    err -= dy;
    if (err < 0)
    {
      len++;
      if (steep)
      {
        writeFillRectPreclipped(y0, x0, 1, len, color);
      }
      else
      {
        writeFillRectPreclipped(x0, y0, len, 1, color);
      }
      y0 += ystep;
      err += dx;
      len = 0;
    }
  }
  if (len)
  {
    if (steep)
    {
      writeFillRectPreclipped(y0, x0, 1, len, color);
    }
    else
    {
      writeFillRectPreclipped(x0, y0, len, 1, color);
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
#ifdef ESP8266
  yield();
#endif
  writeAddrWindow(x, y, w, h);
  writeRepeat(color, (uint32_t)w * h);
}

void Arduino_TFT::writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if ((w == 1) && (h == 1))
  {
    if (x != _currentX)
    {
      writeAddrColumn(x, w);
      _currentX = x;
    }
    if (y != _currentY)
    {
      writeAddrRow(y, h);
      _currentY = y;
    }
  }
  else
  {
    writeAddrColumn(x, w);
    _currentX = 0xFFFF;
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
  writeColor(color);
  _bus->endWrite();
}

// TFT tuned BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

/**************************************************************************/
/*!
   @brief      Draw a PROGMEM-resident 1-bit image at the specified (x,y) position, using the specified foreground (for set bits) and background (unset bits) colors.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
    @param    bg 16-bit 5-6-5 Color to draw background with
*/
/**************************************************************************/
void Arduino_TFT::drawBitmap(int16_t x, int16_t y,
                             const uint8_t bitmap[], int16_t w, int16_t h,
                             uint16_t color, uint16_t bg)
{

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
      {
        byte <<= 1;
      }
      else
      {
        byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
      }
      writeColor((byte & 0x80) ? color : bg);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief      Draw a RAM-resident 1-bit image at the specified (x,y) position, using the specified foreground (for set bits) and background (unset bits) colors.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
    @param    bg 16-bit 5-6-5 Color to draw background with
*/
/**************************************************************************/
void Arduino_TFT::drawBitmap(int16_t x, int16_t y,
                             uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{

  int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  uint8_t byte = 0;

  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if (i & 7)
      {
        byte <<= 1;
      }
      else
      {
        byte = bitmap[j * byteWidth + i / 8];
      }
      writeColor((byte & 0x80) ? color : bg);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a PROGMEM-resident 8-bit image (grayscale) at the specified (x,y) pos.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Arduino_TFT::drawGrayscaleBitmap(int16_t x, int16_t y,
                                      const uint8_t bitmap[], int16_t w, int16_t h)
{
  uint8_t v;
  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      v = (uint8_t)pgm_read_byte(&bitmap[j * w + i]);
      writeColor(color565(v, v, v));
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y) pos.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with grayscale bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Arduino_TFT::drawGrayscaleBitmap(int16_t x, int16_t y,
                                      uint8_t *bitmap, int16_t w, int16_t h)
{
  uint8_t v;
  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      v = bitmap[j * w + i];
      writeColor(color565(v, v, v));
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) at the specified (x,y) position.
   For 16-bit display devices; no color reduction performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Arduino_TFT::draw16bitRGBBitmap(int16_t x, int16_t y,
                                     const uint16_t bitmap[], int16_t w, int16_t h)
{
  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      writeColor(pgm_read_word(&bitmap[j * w + i]));
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 16-bit image (RGB 5/6/5) at the specified (x,y) position.
   For 16-bit display devices; no color reduction performed.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Arduino_TFT::draw16bitRGBBitmap(int16_t x, int16_t y,
                                     uint16_t *bitmap, int16_t w, int16_t h)
{
  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      writeColor(bitmap[j * w + i]);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a PROGMEM-resident 24-bit image (RGB 5/6/5) at the specified (x,y) position.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Arduino_TFT::draw24bitRGBBitmap(int16_t x, int16_t y,
                                     const uint8_t bitmap[], int16_t w, int16_t h)
{
  int16_t offset = 0;
  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      writeColor(color565(pgm_read_byte(&bitmap[offset++]), pgm_read_byte(&bitmap[offset++]), pgm_read_byte(&bitmap[offset++])));
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
   @brief   Draw a RAM-resident 24-bit image (RGB 5/6/5) at the specified (x,y) position.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Arduino_TFT::draw24bitRGBBitmap(int16_t x, int16_t y,
                                     uint8_t *bitmap, int16_t w, int16_t h)
{
  int16_t offset = 0;
  startWrite();
  writeAddrWindow(x, y, w, h);
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      writeColor(color565(bitmap[offset++], bitmap[offset++], bitmap[offset++]));
    }
  }
  endWrite();
}

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void Arduino_TFT::drawChar(int16_t x, int16_t y, unsigned char c,
                           uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
{
  uint16_t block_w;
  uint16_t block_h;

  if (!gfxFont) // 'Classic' built-in font
  {
    block_w = 6 * size_x;
    block_h = 8 * size_y;
    if (
        (x > _max_x) ||            // Clip right
        (y > _max_y) ||            // Clip bottom
        ((x + block_w - 1) < 0) || // Clip left
        ((y + block_h - 1) < 0)    // Clip top
    )
    {
      return;
    }

    if (!_cp437 && (c >= 176))
    {
      c++; // Handle 'classic' charset behavior
    }

    uint8_t col[5];
    for (int8_t i = 0; i < 5; i++)
    {
      col[i] = pgm_read_byte(&font[c * 5 + i]);
    }

    startWrite();
    if (bg != color)
    {
      writeAddrWindow(x, y, block_w, block_h);

      uint16_t line_buf[block_w];
      if (size_x == 1)
      {
        line_buf[5] = bg; // last column always bg
      }
      else
      {
        for (int8_t k = 0; k < size_x; k++)
        {
          line_buf[5 * size_x + k] = bg;
        }
      }
      uint8_t bit = 1;

      while (bit)
      {
        for (int8_t i = 0; i < 5; i++)
        {
          if (size_x == 1)
          {
            line_buf[i] = (col[i] & bit) ? color : bg;
          }
          else
          {
            uint16_t dot_color = (col[i] & bit) ? color : bg;
            for (int8_t k = 0; k < size_x; k++)
            {
              line_buf[i * size_x + k] = dot_color;
            }
          }
        }
        if (size_y == 1)
        {
          _bus->writePixels(line_buf, block_w);
        }
        else
        {
          for (int8_t l = 0; l < size_y; l++)
          {
            _bus->writePixels(line_buf, block_w);
          }
        }
        bit <<= 1;
      }
    }
    else // (bg == color), no background color
    {
      for (int8_t i = 0; i < 5; i++)
      { // Char bitmap = 5 columns
        uint8_t line = col[i];
        for (int8_t j = 0; j < 8; j++, line >>= 1)
        {
          if (line & 1)
          {
            if (size_x == 1 && size_y == 1)
            {
              writePixel(x + i, y + j, color);
            }
            else
            {
              writeFillRectPreclipped(x + i * size_x, y + j * size_y, size_x, size_y, color);
            }
          }
        }
      }
    }
    endWrite();
  }
  else // Custom font
  {
    // Character is assumed previously filtered by write() to eliminate
    // newlines, returns, non-printable characters, etc.  Calling
    // drawChar() directly with 'bad' characters of font may cause mayhem!

    c -= (uint8_t)pgm_read_byte(&gfxFont->first);
    GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
    uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);

    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
    uint8_t w = pgm_read_byte(&glyph->width),
            h = pgm_read_byte(&glyph->height);
    int8_t xo = pgm_read_byte(&glyph->xOffset),
           yo = pgm_read_byte(&glyph->yOffset);
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    if (size_x > 1 || size_y > 1)
    {
      xo16 = xo;
      yo16 = yo;
    }

    block_w = w * size_x;
    block_h = h * size_y;
    if (
        (x > _max_x) ||            // Clip right
        (y > _max_y) ||            // Clip bottom
        ((x + block_w - 1) < 0) || // Clip left
        ((y + block_h - 1) < 0)    // Clip top
    )
    {
      return;
    }

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    startWrite();
    for (yy = 0; yy < h; yy++)
    {
      for (xx = 0; xx < w; xx++)
      {
        if (!(bit++ & 7))
        {
          bits = pgm_read_byte(&bitmap[bo++]);
        }
        if (bits & 0x80)
        {
          if (size_x == 1 && size_y == 1)
          {
            writePixel(x + xo + xx, y + yo + yy, color);
          }
          else
          {
            writeFillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
                          size_x, size_y, color);
          }
        }
        bits <<= 1;
      }
    }
    endWrite();

  } // End classic vs custom font
}

void Arduino_TFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w,
                                uint16_t h)
{
  startWrite();

  writeAddrWindow(x0, y0, w, h);

  endWrite();
}

/**************************************************************************/
/*!
    @brief      Set rotation setting for display
    @param  x   0 thru 3 corresponding to 4 cardinal rotations
*/
/**************************************************************************/
void Arduino_TFT::setRotation(uint8_t r)
{
  Arduino_GFX::setRotation(r);
  switch (_rotation)
  {
  case 0:
    _xStart = COL_OFFSET1;
    _yStart = ROW_OFFSET1;
    break;

  case 1:
    _xStart = ROW_OFFSET1;
    _yStart = COL_OFFSET1;
    break;

  case 2:
    _xStart = COL_OFFSET2;
    _yStart = ROW_OFFSET2;
    break;

  case 3:
    _xStart = ROW_OFFSET2;
    _yStart = COL_OFFSET2;
    break;
  }
}
