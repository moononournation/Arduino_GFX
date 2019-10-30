/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.
*/

#include "SPI.h"
#include "Arduino_HWSPI.h"
#include "Arduino_ESP32SPI.h"
#include "Arduino_SWSPI.h"
#include "Arduino_GFX.h"            // Core graphics library by Adafruit
#include "Arduino_Canvas.h"         // Canvas (framebuffer) library
#include "Arduino_Canvas_Indexed.h" // Indexed Color Canvas (framebuffer) library
#include "Arduino_HX8347C.h"        // Hardware-specific library for HX8347C
#include "Arduino_HX8352C.h"        // Hardware-specific library for HX8352C
#include "Arduino_HX8357B.h"        // Hardware-specific library for HX8357B
#include "Arduino_ILI9225.h"        // Hardware-specific library for ILI9225
#include "Arduino_ILI9341.h"        // Hardware-specific library for ILI9341
#include "Arduino_ILI9486.h"        // Hardware-specific library for ILI9486
#include "Arduino_SEPS525.h"        // Hardware-specific library for SEPS525
#include "Arduino_SSD1283A.h"       // Hardware-specific library for SSD1283A
#include "Arduino_SSD1331.h"        // Hardware-specific library for SSD1331
#include "Arduino_SSD1351.h"        // Hardware-specific library for SSD1351
#include "Arduino_ST7735.h"         // Hardware-specific library for ST7735
#include "Arduino_ST7789.h"         // Hardware-specific library for ST7789
#include "Arduino_ST7796.h"         // Hardware-specific library for ST7796

#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
#define TFT_BL 32
#include "Arduino_ILI9341_M5STACK.h"
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(27 /* DC */, 14 /* CS */, SCK, MOSI, MISO);
Arduino_ILI9341_M5STACK *tft = new Arduino_ILI9341_M5STACK(bus, 33 /* RST */, 1 /* rotation */);
#elif defined(ARDUINO_ODROID_ESP32)
#define TFT_BL 14
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(21 /* DC */, 5 /* CS */, SCK, MOSI, MISO);
Arduino_ILI9341 *tft = new Arduino_ILI9341(bus, -1 /* RST */, 1 /* rotation */);
// Arduino_ST7789 *tft = new Arduino_ST7789(bus,  -1 /* RST */, 0 /* rotation */, true /* IPS */);
#elif defined(ARDUINO_T) // TTGO T-Watch
#define TFT_BL 12
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 18 /* SCK */, 19 /* MOSI */, -1 /* MISO */);
Arduino_ST7789 *tft = new Arduino_ST7789(bus, -1 /* RST */, 2 /* rotation */, true /* IPS */, 240, 240, 0, 80);
#else /* not a specific hardware */

#if defined(ESP32)
#define TFT_CS 5
// #define TFT_CS -1 // for display without CS pin
// #define TFT_DC 16
#define TFT_DC 27
// #define TFT_DC -1 // for display without DC pin (9-bit SPI)
// #define TFT_RST 17
#define TFT_RST 33
#define TFT_BL 22
#elif defined(ESP8266)
#define TFT_CS 15
#define TFT_DC 5
#define TFT_RST -1
// #define TFT_BL 4
#else
#define TFT_CS 20
#define TFT_DC 19
#define TFT_RST 18
#define TFT_BL 10
#endif

/*
 * Step 1: Initize one databus for your display
*/

// General software SPI
// Arduino_DataBus *bus = new Arduino_SWSPI(TFT_DC, TFT_CS, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */);

// General hardware SPI
// Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);

// ESP32 hardware SPI, more customizable parameters
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */, VSPI /* spi_num */);

/*
 * Step 2: Initize one driver for your display
*/

// Canvas (framebuffer)
// Arduino_HX8347C *output_display = new Arduino_HX8347C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// 16-bit color Canvas (240x320 resolution only works for ESP32 with PSRAM)
// Arduino_Canvas *tft = new Arduino_Canvas(240, 320, output_display);
// Indexed color Canvas
// Arduino_Canvas_Indexed *tft = new Arduino_Canvas_Indexed(240, 320, output_display, 0xC618 /* color_mask */);

// HX8347C IPS LCD 240x320
// Arduino_HX8347C *tft = new Arduino_HX8347C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8352C IPS LCD 240x400
// Arduino_HX8352C *tft = new Arduino_HX8352C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8357B IPS LCD 320x480
// Arduino_HX8357B *tft = new Arduino_HX8357B(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// ILI9225 LCD 176x220
// Arduino_ILI9225 *tft = new Arduino_ILI9225(bus, TFT_RST);

// ILI9341 LCD 240x320
// Arduino_ILI9341 *tft = new Arduino_ILI9341(bus, TFT_RST);

// ILI9486 LCD 320x480
// Arduino_ILI9486 *tft = new Arduino_ILI9486(bus, TFT_RST);

// SEPS525 OLED 160x128
// Arduino_SEPS525 *tft = new Arduino_SEPS525(bus, TFT_RST);

// SSD1283A OLED 130x130
// Arduino_SSD1283A *tft = new Arduino_SSD1283A(bus, TFT_RST);

// SSD1331 OLED 96x64
// Arduino_SSD1331 *tft = new Arduino_SSD1331(bus, TFT_RST);

// SSD1351 OLED 128x128
// Arduino_SSD1351 *tft = new Arduino_SSD1351(bus, TFT_RST);

// ST7735 LCD
// 1.8" REDTAB 128x160
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST);
// 1.8" BLACKTAB 128x160
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */, false /* BGR */);
// 1.8" GREENTAB A 128x160
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" GREENTAB B 128x160
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 3 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" Wide angle LCD 128x160
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 2 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */, false /* BGR */);
// 1.5" GREENTAB B 128x128
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 128 /* height */, 2 /* col offset 1 */, 3 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.5" GREENTAB C 128x128
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 128 /* height */, 0 /* col offset 1 */, 32 /* row offset 1 */);
// 0.96" IPS LCD 80x160
// Arduino_ST7735 *tft = new Arduino_ST7735(bus, TFT_RST, 3 /* rotation */, true /* IPS */, 80 /* width */, 160 /* height */, 26 /* col offset 1 */, 1 /* row offset 1 */, 26 /* col offset 2 */, 1 /* row offset 2 */);

// ST7789 LCD
// 2.4" LCD 240x320
// Arduino_ST7789 *tft = new Arduino_ST7789(bus, TFT_RST);
// 2.4" IPS LCD 240x320
// Arduino_ST7789 *tft = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// 1.3"/1.5" square IPS LCD 240x240
// Arduino_ST7789 *tft = new Arduino_ST7789(bus, TFT_RST, 2 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */, 0 /* col offset 1 */, 80 /* row offset 1 */);
// 1.14" IPS LCD 135x240 TTGO T-Display
// Arduino_ST7789 *tft = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */, 53 /* col offset 1 */, 40 /* row offset 1 */, 52 /* col offset 2 */, 40 /* row offset 2 */);

// ST7796 LCD
// 4" LCD 320x480
Arduino_ST7796 *tft = new Arduino_ST7796(bus, TFT_RST);
// 4" IPS LCD 320x480
// Arduino_ST7796 *tft = new Arduino_ST7796(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

#endif /* not a specific hardware */

uint32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

unsigned long total = 0;
unsigned long tn = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // wait and do nothing
  }

  Serial.println("Arduino_GFX library Test!");

  tft->begin();
  // tft->begin(80000000); /* specify data bus speed */

  w = tft->width();
  h = tft->height();
  n = min(w, h);
  n1 = min(w, h) - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = min(cx1, cy1) - 1;
  tsa = (w <= 160) ? 1 : ((w <= 240) ? 2 : 3); // text size A
  tsb = (w <= 240) ? 1 : 2;                    // text size B
  tsc = (w <= 220) ? 1 : 2;                    // text size C
  ds = (w <= 160) ? 9 : 12;                    // digit size

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif
}

void loop(void)
{

  Serial.println(F("Benchmark                Time (microseconds)"));

  uint32_t usecFillScreen = testFillScreen();
  tft->flush();
  Serial.print(F("Screen fill              "));
  Serial.println(usecFillScreen);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecText = testText();
  tft->flush();
  Serial.print(F("Text                     "));
  Serial.println(usecText);
  delay(3000); // delay for verifing the text

  tft->fillScreen(BLACK);
  uint32_t usecPixels = testPixels();
  tft->flush();
  Serial.print(F("Pixels                   "));
  Serial.println(usecPixels);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecLines = testLines(BLUE);
  tft->flush();
  Serial.print(F("Lines                    "));
  Serial.println(usecLines);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecFastLines = testFastLines(RED, BLUE);
  tft->flush();
  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(usecFastLines);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecFilledRects = testFilledRects(YELLOW, MAGENTA);
  tft->flush();
  Serial.print(F("Rectangles (filled)      "));
  Serial.println(usecFilledRects);
  delay(100);

  uint32_t usecRects = testRects(GREEN);
  tft->flush();
  Serial.print(F("Rectangles (outline)     "));
  Serial.println(usecRects);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecFilledCircles = testFilledCircles(10, MAGENTA);
  tft->flush();
  Serial.print(F("Circles (filled)         "));
  Serial.println(usecFilledCircles);
  delay(100);

  uint32_t usecCircles = testCircles(10, WHITE);
  tft->flush();
  Serial.print(F("Circles (outline)        "));
  Serial.println(usecCircles);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecFilledTrangles = testFilledTriangles();
  tft->flush();
  Serial.print(F("Triangles (filled)       "));
  Serial.println(usecFilledTrangles);
  delay(100);

  uint32_t usecTriangles = testTriangles();
  tft->flush();
  Serial.print(F("Triangles (outline)      "));
  Serial.println(usecTriangles);
  delay(100);

  tft->fillScreen(BLACK);
  uint32_t usecFilledRoundRects = testFilledRoundRects();
  tft->flush();
  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(usecFilledRoundRects);
  delay(100);

  uint32_t usecRoundRects = testRoundRects();
  tft->flush();
  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(usecRoundRects);
  delay(100);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    tft->drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11)
      d = -d;
  }

  tft->setCursor(0, 0);
  tft->setTextColor(MAGENTA);
  tft->setTextSize(tsa);
  tft->println(F("Arduino GFX"));
  tft->setTextSize(1);
  tft->println(F(""));

  if (h > w)
  {
    tft->setTextColor(GREEN);
    tft->setTextSize(tsb);
    tft->print(F("Benchmark "));
    tft->setTextSize(tsc);
    if (ds == 12)
    {
      tft->print(F("   "));
    }
    tft->println(F("micro-secs"));
    tft->setTextSize(1);
    tft->println(F(""));
    tft->setTextColor(YELLOW);
  }

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Screen fill "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFillScreen);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Text        "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecText);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Pixels      "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecPixels);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Lines       "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecLines);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("H/V Lines   "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFastLines);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Rectangles F"));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledRects);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Rectangles  "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecRects);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Circles F   "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledCircles);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Circles     "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecCircles);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Triangles F "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledTrangles);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Triangles   "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecTriangles);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("RoundRects F"));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledRoundRects);

  tft->setTextColor(CYAN);
  tft->setTextSize(tsb);
  tft->print(F("RoundRects  "));
  tft->setTextColor(YELLOW);
  tft->setTextSize(tsc);
  printnice(usecRoundRects);

  if (h > w)
  {
    tft->setTextSize(1);
    tft->println(F(""));
    tft->setTextColor(GREEN);
    tft->setTextSize(tsc);
    tft->print(F("Benchmark Complete!"));
  }

  tft->flush();

  delay(60 * 1000L);
}

void printnice(int32_t v)
{
  char str[32] = {0};
  sprintf(str, "%lu", v);
  for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
  {
    memmove(p + 1, p, strlen(p) + 1);
    *p = ',';
  }
  while (strlen(str) < ds)
  {
    memmove(str + 1, str, strlen(str) + 1);
    *str = ' ';
  }
  tft->println(str);
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}

uint32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  tft->fillScreen(WHITE);
  tft->fillScreen(RED);
  tft->fillScreen(GREEN);
  tft->fillScreen(BLUE);
  tft->fillScreen(BLACK);

  return (micros() - start) / 5;
}

uint32_t testText()
{
  uint32_t start = micros_start();
  tft->setCursor(0, 0);
  tft->setTextSize(1);
  tft->setTextColor(WHITE, BLACK);
  tft->println(F("Hello World!"));
  tft->setTextSize(2);
  tft->setTextColor(tft->color565(0xff, 0x00, 0x00));
  tft->print(F("RED "));
  tft->setTextColor(tft->color565(0x00, 0xff, 0x00));
  tft->print(F("GREEN "));
  tft->setTextColor(tft->color565(0x00, 0x00, 0xff));
  tft->println(F("BLUE"));
  tft->setTextSize(tsa);
  tft->setTextSize(3);
  tft->setTextColor(YELLOW);
  tft->println(1234.56);
  tft->setTextColor(WHITE);
  tft->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);
  tft->setTextColor(CYAN, WHITE);
  tft->println(F("Groop,"));
  tft->setTextSize(tsc);
  tft->setTextColor(MAGENTA, WHITE);
  tft->println(F("I implore thee,"));
  tft->setTextSize(1);
  tft->setTextColor(NAVY, WHITE);
  tft->println(F("my foonting turlingdromes."));
  tft->setTextColor(DARKGREEN, WHITE);
  tft->println(F("And hooptiously drangle me"));
  tft->setTextColor(DARKCYAN, WHITE);
  tft->println(F("with crinkly bindlewurdles,"));
  tft->setTextColor(MAROON, WHITE);
  tft->println(F("Or I will rend thee"));
  tft->setTextColor(PURPLE, WHITE);
  tft->println(F("in the gobberwartsb"));
  tft->setTextColor(OLIVE, WHITE);
  tft->println(F("with my blurglecruncheon,"));
  tft->setTextColor(DARKGREY, WHITE);
  tft->println(F("see if I don't!"));
  if (h > 160)
  {
    tft->setTextColor(ORANGE);
    tft->setTextSize(4);
    tft->println(F("Size 4"));
  }
  if (h > 220)
  {
    tft->setTextColor(GREENYELLOW);
    tft->setTextSize(5);
    tft->println(F("Size 5"));
  }
  if (h > 240)
  {
    tft->setTextColor(PINK);
    tft->setTextSize(6);
    tft->println(F("Size 6"));
  }
  uint32_t t = micros() - start;
  return t;
}

uint32_t testPixels()
{
  uint32_t start = micros_start();

  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      tft->drawPixel(x, y, tft->color565(x << 3, y << 3, x * y));
    }
  }

  return micros() - start;
}

uint32_t testLines(uint16_t color)
{
  uint32_t start, t;
  int32_t x1, y1, x2, y2;

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  t = micros() - start; // fillScreen doesn't count against timing

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }
  t += micros() - start;

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
    tft->drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    tft->drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
  uint32_t start, t = 0;
  int32_t i, i2;

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    tft->fillRect(cx1 - i2, cy1 - i2, i, i, color1);

    t += micros() - start;

    // Outlines are not included in timing results
    tft->drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

uint32_t testRects(uint16_t color)
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft->drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft->fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w1 = w + radius;
  int32_t h1 = h + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      tft->drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testFilledTriangles()
{
  uint32_t start, t = 0;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    start = micros_start();
    tft->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      tft->color565(0, i, i));
    t += micros() - start;
    tft->drawTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      tft->color565(i, i, 0));
  }

  return t;
}

uint32_t testTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = 0; i < cn; i += 5)
  {
    tft->drawTriangle(
        cx1, cy1 - i,     // peak
        cx1 - i, cy1 + i, // bottom left
        cx1 + i, cy1 + i, // bottom right
        tft->color565(0, 0, i));
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    tft->fillRoundRect(cx1 - i2, cy1 - i2, i, i, i / 8, tft->color565(0, i, 0));
  }

  return micros() - start;
}

uint32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 0; i < n1; i += 6)
  {
    i2 = i / 2;
    tft->drawRoundRect(cx1 - i2, cy1 - i2, i, i, i / 8, tft->color565(i, 0, 0));
  }

  return micros() - start;
}

/***************************************************
  Original sketch text:

  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
