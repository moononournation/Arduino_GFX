# Arduino_GFX
This library is developing aim for support various data bus interfaces and various displays.

As I know, it should be the first Arduino display library that can support ESP32 9-bit hardware SPI. It is very important to support the displays (e.g. HX8357B, ST7701, ...) that only have 9-bit SPI interface.

This library start rewrite from Adafruit_GFX, TFT_eSPI, Ucglib and more...

## Ease of use
#### Simple Declaration
(not require modify library header files)
```C
#include <SPI.h>
#include <Arduino_ESP32SPI.h>
#include <Arduino_GFX.h>
#include <Arduino_ILI9341.h>
Arduino_ESP32SPI *bus = new Arduino_ESP32SPI(16 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, -1 /* MISO */);
Arduino_ILI9341 *tft = new Arduino_ILI9341(bus, 17 /* RST */);
```

#### And Simple Usage
```
tft->begin();
tft->fillScreen(BLACK);
tft->setCursor(10, 10);
tft->setTextColor(RED);
tft->println("Hello World!");
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

## Tobe Support data bus (Donation can make it happen)
- 8 bit parallel interface
- 16 bit parallel interface

## Currently Supported Display
- GC9A01 round display 240x240 [[test video](https://youtu.be/kJrAFm20-zg)]
- HX8347C 240x320 [[test video](https://youtu.be/25ymuV51YQM)]
- HX8352C 240x400 [[test video](https://youtu.be/m2xWYbS3t7s)]
- HX8357B 320x480 [[test video](https://youtu.be/pB6_LOCiUqg)]
- ILI9225 176x220 [[test video](https://youtu.be/jm2UrCG27F4)]
- ILI9341 240x320 [[test video](https://youtu.be/NtlEEL7MkQY)]
- ILI9341 M5Stack 320x240 [[test video](https://youtu.be/UoPpIjVSO5Q)]
- ILI9481 320x480 (18 bit color) [[test video](https://youtu.be/YxjuuCFhlqM)]
- ILI9486 320x480 (18 bit color) [[test video](https://youtu.be/pZ6izDqmVds)]
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
- ST7789 240x320 [[test video](https://youtu.be/ZEvc1LkuVuQ)]
- ST7796 320x480 [[test video](https://youtu.be/hUL-RuG4MAQ)]
- Canvas (framebuffer)
- Canvas_Indexed (for saving memory space)

## Tobe Support Display (Donation can make it happen)
- LG4573 480x800
- ST7701 480x800
- Mono display supported by co-operate with Canvas
- Multi-color e-ink display supported by co-operate with Canvas
- ILI9486 320x480 (3 bit color) supported by co-operate with Canvas

## Known issue
- ESP32SPI data bus class is a little bit faster than HWSPI, but not all chip supported, e.g. SSD1283A

## Used source code
- http://elm-chan.org/fsw/tjpgd/00index.html
- https://github.com/adafruit/Adafruit-GFX-Library.git
- https://github.com/adafruit/Adafruit_ILI9341.git
- https://github.com/adafruit/Adafruit-SSD1351-library.git
- https://github.com/ananevilya/Arduino-ST7789-Library.git
- https://github.com/BasementCat/arduino-tft-gif
- https://github.com/espressif/arduino-esp32.git
- https://github.com/gitcnd/LCDWIKI_SPI.git
- https://github.com/lovyan03/LovyanGFX.git
- https://github.com/lovyan03/M5Stack_JpgLoopAnime
