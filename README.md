# Arduino_GFX

Arduino_GFX is a Arduino graphics library supporting various displays with various data bus interfaces.

This library start rewrite from Adafruit_GFX, TFT_eSPI, Ucglib and more...

## Various dat bus interfaces

Most tiny display in hobbiest electronics world support 8-bit SPI, but some require 9-bit SPI. As I know, it should be the first Arduino display library that can support ESP32 9-bit hardware SPI. It is very important to support the displays (e.g. HX8357B, ST7701, ...) that require 9-bit SPI interface.

Larger display most likely not support standalone SPI since it is not fast enough to refresh the full screen details. Most of them can use 8-bit/16-bit Parallel interface.

## Ease of use
#### Simple Declaration
(not require touch the header files in libraries folder)
```C
#include <Arduino_GFX_Library.h>
Arduino_DataBus *bus = new Arduino_HWSPI(16 /* DC */, 5 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 17 /* RST */);
```

#### And Simple Usage
```
gfx->begin();
gfx->fillScreen(BLACK);
gfx->setCursor(10, 10);
gfx->setTextColor(RED);
gfx->println("Hello World!");
```


## Performance
This library is not putting speed at the first priority, but still paid much effort to make the display look smooth. Below are some figures compare with other 2 Arduino common display libraries.
- Arduino IDE: 1.8.10
- MCU: ESP32-PICO-D4
- PSRAM: disable
- Display: ILI9341
- Interface: SPI@40MHz
- Test time: 2019 Oct 13

| Benchmark          | Adafruit_GFX  | *Arduino_GFX* | TFT_eSPI      |
| ------------------ | ------------- | ------------- | ------------- |
| Screen fill        | 39,055        | ***32,229***  | 33,355        |
| Text               | 96,432        | ***18,717***  | 24,010        |
| Pixels             | 1,353,319     | *919,219*     | **768,022**   |
| Lines              | 1,061,808     | *455,992*     | **307,429**   |
| Horiz/Vert Lines   | 17,614        | ***14,277***  | 14,587        |
| Rectangles-filled  | 405,880       | ***334,974*** | 346,317       |
| Rectangles         | 11,656        | *9,374*       | **9,251**     |
| Circles-filled     | 76,619        | ***55,173***  | 62,182        |
| Circles            | 118,051       | *52,315*      | **46,909**    |
| Triangles-filled   | 150,999       | *120,362*     | **117,591**   |
| Triangles          | 58,795        | *26,143*      | **18,704**    |
| Rounded rects-fill | 407,755       | ***335,537*** | 376,764       |
| Rounded rects      | 42,668        | ***21,100***  | 24,201        |

| Foot print         | Adafruit_GFX  | *Arduino_GFX* | TFT_eSPI      |
| ------------------ | ------------- | ------------- | ------------- |
| Flash              | 232,572       | 245,544       | ***231,136*** |
| Estimate memory    | 15,512        | 15,616        | ***15,432***  |


## Currently Supported data bus
- 8-bit and 9-bit hardware SPI (ESP32SPI)
- 8-bit hardware SPI (HWSPI)
- 8-bit and 9-bit software SPI (SWSPI)
- 8-bit parallel interface (ESP32PAR8)
- 16-bit parallel interface (ESP32PAR16)

## Tobe Support data bus (Donation can make it happen)
- ESP32 I2S 8-bit/16-bit parallel interface
- FastLED

## Currently Supported Dev Device
- M5Stack Core Family
- Odroid Go
- TTGO T-Watch
- Wio Terminal

## Currently Supported Display
- GC9A01 round display 240x240 [[test video](https://youtu.be/kJrAFm20-zg)]
- HX8347C 240x320 [[test video](https://youtu.be/25ymuV51YQM)]
- HX8347D 240x320 [[test video](https://youtu.be/sv6LGkLRZjI)]
- HX8352C 240x400 [[test video](https://youtu.be/m2xWYbS3t7s)]
- HX8357B (9-bit SPI) 320x480 [[test video](https://youtu.be/pB6_LOCiUqg)]
- ILI9225 176x220 [[test video](https://youtu.be/jm2UrCG27F4)]
- ILI9341 240x320 [[test video](https://youtu.be/NtlEEL7MkQY)]
- ILI9341 M5Stack 320x240 [[test video](https://youtu.be/UoPpIjVSO5Q)]
- ILI9481 320x480 (18 bit color) [[test video](https://youtu.be/YxjuuCFhlqM)]
- ILI9486 320x480 (18 bit color) [[test video](https://youtu.be/pZ6izDqmVds)]
- ILI9488 320x480 (18 bit color) [[test video](https://youtu.be/NkE-LhtLHBQ)]
- JBT6K71 (8-bit Parallel) 240x320 [[test video](https://youtu.be/qid3F4Gb0mM)]
- R61529 (8-bit/16-bit Parallel) 320x480 [[test video](https://youtu.be/s93gxjbIAT8)]
- SEPS525 160x128 [[test video](https://youtu.be/tlmvFBHYv-k)]
- SSD1283A 130x130 [[test video](https://youtu.be/OrIchaRikiQ)]
- SSD1331 96x64 [[test video](https://youtu.be/v20b1A_KDcQ)]
- SSD1351 128x128 [[test video](https://youtu.be/5TIM-qMVBNQ)]
- SSD1351 128x96
- ST7735 128x160 (various tabs) [[test video](https://youtu.be/eRBSSD_N9II)]
- ST7735 128x128 (various tabs) [[test video](https://youtu.be/6rueSV2Ee6c)]
- ST7735 80x160 [[test video](https://youtu.be/qESHDuYo_Mk)]
- ST7789 TTGO T-Display 135x240 [[test video](https://youtu.be/Zk81_T8c20E)]
- ST7789 240x240 [[test video](https://youtu.be/Z27zYg5uAsk)]
- ST7789 TTGO T-Watch 240x240 [[test video](https://youtu.be/9AqsXMB8Qbk)]
- ST7789 round corner display 240x280 [[test video](https://youtu.be/KzDC02wg8z0)]
- ST7789 240x320 [[test video](https://youtu.be/ZEvc1LkuVuQ)]
- ST7796 320x480 [[test video](https://youtu.be/hUL-RuG4MAQ)]
- Canvas (framebuffer)
- Canvas_Indexed (for saving memory space)

## Tobe Support Display (Donation can make it happen)
- HX8357A 240x320 (first trial failed)
- LG4573 480x800 (first trial failed)
- ILI9806 480x800 (first trial failed)
- ST7701 480x800
- FastLED Martix supported by co-operate with Canvas
- Mono display supported by co-operate with Canvas
- Multi-color e-ink display supported by co-operate with Canvas
- ILI9486 320x480 (3 bit color) supported by co-operate with Canvas

## Used source code
- http://elm-chan.org/fsw/tjpgd/00index.html
- https://github.com/adafruit/Adafruit-GFX-Library.git
- https://github.com/adafruit/Adafruit_ILI9341.git
- https://github.com/adafruit/Adafruit-SSD1351-library.git
- https://github.com/ananevilya/Arduino-ST7789-Library.git
- https://github.com/BasementCat/arduino-tft-gif
- https://github.com/Bodmer/TFT_eSPI
- https://github.com/daumemo/IPS_LCD_R61529_FT6236_Arduino_eSPI_Test
- https://github.com/espressif/arduino-esp32.git
- https://github.com/gitcnd/LCDWIKI_SPI.git
- https://github.com/lovyan03/LovyanGFX.git
- https://github.com/lovyan03/M5Stack_JpgLoopAnime
