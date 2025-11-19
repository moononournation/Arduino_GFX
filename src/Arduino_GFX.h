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
#include "font/u8g2_font_unifont_h_chinese.h"
#include "font/u8g2_font_unifont_h_chinese4.h"
#include "font/u8g2_font_unifont_h_cjk.h"
#include "font/u8g2_font_unifont_h_utf8.h"
#endif

#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#define RGB16TO24(c) ((((uint32_t)c & 0xF800) << 8) | ((c & 0x07E0) << 5) | ((c & 0x1F) << 3))

/* https://www.w3.org/wiki/CSS/Properties/color/keywords */
#define RGB565_ALICEBLUE RGB565(240, 248, 248)
#define RGB565_ANTIQUEWHITE RGB565(248, 236, 216)
#define RGB565_AQUA RGB565(0, 252, 248)
#define RGB565_AQUAMARINE RGB565(128, 252, 216)
#define RGB565_AZURE RGB565(240, 252, 248)
#define RGB565_BEIGE RGB565(248, 244, 224)
#define RGB565_BISQUE RGB565(248, 228, 200)
#define RGB565_BLACK RGB565(0, 0, 0)
#define RGB565_BLANCHEDALMOND RGB565(248, 236, 208)
#define RGB565_BLUE RGB565(0, 0, 248)
#define RGB565_BLUEVIOLET RGB565(136, 44, 224)
#define RGB565_BROWN RGB565(168, 44, 40)
#define RGB565_BURLYWOOD RGB565(224, 184, 136)
#define RGB565_CADETBLUE RGB565(96, 160, 160)
#define RGB565_CHARTREUSE RGB565(128, 252, 0)
#define RGB565_CHOCOLATE RGB565(208, 104, 32)
#define RGB565_CORAL RGB565(248, 128, 80)
#define RGB565_CORNFLOWERBLUE RGB565(104, 148, 240)
#define RGB565_CORNSILK RGB565(248, 248, 224)
#define RGB565_CRIMSON RGB565(224, 20, 64)
#define RGB565_CYAN RGB565(0, 252, 248)
#define RGB565_DARKBLUE RGB565(0, 0, 136)
#define RGB565_DARKCYAN RGB565(0, 140, 136)
#define RGB565_DARKGOLDENROD RGB565(184, 136, 8)
#define RGB565_DARKGRAY RGB565(168, 168, 168)
#define RGB565_DARKGREEN RGB565(0, 100, 0)
#define RGB565_DARKGREY RGB565(168, 168, 168)
#define RGB565_DARKKHAKI RGB565(192, 184, 104)
#define RGB565_DARKMAGENTA RGB565(136, 0, 136)
#define RGB565_DARKOLIVEGREEN RGB565(88, 108, 48)
#define RGB565_DARKORANGE RGB565(248, 140, 0)
#define RGB565_DARKORCHID RGB565(152, 52, 208)
#define RGB565_DARKRED RGB565(136, 0, 0)
#define RGB565_DARKSALMON RGB565(232, 152, 120)
#define RGB565_DARKSEAGREEN RGB565(144, 188, 144)
#define RGB565_DARKSLATEBLUE RGB565(72, 60, 136)
#define RGB565_DARKSLATEGRAY RGB565(48, 80, 80)
#define RGB565_DARKSLATEGREY RGB565(48, 80, 80)
#define RGB565_DARKTURQUOISE RGB565(0, 208, 208)
#define RGB565_DARKVIOLET RGB565(152, 0, 208)
#define RGB565_DEEPPINK RGB565(248, 20, 144)
#define RGB565_DEEPSKYBLUE RGB565(0, 192, 248)
#define RGB565_DIMGRAY RGB565(104, 104, 104)
#define RGB565_DIMGREY RGB565(104, 104, 104)
#define RGB565_DODGERBLUE RGB565(32, 144, 248)
#define RGB565_FIREBRICK RGB565(176, 36, 32)
#define RGB565_FLORALWHITE RGB565(248, 252, 240)
#define RGB565_FORESTGREEN RGB565(32, 140, 32)
#define RGB565_FUCHSIA RGB565(248, 0, 248)
#define RGB565_GAINSBORO RGB565(224, 220, 224)
#define RGB565_GHOSTWHITE RGB565(248, 248, 248)
#define RGB565_GOLD RGB565(248, 216, 0)
#define RGB565_GOLDENROD RGB565(216, 164, 32)
#define RGB565_GRAY RGB565(128, 128, 128)
#define RGB565_GREEN RGB565(0, 128, 0)
#define RGB565_GREENYELLOW RGB565(176, 252, 48)
#define RGB565_GREY RGB565(128, 128, 128)
#define RGB565_HONEYDEW RGB565(240, 252, 240)
#define RGB565_HOTPINK RGB565(248, 104, 184)
#define RGB565_INDIANRED RGB565(208, 92, 96)
#define RGB565_INDIGO RGB565(72, 0, 128)
#define RGB565_IVORY RGB565(248, 252, 240)
#define RGB565_KHAKI RGB565(240, 232, 144)
#define RGB565_LAVENDER RGB565(232, 232, 248)
#define RGB565_LAVENDERBLUSH RGB565(248, 240, 248)
#define RGB565_LAWNGREEN RGB565(128, 252, 0)
#define RGB565_LEMONCHIFFON RGB565(248, 252, 208)
#define RGB565_LIGHTBLUE RGB565(176, 216, 232)
#define RGB565_LIGHTCORAL RGB565(240, 128, 128)
#define RGB565_LIGHTCYAN RGB565(224, 252, 248)
#define RGB565_LIGHTGOLDENRODYELLOW RGB565(248, 252, 208)
#define RGB565_LIGHTGRAY RGB565(208, 212, 208)
#define RGB565_LIGHTGREEN RGB565(144, 240, 144)
#define RGB565_LIGHTGREY RGB565(208, 212, 208)
#define RGB565_LIGHTPINK RGB565(248, 184, 192)
#define RGB565_LIGHTSALMON RGB565(248, 160, 120)
#define RGB565_LIGHTSEAGREEN RGB565(32, 180, 168)
#define RGB565_LIGHTSKYBLUE RGB565(136, 208, 248)
#define RGB565_LIGHTSLATEGRAY RGB565(120, 136, 152)
#define RGB565_LIGHTSLATEGREY RGB565(120, 136, 152)
#define RGB565_LIGHTSTEELBLUE RGB565(176, 196, 224)
#define RGB565_LIGHTYELLOW RGB565(248, 252, 224)
#define RGB565_LIME RGB565(0, 252, 0)
#define RGB565_LIMEGREEN RGB565(48, 204, 48)
#define RGB565_LINEN RGB565(248, 240, 232)
#define RGB565_MAGENTA RGB565(248, 0, 248)
#define RGB565_MAROON RGB565(128, 0, 0)
#define RGB565_MEDIUMAQUAMARINE RGB565(104, 204, 168)
#define RGB565_MEDIUMBLUE RGB565(0, 0, 208)
#define RGB565_MEDIUMORCHID RGB565(184, 84, 208)
#define RGB565_MEDIUMPURPLE RGB565(144, 112, 216)
#define RGB565_MEDIUMSEAGREEN RGB565(64, 180, 112)
#define RGB565_MEDIUMSLATEBLUE RGB565(120, 104, 240)
#define RGB565_MEDIUMSPRINGGREEN RGB565(0, 252, 152)
#define RGB565_MEDIUMTURQUOISE RGB565(72, 208, 208)
#define RGB565_MEDIUMVIOLETRED RGB565(200, 20, 136)
#define RGB565_MIDNIGHTBLUE RGB565(24, 24, 112)
#define RGB565_MINTCREAM RGB565(248, 252, 248)
#define RGB565_MISTYROSE RGB565(248, 228, 224)
#define RGB565_MOCCASIN RGB565(248, 228, 184)
#define RGB565_NAVAJOWHITE RGB565(248, 224, 176)
#define RGB565_NAVY RGB565(0, 0, 128)
#define RGB565_OLDLACE RGB565(248, 244, 232)
#define RGB565_OLIVE RGB565(128, 128, 0)
#define RGB565_OLIVEDRAB RGB565(104, 144, 32)
#define RGB565_ORANGE RGB565(248, 164, 0)
#define RGB565_ORANGERED RGB565(248, 68, 0)
#define RGB565_ORCHID RGB565(216, 112, 216)
#define RGB565_PALEGOLDENROD RGB565(240, 232, 168)
#define RGB565_PALEGREEN RGB565(152, 252, 152)
#define RGB565_PALETURQUOISE RGB565(176, 240, 240)
#define RGB565_PALEVIOLETRED RGB565(216, 112, 144)
#define RGB565_PAPAYAWHIP RGB565(248, 240, 216)
#define RGB565_PEACHPUFF RGB565(248, 220, 184)
#define RGB565_PERU RGB565(208, 132, 64)
#define RGB565_PINK RGB565(248, 192, 200)
#define RGB565_PLUM RGB565(224, 160, 224)
#define RGB565_POWDERBLUE RGB565(176, 224, 232)
#define RGB565_PURPLE RGB565(128, 0, 128)
#define RGB565_RED RGB565(248, 0, 0)
#define RGB565_ROSYBROWN RGB565(192, 144, 144)
#define RGB565_ROYALBLUE RGB565(64, 104, 224)
#define RGB565_SADDLEBROWN RGB565(136, 68, 16)
#define RGB565_SALMON RGB565(248, 128, 112)
#define RGB565_SANDYBROWN RGB565(248, 164, 96)
#define RGB565_SEAGREEN RGB565(48, 140, 88)
#define RGB565_SEASHELL RGB565(248, 244, 240)
#define RGB565_SIENNA RGB565(160, 84, 48)
#define RGB565_SILVER RGB565(192, 192, 192)
#define RGB565_SKYBLUE RGB565(136, 208, 232)
#define RGB565_SLATEBLUE RGB565(104, 92, 208)
#define RGB565_SLATEGRAY RGB565(112, 128, 144)
#define RGB565_SLATEGREY RGB565(112, 128, 144)
#define RGB565_SNOW RGB565(248, 252, 248)
#define RGB565_SPRINGGREEN RGB565(0, 252, 128)
#define RGB565_STEELBLUE RGB565(72, 132, 184)
#define RGB565_TAN RGB565(208, 180, 144)
#define RGB565_TEAL RGB565(0, 128, 128)
#define RGB565_THISTLE RGB565(216, 192, 216)
#define RGB565_TOMATO RGB565(248, 100, 72)
#define RGB565_TURQUOISE RGB565(64, 224, 208)
#define RGB565_VIOLET RGB565(240, 132, 240)
#define RGB565_WHEAT RGB565(248, 224, 176)
#define RGB565_WHITE RGB565(248, 252, 248)
#define RGB565_WHITESMOKE RGB565(248, 244, 248)
#define RGB565_YELLOW RGB565(248, 252, 0)
#define RGB565_YELLOWGREEN RGB565(152, 204, 48)

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
