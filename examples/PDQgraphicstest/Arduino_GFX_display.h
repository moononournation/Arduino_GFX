/***************************************
 * Start of Canvas (framebuffer)
 **************************************/
// #define CANVAS

// 16-bit color Canvas (240x320 resolution only works for ESP32 with PSRAM)
// Arduino_G *output_display = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// Arduino_GFX *gfx = new Arduino_Canvas(240 /* width */, 320 /* height */, output_display);

// Indexed color Canvas, mask_level: 0-2, larger mask level mean less color variation but can have faster index mapping
// Arduino_G *output_display = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// Arduino_GFX *gfx = new Arduino_Canvas_Indexed(240 /* width */, 320 /* height */, output_display, 0 /* output_x */, 0 /* output_y */, MAXMASKLEVEL /* mask_level */);

// 3-bit color Canvas, R1G1B1, 8 colors
// Arduino_G *output_display = new Arduino_ILI9488_3bit(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */, false /* IPS */);
// Arduino_GFX *gfx = new Arduino_Canvas_3bit(480 /* width */, 320 /* height */, output_display, 0 /* output_x */, 0 /* output_y */);

// Mono color Canvas
// Arduino_G *output_display = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// Arduino_GFX *gfx = new Arduino_Canvas_Mono(240 /* width */, 320 /* height */, output_display, 0 /* output_x */, 0 /* output_y */);
/***************************************
 * End of Canvas (framebuffer)
 **************************************/

// GC9A01 IPS LCD 240x240
// Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// GC9A01 IPS LCD 360x360
// Arduino_GFX *g = new Arduino_GC9C01(bus, TFT_RST, 0 /* rotation */);
// #define CANVAS
// Arduino_Canvas *gfx = new Arduino_Canvas(360 /* width */, 360 /* height */, g);

// GC9106 IPS LCD 80x160
// Arduino_GFX *gfx = new Arduino_GC9106(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// GC9107 IPS LCD 128x128
// Arduino_GFX *gfx = new Arduino_GC9107(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8347C IPS LCD 240x320
// Arduino_GFX *gfx = new Arduino_HX8347C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8347D IPS LCD 240x320
// Arduino_GFX *gfx = new Arduino_HX8347D(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8352C IPS LCD 240x400
// Arduino_GFX *gfx = new Arduino_HX8352C(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8357A IPS LCD 320x480 (currently only portrait works, i.e. rotation 0 and 2)
// Arduino_GFX *gfx = new Arduino_HX8357A(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8357B IPS LCD 320x480
// Arduino_GFX *gfx = new Arduino_HX8357B(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// HX8369A LCD 480x800
// Arduino_GFX *gfx = new Arduino_HX8369A(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 480, 800, 0, 7, 0, 57);

// ILI9225 LCD 176x220
// Arduino_GFX *gfx = new Arduino_ILI9225(bus, TFT_RST);

// ILI9341 LCD 240x320 (default Display, comment below line if you are not using ILI9341)
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9342 LCD 320x240
// Arduino_GFX *gfx = new Arduino_ILI9342(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9481 parallel 8/16-bit LCD 320x480
// Arduino_GFX *gfx = new Arduino_ILI9481(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9481 SPI LCD 320x480
// Arduino_GFX *gfx = new Arduino_ILI9481_18bit(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9486 parallel 8/16-bit LCD 320x480
// Arduino_GFX *gfx = new Arduino_ILI9486(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9486 SPI LCD 320x480
// Arduino_GFX *gfx = new Arduino_ILI9486_18bit(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9488 parallel 8/16-bit LCD 320x480
// Arduino_GFX *gfx = new Arduino_ILI9488(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9488 SPI LCD 320x480
// Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// ILI9806 LCD 480x854
// Arduino_GFX *gfx = new Arduino_ILI9806(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

// JBT6K71 LCD 240x320
// Arduino_GFX *gfx = new Arduino_JBT6K71(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 240, 320, 0, 0, 16, 0);

// NT35310 LCD 320x480
// Arduino_GFX *gfx = new Arduino_NT35310(bus, TFT_RST, 0 /* rotation */);

// NT35510 LCD 480x800
// Arduino_GFX *gfx = new Arduino_NT35510(bus, TFT_RST, 0 /* rotation */);

// NT39125 LCD 240x376
// Arduino_GFX *gfx = new Arduino_NT39125(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 240, 376, 0, 0, 0, 56);

// NV3007 LCD 142x428
// Arduino_GFX *gfx = new Arduino_NV3007(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 142 /* width */, 428 /* height */, 12 /* col offset 1 */, 0 /* row offset 1 */, 14 /* col offset 2 */, 0 /* row offset 2 */);
// Alternate driver, with a different initialization sequence
// Arduino_GFX *gfx = new Arduino_NV3007_alternate(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 142 /* width */, 428 /* height */, 12 /* col offset 1 */, 0 /* row offset 1 */, 14 /* col offset 2 */, 0 /* row offset 2 */);

// NV3041A IPS LCD
// Arduino_GFX *gfx = new Arduino_NV3041A(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// OTM8009A LCD 480x800
// Arduino_GFX *gfx = new Arduino_OTM8009A(bus, TFT_RST, 0 /* rotation */);

// R61529 IPS LCD 320x480
// Arduino_GFX *gfx = new Arduino_R61529(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// SEPS525 OLED 160x128
// Arduino_GFX *gfx = new Arduino_SEPS525(bus, TFT_RST, 0 /* rotation */);

// SSD1283A OLED 130x130
// Arduino_GFX *gfx = new Arduino_SSD1283A(bus, TFT_RST, 0 /* rotation */);

// SSD1331 OLED 96x64
// Arduino_GFX *gfx = new Arduino_SSD1331(bus, TFT_RST, 0 /* rotation */);

// SSD1351 OLED 128x128
// Arduino_GFX *gfx = new Arduino_SSD1351(bus, TFT_RST, 0 /* rotation */);

// ST7735 LCD
// 0.42" 96x54
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 96 /* width */, 54 /* height */, 16 /* col offset 1 */, 106 /* row offset 1 */, 16 /* col offset 2 */, 0 /* row offset 2 */);
// 0.85" 128x128
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 128 /* width */, 128 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" REDTAB 128x160
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */);
// 1.8" BLACKTAB 128x160
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */, false /* BGR */);
// 1.8" GREENTAB A 128x160
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 1 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" GREENTAB B 128x160
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 2 /* col offset 1 */, 3 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.8" Wide angle LCD 128x160
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 160 /* height */, 0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */, false /* BGR */);
// 1.5" GREENTAB B 128x128
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 128 /* height */, 2 /* col offset 1 */, 3 /* row offset 1 */, 2 /* col offset 2 */, 1 /* row offset 2 */);
// 1.5" GREENTAB C 128x128
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 128 /* width */, 128 /* height */, 0 /* col offset 1 */, 32 /* row offset 1 */);
// 0.96" IPS LCD 80x160
// Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 80 /* width */, 160 /* height */, 26 /* col offset 1 */, 1 /* row offset 1 */, 26 /* col offset 2 */, 1 /* row offset 2 */);

// ST7789 LCD
// 2.4" LCD 240x320
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */);
// 2.4" IPS LCD 240x320
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */);
// 2.25" LCD 76x284
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, false /* IPS */, 76 /* width */, 284 /* height */, 82 /* col offset 1 */, 18 /* row offset 1 */, 82 /* col offset 2 */, 18 /* row offset 2 */);
// 1.9" IPS round corner LCD 170x320
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);
// 1.69" IPS round corner LCD 240x280
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 240 /* width */, 280 /* height */, 0 /* col offset 1 */, 20 /* row offset 1 */, 0 /* col offset 2 */, 20 /* row offset 2 */);
// 1.47" IPS round corner LCD 172x320
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 172 /* width */, 320 /* height */, 34 /* col offset 1 */, 0 /* row offset 1 */, 34 /* col offset 2 */, 0 /* row offset 2 */);
// 1.3"/1.5" square IPS LCD 240x240
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */, 0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 80 /* row offset 2 */);
// 1.14" IPS LCD 135x240
// Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */, 52 /* col offset 1 */, 40 /* row offset 1 */, 53 /* col offset 2 */, 40 /* row offset 2 */);

// ST7796 LCD
// 1.55" IPS LCD 320x360
// Arduino_GFX *gfx = new Arduino_ST7796(bus, TFT_RST, 3 /* rotation */, true /* IPS */, 320 /* width */, 360 /* height */, 0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 120 /* row offset 2 */);
// 4" LCD 320x480
// Arduino_GFX *gfx = new Arduino_ST7796(bus, TFT_RST, 0 /* rotation */);
// 4" IPS LCD 320x480
// Arduino_GFX *gfx = new Arduino_ST7796(bus, TFT_RST, 0 /* rotation */, true /* IPS */);

// WEA2012 LCD
// #define CANVAS
// Arduino_GFX *output_display = new Arduino_WEA2012(bus, TFT_RST);
// Arduino_GFX *gfx = new Arduino_Canvas(356 /* width */, 400 /* height */, output_display);
