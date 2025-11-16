/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_GFX_H_
#define _ARDUINO_GFX_H_

#include "Arduino_G.h"
#include "Arduino_DataBus.h"
#include <Print.h>

#if !defined(ATTINY_CORE)
#include "gfxfont.h"
#endif // !defined(ATTINY_CORE)

#ifndef DEGTORAD
#define DEGTORAD 0.017453292519943295769236907684886F
#endif

#if __has_include(<U8g2lib.h>)
#include <U8g2lib.h>
#define U8G2_FONT_SUPPORT
#include "font/u8g2_font_chill7_h_cjk.h"
#include "font/u8g2_font_cubic11_h_cjk.h"
#include "font/u8g2_font_quan7_h_cjk.h"
#include "font/u8g2_font_unifont_h_utf8.h"
#include "font/u8g2_font_unifont_t_chinese.h"
#include "font/u8g2_font_unifont_t_chinese4.h"
#include "font/u8g2_font_unifont_t_cjk.h"
#endif

#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#define RGB16TO24(c) ((((uint32_t)c & 0xF800) << 8) | ((c & 0x07E0) << 5) | ((c & 0x1F) << 3))

/* https://www.w3.org/wiki/CSS/Properties/color/keywords */
#define RGB565_ALICEBLUE RGB565(240, 248, 255)
#define RGB565_ANTIQUEWHITE RGB565(250, 235, 215)
#define RGB565_AQUA RGB565(0, 255, 255)
#define RGB565_AQUAMARINE RGB565(127, 255, 212)
#define RGB565_AZURE RGB565(240, 255, 255)
#define RGB565_BEIGE RGB565(245, 245, 220)
#define RGB565_BISQUE RGB565(255, 228, 196)
#define RGB565_BLACK RGB565(0, 0, 0)
#define RGB565_BLANCHEDALMOND RGB565(255, 235, 205)
#define RGB565_BLUE RGB565(0, 0, 255)
#define RGB565_BLUEVIOLET RGB565(138, 43, 226)
#define RGB565_BROWN RGB565(165, 42, 42)
#define RGB565_BURLYWOOD RGB565(222, 184, 135)
#define RGB565_CADETBLUE RGB565(95, 158, 160)
#define RGB565_CHARTREUSE RGB565(127, 255, 0)
#define RGB565_CHOCOLATE RGB565(210, 105, 30)
#define RGB565_CORAL RGB565(255, 127, 80)
#define RGB565_CORNFLOWERBLUE RGB565(100, 149, 237)
#define RGB565_CORNSILK RGB565(255, 248, 220)
#define RGB565_CRIMSON RGB565(220, 20, 60)
#define RGB565_CYAN RGB565(0, 255, 255)
#define RGB565_DARKBLUE RGB565(0, 0, 139)
#define RGB565_DARKCYAN RGB565(0, 139, 139)
#define RGB565_DARKGOLDENROD RGB565(184, 134, 11)
#define RGB565_DARKGRAY RGB565(169, 169, 169)
#define RGB565_DARKGREEN RGB565(0, 100, 0)
#define RGB565_DARKGREY RGB565(169, 169, 169)
#define RGB565_DARKKHAKI RGB565(189, 183, 107)
#define RGB565_DARKMAGENTA RGB565(139, 0, 139)
#define RGB565_DARKOLIVEGREEN RGB565(85, 107, 47)
#define RGB565_DARKORANGE RGB565(255, 140, 0)
#define RGB565_DARKORCHID RGB565(153, 50, 204)
#define RGB565_DARKRED RGB565(139, 0, 0)
#define RGB565_DARKSALMON RGB565(233, 150, 122)
#define RGB565_DARKSEAGREEN RGB565(143, 188, 143)
#define RGB565_DARKSLATEBLUE RGB565(72, 61, 139)
#define RGB565_DARKSLATEGRAY RGB565(47, 79, 79)
#define RGB565_DARKSLATEGREY RGB565(47, 79, 79)
#define RGB565_DARKTURQUOISE RGB565(0, 206, 209)
#define RGB565_DARKVIOLET RGB565(148, 0, 211)
#define RGB565_DEEPPINK RGB565(255, 20, 147)
#define RGB565_DEEPSKYBLUE RGB565(0, 191, 255)
#define RGB565_DIMGRAY RGB565(105, 105, 105)
#define RGB565_DIMGREY RGB565(105, 105, 105)
#define RGB565_DODGERBLUE RGB565(30, 144, 255)
#define RGB565_FIREBRICK RGB565(178, 34, 34)
#define RGB565_FLORALWHITE RGB565(255, 250, 240)
#define RGB565_FORESTGREEN RGB565(34, 139, 34)
#define RGB565_FUCHSIA RGB565(255, 0, 255)
#define RGB565_GAINSBORO RGB565(220, 220, 220)
#define RGB565_GHOSTWHITE RGB565(248, 248, 255)
#define RGB565_GOLD RGB565(255, 215, 0)
#define RGB565_GOLDENROD RGB565(218, 165, 32)
#define RGB565_GRAY RGB565(128, 128, 128)
#define RGB565_GREEN RGB565(0, 128, 0)
#define RGB565_GREENYELLOW RGB565(173, 255, 47)
#define RGB565_GREY RGB565(128, 128, 128)
#define RGB565_HONEYDEW RGB565(240, 255, 240)
#define RGB565_HOTPINK RGB565(255, 105, 180)
#define RGB565_INDIANRED RGB565(205, 92, 92)
#define RGB565_INDIGO RGB565(75, 0, 130)
#define RGB565_IVORY RGB565(255, 255, 240)
#define RGB565_KHAKI RGB565(240, 230, 140)
#define RGB565_LAVENDER RGB565(230, 230, 250)
#define RGB565_LAVENDERBLUSH RGB565(255, 240, 245)
#define RGB565_LAWNGREEN RGB565(124, 252, 0)
#define RGB565_LEMONCHIFFON RGB565(255, 250, 205)
#define RGB565_LIGHTBLUE RGB565(173, 216, 230)
#define RGB565_LIGHTCORAL RGB565(240, 128, 128)
#define RGB565_LIGHTCYAN RGB565(224, 255, 255)
#define RGB565_LIGHTGOLDENRODYELLOW RGB565(250, 250, 210)
#define RGB565_LIGHTGRAY RGB565(211, 211, 211)
#define RGB565_LIGHTGREEN RGB565(144, 238, 144)
#define RGB565_LIGHTGREY RGB565(211, 211, 211)
#define RGB565_LIGHTPINK RGB565(255, 182, 193)
#define RGB565_LIGHTSALMON RGB565(255, 160, 122)
#define RGB565_LIGHTSEAGREEN RGB565(32, 178, 170)
#define RGB565_LIGHTSKYBLUE RGB565(135, 206, 250)
#define RGB565_LIGHTSLATEGRAY RGB565(119, 136, 153)
#define RGB565_LIGHTSLATEGREY RGB565(119, 136, 153)
#define RGB565_LIGHTSTEELBLUE RGB565(176, 196, 222)
#define RGB565_LIGHTYELLOW RGB565(255, 255, 224)
#define RGB565_LIME RGB565(0, 255, 0)
#define RGB565_LIMEGREEN RGB565(50, 205, 50)
#define RGB565_LINEN RGB565(250, 240, 230)
#define RGB565_MAGENTA RGB565(255, 0, 255)
#define RGB565_MAROON RGB565(128, 0, 0)
#define RGB565_MEDIUMAQUAMARINE RGB565(102, 205, 170)
#define RGB565_MEDIUMBLUE RGB565(0, 0, 205)
#define RGB565_MEDIUMORCHID RGB565(186, 85, 211)
#define RGB565_MEDIUMPURPLE RGB565(147, 112, 219)
#define RGB565_MEDIUMSEAGREEN RGB565(60, 179, 113)
#define RGB565_MEDIUMSLATEBLUE RGB565(123, 104, 238)
#define RGB565_MEDIUMSPRINGGREEN RGB565(0, 250, 154)
#define RGB565_MEDIUMTURQUOISE RGB565(72, 209, 204)
#define RGB565_MEDIUMVIOLETRED RGB565(199, 21, 133)
#define RGB565_MIDNIGHTBLUE RGB565(25, 25, 112)
#define RGB565_MINTCREAM RGB565(245, 255, 250)
#define RGB565_MISTYROSE RGB565(255, 228, 225)
#define RGB565_MOCCASIN RGB565(255, 228, 181)
#define RGB565_NAVAJOWHITE RGB565(255, 222, 173)
#define RGB565_NAVY RGB565(0, 0, 128)
#define RGB565_OLDLACE RGB565(253, 245, 230)
#define RGB565_OLIVE RGB565(128, 128, 0)
#define RGB565_OLIVEDRAB RGB565(107, 142, 35)
#define RGB565_ORANGE RGB565(255, 165, 0)
#define RGB565_ORANGERED RGB565(255, 69, 0)
#define RGB565_ORCHID RGB565(218, 112, 214)
#define RGB565_PALEGOLDENROD RGB565(238, 232, 170)
#define RGB565_PALEGREEN RGB565(152, 251, 152)
#define RGB565_PALETURQUOISE RGB565(175, 238, 238)
#define RGB565_PALEVIOLETRED RGB565(219, 112, 147)
#define RGB565_PAPAYAWHIP RGB565(255, 239, 213)
#define RGB565_PEACHPUFF RGB565(255, 218, 185)
#define RGB565_PERU RGB565(205, 133, 63)
#define RGB565_PINK RGB565(255, 192, 203)
#define RGB565_PLUM RGB565(221, 160, 221)
#define RGB565_POWDERBLUE RGB565(176, 224, 230)
#define RGB565_PURPLE RGB565(128, 0, 128)
#define RGB565_RED RGB565(255, 0, 0)
#define RGB565_ROSYBROWN RGB565(188, 143, 143)
#define RGB565_ROYALBLUE RGB565(65, 105, 225)
#define RGB565_SADDLEBROWN RGB565(139, 69, 19)
#define RGB565_SALMON RGB565(250, 128, 114)
#define RGB565_SANDYBROWN RGB565(244, 164, 96)
#define RGB565_SEAGREEN RGB565(46, 139, 87)
#define RGB565_SEASHELL RGB565(255, 245, 238)
#define RGB565_SIENNA RGB565(160, 82, 45)
#define RGB565_SILVER RGB565(192, 192, 192)
#define RGB565_SKYBLUE RGB565(135, 206, 235)
#define RGB565_SLATEBLUE RGB565(106, 90, 205)
#define RGB565_SLATEGRAY RGB565(112, 128, 144)
#define RGB565_SLATEGREY RGB565(112, 128, 144)
#define RGB565_SNOW RGB565(255, 250, 250)
#define RGB565_SPRINGGREEN RGB565(0, 255, 127)
#define RGB565_STEELBLUE RGB565(70, 130, 180)
#define RGB565_TAN RGB565(210, 180, 140)
#define RGB565_TEAL RGB565(0, 128, 128)
#define RGB565_THISTLE RGB565(216, 191, 216)
#define RGB565_TOMATO RGB565(255, 99, 71)
#define RGB565_TURQUOISE RGB565(64, 224, 208)
#define RGB565_VIOLET RGB565(238, 130, 238)
#define RGB565_WHEAT RGB565(245, 222, 179)
#define RGB565_WHITE RGB565(255, 255, 255)
#define RGB565_WHITESMOKE RGB565(245, 245, 245)
#define RGB565_YELLOW RGB565(255, 255, 0)
#define RGB565_YELLOWGREEN RGB565(154, 205, 50)

// Many (but maybe not all) non-AVR board installs define macros
// for compatibility with existing PROGMEM-reading AVR code.
// Do our own checks and defines here for good measure...

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_sbyte
#define pgm_read_sbyte(addr) (*(const signed char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif
// workaround of a15 asm compile error
#ifdef ESP8266
#undef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.  Try to accommodate both...

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#ifndef _swap_uint8_t
#define _swap_uint8_t(a, b) \
  {                         \
    uint8_t t = a;          \
    a = b;                  \
    b = t;                  \
  }
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
  {                         \
    int16_t t = a;          \
    a = b;                  \
    b = t;                  \
  }
#endif

#ifndef _diff
#define _diff(a, b) ((a > b) ? (a - b) : (b - a))
#endif

#ifndef _ordered_in_range
#define _ordered_in_range(v, a, b) ((a <= v) && (v <= b))
#endif

#ifndef _in_range
#define _in_range(v, a, b) ((a > b) ? _ordered_in_range(v, b, a) : _ordered_in_range(v, a, b))
#endif

#if !defined(ATTINY_CORE)
GFX_INLINE static GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c)
{
#ifdef __AVR__
  return &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
#else
  // expression in __AVR__ section may generate "dereferencing type-punned pointer will break strict-aliasing rules" warning
  // In fact, on other platforms (such as STM32) there is no need to do this pointer magic as program memory may be read in a usual way
  // So expression may be simplified
  return gfxFont->glyph + c;
#endif //__AVR__
}

GFX_INLINE static uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont)
{
#ifdef __AVR__
  return (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
#else
  // expression in __AVR__ section generates "dereferencing type-punned pointer will break strict-aliasing rules" warning
  // In fact, on other platforms (such as STM32) there is no need to do this pointer magic as program memory may be read in a usual way
  // So expression may be simplified
  return gfxFont->bitmap;
#endif //__AVR__
}
#endif // !defined(ATTINY_CORE)

/// A generic graphics superclass that can handle all sorts of drawing. At a minimum you can subclass and provide drawPixel(). At a maximum you can do a ton of overriding to optimize. Used for any/all Adafruit displays!
#if defined(LITTLE_FOOT_PRINT)
class Arduino_GFX : public Print
#else
class Arduino_GFX : public Print, public Arduino_G
#endif // !defined(LITTLE_FOOT_PRINT)
{
public:
  Arduino_GFX(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass:
  virtual bool begin(int32_t speed = GFX_NOT_DEFINED) = 0;
  virtual void writePixelPreclipped(int16_t x, int16_t y, uint16_t color) = 0;

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void startWrite();
  virtual void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  virtual void endWrite(void);

  // CONTROL API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void setRotation(uint8_t r);
  virtual void invertDisplay(bool i);
  virtual void displayOn();
  virtual void displayOff();
  bool enableRoundMode();

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  // It's good to implement those, even if using transaction API
  void writePixel(int16_t x, int16_t y, uint16_t color);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillScreen(uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
  void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
  void draw16bitRGBBitmapWithMask(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
  void draw24bitRGBBitmap(int16_t x, int16_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
  void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
  void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void setTextSize(uint8_t s);
  void setTextSize(uint8_t sx, uint8_t sy);
  void setTextSize(uint8_t sx, uint8_t sy, uint8_t pixel_margin);

#if !defined(ATTINY_CORE)
  void setFont(const GFXfont *f = NULL);
#if defined(U8G2_FONT_SUPPORT)
  void setFont(const uint8_t *font);
  void setUTF8Print(bool isEnable);
  uint16_t u8g2_font_get_word(const uint8_t *font, uint8_t offset);
  uint8_t u8g2_font_decode_get_unsigned_bits(uint8_t cnt);
  int8_t u8g2_font_decode_get_signed_bits(uint8_t cnt);
  void u8g2_font_decode_len(uint8_t len, uint8_t is_foreground, uint16_t color, uint16_t bg);
#endif // defined(U8G2_FONT_SUPPORT)
  virtual void flush(bool force_flush = false);
#endif // !defined(ATTINY_CORE)

  // adopt from LovyanGFX
  void drawEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry, uint16_t color);
  void writeEllipseHelper(int32_t x, int32_t y, int32_t rx, int32_t ry, uint8_t cornername, uint16_t color);
  void fillEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry, uint16_t color);
  void writeFillEllipseHelper(int32_t x, int32_t y, int32_t rx, int32_t ry, uint8_t cornername, int16_t delta, uint16_t color);
  void drawArc(int16_t x, int16_t y, int16_t r1, int16_t r2, float start, float end, uint16_t color);
  void fillArc(int16_t x, int16_t y, int16_t r1, int16_t r2, float start, float end, uint16_t color);
  void writeFillArcHelper(int16_t cx, int16_t cy, int16_t oradius, int16_t iradius, float start, float end, uint16_t color);

// TFT optimization code, too big for ATMEL family
#if defined(LITTLE_FOOT_PRINT)
  void writeSlashLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
  void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
  void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h, int16_t x_skip = 0);
  void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, uint8_t chroma_key, int16_t w, int16_t h, int16_t x_skip = 0);
  void draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  void draw16bitRGBBitmapWithMask(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
  void draw16bitRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  void draw16bitRGBBitmapWithTranColor(int16_t x, int16_t y, uint16_t *bitmap, uint16_t transparent_color, int16_t w, int16_t h);
  void draw16bitBeRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  void draw24bitRGBBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg);
#else  // !defined(LITTLE_FOOT_PRINT)
  virtual void writeSlashLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  virtual void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg);
  virtual void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
  virtual void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  virtual void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  virtual void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, int16_t w, int16_t h, int16_t x_skip = 0);
  virtual void drawIndexedBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint16_t *color_index, uint8_t chroma_key, int16_t w, int16_t h, int16_t x_skip = 0);
  virtual void draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  virtual void draw16bitRGBBitmapWithMask(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
  virtual void draw16bitRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  virtual void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  virtual void draw16bitRGBBitmapWithTranColor(int16_t x, int16_t y, uint16_t *bitmap, uint16_t transparent_color, int16_t w, int16_t h);
  virtual void draw16bitBeRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  virtual void draw24bitRGBBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
  virtual void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
  virtual void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg);

  virtual void draw16bitBeRGBBitmapR1(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
#endif // !defined(LITTLE_FOOT_PRINT)

  /**********************************************************************/
  /*!
    @brief  Set text cursor location
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
  */
  /**********************************************************************/
  void setCursor(int16_t x, int16_t y)
  {
    cursor_x = x;
    cursor_y = y;
  }

  /**********************************************************************/
  /*!
    @brief  Set text bound for printing
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
  */
  void setTextBound(int16_t x, int16_t y, int16_t w, int16_t h)
  {
    _min_text_x = x;
    _min_text_y = y;
    _max_text_x = x + w - 1;
    _max_text_y = y + h - 1;
  }

  /**********************************************************************/
  /*!
    @brief   Set text font color with transparant background
    @param   c   16-bit 5-6-5 Color to draw text with
    @note    For 'transparent' background, background and foreground
             are set to same color rather than using a separate flag.
  */
  /**********************************************************************/
  void setTextColor(uint16_t c) { textcolor = textbgcolor = c; }

  /**********************************************************************/
  /*!
    @brief   Set text font color with custom background color
    @param   c   16-bit 5-6-5 Color to draw text with
    @param   bg  16-bit 5-6-5 Color to draw background/fill with
  */
  /**********************************************************************/
  void setTextColor(uint16_t c, uint16_t bg)
  {
    textcolor = c;
    textbgcolor = bg;
  }

  /**********************************************************************/
  /*!
  @brief  Set whether text that is too long for the screen width should
          automatically wrap around to the next line (else clip right).
  @param  w  true for wrapping, false for clipping
  */
  /**********************************************************************/
  void setTextWrap(bool w) { wrap = w; }

  virtual size_t write(uint8_t);

  /************************************************************************/
  /*!
    @brief      Get width of the display, accounting for current rotation
    @returns    Width in pixels
  */
  /************************************************************************/
  int16_t width(void) const { return _width; };

  /************************************************************************/
  /*!
    @brief      Get height of the display, accounting for current rotation
    @returns    Height in pixels
  */
  /************************************************************************/
  int16_t height(void) const { return _height; }

  /************************************************************************/
  /*!
    @brief      Get rotation setting for display
    @returns    0 thru 3 corresponding to 4 cardinal rotations
  */
  /************************************************************************/
  uint8_t getRotation(void) const { return _rotation; }

  // get current cursor position (get rotation safe maximum values,
  // using: width() for x, height() for y)
  /************************************************************************/
  /*!
    @brief  Get text cursor X location
    @returns    X coordinate in pixels
  */
  /************************************************************************/
  int16_t getCursorX(void) const { return cursor_x; }

  /************************************************************************/
  /*!
    @brief      Get text cursor Y location
    @returns    Y coordinate in pixels
  */
  /************************************************************************/
  int16_t getCursorY(void) const { return cursor_y; };

  /*!
    @brief   Given 8-bit red, green and blue values, return a 'packed'
             16-bit color value in '565' RGB format (5 bits red, 6 bits
             green, 5 bits blue). This is just a mathematical operation,
             no hardware is touched.
    @param   red    8-bit red brightnesss (0 = off, 255 = max).
    @param   green  8-bit green brightnesss (0 = off, 255 = max).
    @param   blue   8-bit blue brightnesss (0 = off, 255 = max).
    @return  'Packed' 16-bit color value (565 format).
  */
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue)
  {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
  }

protected:
  void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
  int16_t
      _width,  ///< Display width as modified by current rotation
      _height, ///< Display height as modified by current rotation
      _max_x,  ///< x zero base bound (_width - 1)
      _max_y,  ///< y zero base bound (_height - 1)
      _min_text_x,
      _min_text_y,
      _max_text_x,
      _max_text_y,
      cursor_x, ///< x location to start print()ing text
      cursor_y; ///< y location to start print()ing text
  uint16_t
      textcolor,   ///< 16-bit background color for print()
      textbgcolor; ///< 16-bit text color for print()
  uint8_t
      textsize_x,        ///< Desired magnification in X-axis of text to print()
      textsize_y,        ///< Desired magnification in Y-axis of text to print()
      text_pixel_margin, ///< Margin for each text pixel
      _rotation;         ///< Display rotation (0 thru 3)
  bool
      wrap; ///< If set, 'wrap' text at right edge of display
#if !defined(ATTINY_CORE)
  GFXfont *gfxFont; ///< Pointer to special font
#endif              // !defined(ATTINY_CORE)

#if defined(U8G2_FONT_SUPPORT)
  uint8_t *u8g2Font;
  bool _enableUTF8Print = false;
  uint8_t _utf8_state = 0;
  uint16_t _encoding;

  uint8_t _u8g2_glyph_cnt;
  uint8_t _u8g2_bits_per_0;
  uint8_t _u8g2_bits_per_1;
  uint8_t _u8g2_bits_per_char_width;
  uint8_t _u8g2_bits_per_char_height;
  uint8_t _u8g2_bits_per_char_x;
  uint8_t _u8g2_bits_per_char_y;
  uint8_t _u8g2_bits_per_delta_x;
  int8_t _u8g2_max_char_width;
  int8_t _u8g2_max_char_height;
  uint16_t _u8g2_start_pos_upper_A;
  uint16_t _u8g2_start_pos_lower_a;
  uint16_t _u8g2_start_pos_unicode;
  uint8_t _u8g2_first_char;

  uint8_t _u8g2_char_width;
  uint8_t _u8g2_char_height;
  int8_t _u8g2_char_x;
  int8_t _u8g2_char_y;
  int8_t _u8g2_delta_x;

  int8_t _u8g2_dx;
  int8_t _u8g2_dy;
  int16_t _u8g2_target_x;
  int16_t _u8g2_target_y;

  const uint8_t *_u8g2_decode_ptr;
  uint8_t _u8g2_decode_bit_pos;
#endif // defined(U8G2_FONT_SUPPORT)

#if defined(LITTLE_FOOT_PRINT)
  int16_t
      WIDTH,  ///< This is the 'raw' display width - never changes
      HEIGHT; ///< This is the 'raw' display height - never changes
#endif        // defined(LITTLE_FOOT_PRINT)

  bool _isRoundMode = false;
  int16_t *_roundMinX;
  int16_t *_roundMaxX;
};

#endif // _ARDUINO_GFX_H_
