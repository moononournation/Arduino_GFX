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
- MCU: ESP32D@240MHz
- PSRAM: disable
- Display: ILI9341
- Interface: SPI@40MHz
- Test time: 2019 Oct 1

| Benchmark          | Adafruit_GFX | *Arduino_GFX* | TFT_eSPI    |
| ------------------ | ------------ | ------------- | ----------- |
| Screen fill        | 39,094       | ***32,020***  | 33,355      |
| Text               | 96,432       | ***21,294***  | 24,015      |
| Pixels             | 1,353,397    | *960,075*     | **768,029** |
| Lines              | 1,061,786    | *500,028*     | **307,433** |
| Horiz/Vert Lines   | 17,604       | *14,594*      | **14,588**  |
| Rectangles-filled  | 405,985      | ***332,518*** | 346,379     |
| Rectangles         | 11,647       | *9,717*       | **9,247**   |
| Circles-filled     | 76,610       | *65,827*      | **62,179**  |
| Circles            | 118,042      | *59,825*      | **46,916**  |
| Triangles-filled   | 150,644      | *126,312*     | **117,575** |
| Triangles          | 58,791       | *28,392*      | **18,706**  |
| Rounded rects-fill | 407,911      | ***335,466*** | 376,752     |
| Rounded rects      | 42,677       | ***23,859***  | 24,189      |


## Currently Supported data bus
- 8-bit and 9-bit hardware SPI (ESP32SPI)
- 8-bit hardware SPI (HWSPI)
- 8-bit and 9-bit software SPI (SWSPI)

## Tobe Support data bus
- 8 bit parallel interface

## Currently Supported Display
- HX8352C 240x400 [[test video](https://youtu.be/m2xWYbS3t7s)]
- HX8357B 320x480 [[test video](https://youtu.be/pB6_LOCiUqg)]
- ILI9225 176x220 [[test video](https://youtu.be/jm2UrCG27F4)]
- ILI9341 240x320 [[test video](https://youtu.be/NtlEEL7MkQY)]
- ILI9341 M5Stack 320x240 [[test video](https://youtu.be/UoPpIjVSO5Q)]
- ILI9486 320x480 (18 bit color) [[test video](https://youtu.be/pZ6izDqmVds)]
- SEPS525 160x128 [[test video](https://youtu.be/tlmvFBHYv-k)]
- SSD1331 96x64 [[test video](https://youtu.be/v20b1A_KDcQ)]
- SSD1351 128x128 [[test video](https://youtu.be/5TIM-qMVBNQ)]
- SSD1351 128x96
- ST7735 128x160 (various tabs) [[test video](https://youtu.be/eRBSSD_N9II)]
- ST7735 128x128 (various tabs) [[test video](https://youtu.be/6rueSV2Ee6c)]
- ST7735 80x160 [[test video](https://youtu.be/qESHDuYo_Mk)]
- ST7789 240x135
- ST7789 240x240 [[test video](https://youtu.be/Z27zYg5uAsk)]
- ST7789 TTGO T-Watch 240x240 [[test video](https://youtu.be/9AqsXMB8Qbk)]
- ST7789 240x320 [[test video](https://youtu.be/ZEvc1LkuVuQ)]

## Tobe Support Display
- LG4573 480x800
- ILI9481 320x480
- ST7701 480x800
- Canvas (framebuffer)
- Mono display supported by co-operate with Canvas
- Multi-color e-ink display supported by co-operate with Canvas