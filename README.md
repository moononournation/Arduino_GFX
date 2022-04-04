# Arduino_GFX

Arduino_GFX is a Arduino graphics library supporting various displays with various data bus interfaces.

This library start rewrite from Adafruit_GFX, LovyanGFX, TFT_eSPI, Ucglib, and more...

![GitHub release (latest by date)](https://img.shields.io/github/v/release/moononournation/Arduino_GFX)
![GitHub Release Date](https://img.shields.io/github/release-date/moononournation/Arduino_GFX)
![GitHub commits since latest release (by date)](https://img.shields.io/github/commits-since/moononournation/Arduino_GFX/latest)
![GitHub last commit](https://img.shields.io/github/last-commit/moononournation/Arduino_GFX)

![GitHub Sponsors](https://img.shields.io/github/sponsors/moononournation)
![Twitter Follow](https://img.shields.io/twitter/follow/moononournation)

## Various data bus interfaces

Arduino_GFX utilizes Arduino Built-in SPI class to support 8-bit SPI for most platforms.

Most tiny displays in hobbyist electronics world support 8-bit SPI, but some require 9-bit SPI. Arduino_GFX should be the first Arduino display library that can use ESP32 SPI to support 9-bit hardware SPI. It is important to support the displays that require 9-bit SPI interface. (e.g. HX8357B, ...)

Larger displays most likely do not support standalone SPI since it is not fast enough to refresh the full screen details. Most of them support 8-bit/16-bit Parallel interface.

Some larger display require 3-bit SPI + RGB combo interface, i.e. require more than 3 + 24 pins. Most dev board do not have enough GPIO to support this. Arduino_GFX will eventually support this combo interface but will be in very low priority. A huge monthly sponsor may make it happen :P

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
This library is not putting speed at the first priority, but still paid much effort to make the display look smooth.

### Figures
Below are some figures compare with other 3 Arduino common display libraries.
- Arduino IDE: 1.8.15
- arduino-esp32: 1.0.6
- Dev Board: TTGO T8 v1.8
- PSRAM: disable
- Display: ILI9341
- Interface: SPI
- SPI Frequency: 40MHz
- Test time: 2021 Jun 16

| Benchmark          |  Adafruit_GFX | *Arduino_GFX* |    Lovyan_GFX |      TFT_eSPI |
| ------------------ | ------------- | ------------- | ------------- | ------------- |
| Screen fill        |       195,782 |     *160,094* |       154,341 |       155,938 |
| Text               |        97,662 |      *18,960* |        22,473 |        21,752 |
| Pixels             |     1,365,211 |     *903,549* |       867,702 |       775,781 |
| Lines              |     1,062,311 |     *412,026* |       269,060 |       264,950 |
| Horiz/Vert Lines   |        17,637 |      *14,197* |        13,692 |        13,833 |
| Rectangles-filled  |       406,817 |     *332,696* |       320,761 |       323,908 |
| Rectangles         |        11,641 |       *9,254* |         8,545 |         8,714 |
| Triangles-filled   |       150,941 |     *118,010* |       105,661 |       109,675 |
| Triangles          |        58,843 |      *23,570* |        15,884 |        16,277 |
| Circles-filled     |        76,739 |      *52,170* |        42,787 |        45,827 |
| Circles            |       118,125 |      *40,955* |        25,959 |        25,269 |
| Arcs-filled        |      N/A      |      *33,381* |        21,546 |      N/A      |
| Arcs               |      N/A      |      *66,054* |        47,901 |      N/A      |
| Rounded rects-fill |       408,534 |     *338,136* |       318,882 |       323,189 |
| Rounded rects      |        43,185 |      *21,562* |        13,089 |        15,371 |

### Why Run Fast?
- No read operation. Since not all display provide read back graphic memories API, Arduino_GFX skip all read operations. It can reduce the library size footprint and sometimes reduce the operation time.
- Tailor-made data bus classes. Arduino_GFX decouple data bus operation from display driver, it is more easy to write individual data bus class for each platform.

## Currently Supported data bus [[Wiki](https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class)]
- 8-bit and 9-bit hardware SPI (ESP32SPI)
- 8-bit hardware SPI (HWSPI, ESP8266SPI, mbedSPI, NRFXSPI, RPiPicoSPI)
- 8-bit and 9-bit software SPI (SWSPI)
- 8-bit parallel interface (AVRPAR8, ESP32PAR8, ESP32S2PAR8, RPiPicoPAR8, RTLPAR8)
- 16-bit parallel interface (ESP32PAR16, ESP32S2PAR8, RPiPicoPAR16)

## Tobe Support data bus (Sponsors can make it happen)
- ESP32 I2S 8-bit/16-bit parallel interface
- Arduino ATMega2560 dual 8-bit Port form 16-bit parallel interface
- FastLED

## Currently Supported Dev Board
- Ameba RTL8722DM Board (AMB 21)
- Ameba RTL8722DM MINI Board (AMB 23)
- Arduino Nano
- Arduino Nano BLE 33
- Arduino Pro Micro
- ESP8266 Series
- ESP32 Series
- ESP32-C3 Series
- ESP32-S2 Series
- ESP32-S3 Series
- Raspberry Pi Pico
- rtlduino BW16 (by Ai-Thinker)
- Sony Spresense
- WeAct BlackPill V2.0 (BlackPill F411CE)

## Tobe Support Dev Board (Sponsors can make it happen)
- Arduino ATMega2560
- ESP32-S3 Series

## Currently Supported Dev Device [[Wiki](https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration)]
- ESP32 LCDKIT
- ESP32-S3-EYE
- ESPboy [[demo video](https://youtu.be/Cx82XWrc8-0)]
- Makerfabs ESP32 3.5" TFT Touch with Camera
- TTGO T-DISPLAY
- wireless-tag WT-32-SC01
- Wio Terminal
- M5Stack Core Family
- Odroid Go
- TTGO T-Watch

## Currently Supported Display [[Wiki](https://github.com/moononournation/Arduino_GFX/wiki/Display-Class)]
- GC9A01 round display 240x240 [[demo video](https://youtu.be/kJrAFm20-zg)]
- GC9106 80x160 [[demo video](https://youtu.be/RToGeeb1jxQ)]
- HX8347C 240x320 [[demo video](https://youtu.be/25ymuV51YQM)]
- HX8347D 240x320 [[demo video](https://youtu.be/sv6LGkLRZjI)]
- HX8352C 240x400 [[demo video](https://youtu.be/m2xWYbS3t7s)]
- HX8357A 320x480 [[demo video](https://youtu.be/wJkLO_xCTXA)] (currently only portrait works, i.e. rotation 0 and 2)
- HX8357B (9-bit SPI) 320x480 [[demo video](https://youtu.be/pB6_LOCiUqg)]
- ILI9225 176x220 [[demo video](https://youtu.be/jm2UrCG27F4)]
- ILI9341 240x320 [[demo video](https://youtu.be/NtlEEL7MkQY)]
- ILI9341 (8-bit Parallel) 240x320 [[demo video](https://youtu.be/xuVx0dzQ7nM)]
- ILI9342 320x240 [[demo video](https://youtu.be/UoPpIjVSO5Q)]
- ILI9481 320x480 (18 bit color) [[demo video](https://youtu.be/YxjuuCFhlqM)]
- ILI9486 320x480 (18 bit color) [[demo video](https://youtu.be/pZ6izDqmVds)]
- ILI9488 320x480 (3 bit color with canvas) [[demo video](https://youtu.be/r7be0WbIBYk)]
- ILI9488 320x480 (18 bit color) [[demo video](https://youtu.be/NkE-LhtLHBQ)]
- ILI9806 (8-bit/16-bit Parallel) 480x854 [[demo video](https://youtu.be/mYv-wdtWr8s)]
- JBT6K71 (8-bit Parallel) 240x320 [[demo video](https://youtu.be/qid3F4Gb0mM)]
- NT35310 320x480 [[demo video](https://youtu.be/bvIz5CoYPNk)]
- NT35510 (8-bit/16-bit Parallel) 480x800 [[demo video](https://youtu.be/C_1ASzUN3bg)]
- NT39125 (8-bit/16-bit Parallel) 240x376 [[demo video](https://youtu.be/JGMrM18JAFA)]
- R61529 (8-bit/16-bit Parallel) 320x480 [[demo video](https://youtu.be/s93gxjbIAT8)]
- SEPS525 160x128 [[demo video](https://youtu.be/tlmvFBHYv-k)]
- SSD1283A 130x130 [[demo video](https://youtu.be/OrIchaRikiQ)]
- SSD1331 96x64 [[demo video](https://youtu.be/v20b1A_KDcQ)]
- SSD1351 128x128 [[demo video](https://youtu.be/5TIM-qMVBNQ)]
- SSD1351 128x96
- ST7735 128x160 (various tabs) [[demo video](https://youtu.be/eRBSSD_N9II)]
- ST7735 128x128 (various tabs) [[demo video](https://youtu.be/6rueSV2Ee6c)]
- ST7735 80x160 [[demo video](https://youtu.be/qESHDuYo_Mk)]
- ST7789 TTGO T-Display 135x240 [[demo video](https://youtu.be/Zk81_T8c20E)]
- ST7789 240x240 [[demo video](https://youtu.be/Z27zYg5uAsk)]
- ST7789 TTGO T-Watch 240x240 [[demo video](https://youtu.be/9AqsXMB8Qbk)]
- ST7789 round corner display 240x280 [[demo video](https://youtu.be/KzDC02wg8z0)]
- ST7789 240x320 [[demo video](https://youtu.be/ZEvc1LkuVuQ)]
- ST7796 320x480 [[demo video](https://youtu.be/hUL-RuG4MAQ)]

## Tobe Support Display (Sponsors can make it happen)
- FastLED Martix supported by co-operate with Canvas
- Mono display supported by co-operate with Canvas
- Multi-color e-ink display supported by co-operate with Canvas

## Canvas (framebuffer) [[Wiki](https://github.com/moononournation/Arduino_GFX/wiki/Canvas-Class)]
- Canvas (16-bit pixel)
- Canvas_Indexed (half memory space)
- Canvas_3bit (1/4 memory space framebuffer)
- Canvas_Mono (1/16 memory space framebuffer)

## Feature wishlist (Sponsors can make it happen)
- Print UTF8 Characters
- Print color Emoji Characters
- Load bitmap font files from flash / SD
- Fill Gradient

## Using source code come from:
- http://elm-chan.org/fsw/tjpgd/00index.html
- https://github.com/adafruit/Adafruit-GFX-Library.git
- https://github.com/adafruit/Adafruit_ILI9341.git
- https://github.com/adafruit/Adafruit-SSD1351-library.git
- https://github.com/ananevilya/Arduino-ST7789-Library.git
- https://github.com/BasementCat/arduino-tft-gif.git
- https://github.com/Bodmer/TFT_eSPI.git
- https://github.com/daumemo/IPS_LCD_R61529_FT6236_Arduino_eSPI_Test.git
- https://github.com/espressif/arduino-esp32.git
- https://github.com/gitcnd/LCDWIKI_SPI.git
- https://github.com/hi631/LCD_NT35510-MRB3971.git
- https://github.com/lcdwiki/LCDWIKI_SPI.git
- https://github.com/lovyan03/LovyanGFX.git
- https://github.com/lovyan03/M5Stack_JpgLoopAnime.git
- https://github.com/nopnop2002/esp-idf-parallel-tft.git
- https://github.com/olikraus/u8g2.git