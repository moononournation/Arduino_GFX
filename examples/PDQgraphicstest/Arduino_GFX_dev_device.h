// #define AD35_S3
// #define BLOCKCODELAB_ARCADE_LITE
// #define DLC35010R // or called "Elecrow ESP Terminal with 3.5inch Parallel RGB Capacitive Touch Display (ILI9488)"
// #define DRAGON_RADAR
// #define ESP32_1732S019
// #define ESP32_2424012
// #define ESP32_2432S028
// #define ESP32_3248S035
// #define ESP32_4827A043 /* requires manual set bus speed to 8 mHz: gfx->begin(8000000) */
// #define ESP32_4827A043_QSPI
// #define ESP32_4827S043
// #define ESP32_4848S040_86BOX_GUITION
// #define ESP32_8048S043
// #define ESP32_8048S070
// #define ESP32_LCDKIT_SPI
// #define ESP32_LCDKIT_PAR8A
// #define ESP32_LCDKIT_PAR8B
// #define ESP32_LCDKIT_PAR16
// #define ESP32_S3_313LCD
// #define ESP32_S3_BOX_3
// #define ESP32_S3_EYE
// #define ESP32_S3_RGB
// #define ESP32_S3_RPI_DPI
// #define ESP32S3_2_1_TP
// #define GS_T3E // Golden Security GS-T3E
// #define IBUBLY
// #define JC1060P470
// #define JC3248W535
// #define JC3636W518
// #define JC8012P4A1
// #define LILYGO_T_DECK
// #define LILYGO_T_DECK_PLUS
// #define LILYGO_T_DISPLAY
// #define LILYGO_T_DISPLAY_S3
// #define LILYGO_T_Display_S3_AMOLED
// #define LILYGO_T_Display_S3_AMOLED_1_64
// #define LILYGO_T_Display_S3_LONG
// #define LILYGO_T_DISPLAY_S3_PRO
// #define LILYGO_T_QT_PRO
// #define LILYGO_T_RGB
// #define LILYGO_T_TRACK
// #define LILYGO_T_WATCH_2021
// #define LILYGO_T4_S3
// #define MAKERFABS_TFT_TOUCH_3_5
// #define MAKERFABS_ESP32_S3_TFT_4_0
// #define MAKERFABS_ESP32_S3_TFT_4_3_v1_3
// #define XIAO_SAMD21_ROUND_DISPLAY
// #define XIAO_ESP32C3_ROUND_DISPLAY
// #define XIAO_ESP32S3_ROUND_DISPLAY
// #define WAVESHARE_ESP32_C6_LCD_1_47
// #define WAVESHARE_ESP32_C6_LCD_1_9
// #define WAVESHARE_ESP32_S3_LCD_1_3
// #define WAVESHARE_ESP32_S3_LCD_1_3_PRISM
// #define WAVESHARE_ESP32_S3_LCD_1_46
// #define WAVESHARE_ESP32_S3_LCD_1_47
// #define WAVESHARE_ESP32_S3_LCD_2_8
// #define WAVESHARE_ESP32_S3_TOUCH_AMOLED_1_64
// #define WAVESHARE_ESP32_S3_TOUCH_AMOLED_1_8
// #define WAVESHARE_ESP32_S3_TOUCH_AMOLED_2_41
// #define WAVESHARE_ESP32_S3_TOUCH_LCD_3_49
// #define WAVESHARE_RP2040_LCD_0_96
// #define WAVESHARE_RP2040_LCD_1_28 // compatible RP2350-LCD-1.28
// #define WAVESHARE_RP2350_LCD_1_47
// #define WT32_SC01
// #define WZ8048C050 // or called "Elecrow Wizee-ESP32"
// #define ZX2D10GE10R_V4848
// #define ZX3D50CE02S // or called "WT32-SC01 PLUS"
// #define ZX3D95CE01S_AR
// #define ZX3D95CE01S_TR
// #define ZX7D00CE01S // or called "QM Smart Panlee 7.0 inch serial screen"

#if defined(AD35_S3)
#define GFX_DEV_DEVICE AD35_S3
#include <Wire.h>
#include <Adafruit_AW9523.h>
Adafruit_AW9523 aw;
#define DEV_DEVICE_INIT()                         \
    {                                             \
        Wire.begin(6 /* SDA */, 5 /* SCL */);     \
        aw.begin(0x59);                           \
        aw.pinMode(8, OUTPUT);     /* LCD_LEDK */ \
        aw.pinMode(9, OUTPUT);     /* LCD_LEDK */ \
        aw.pinMode(10, OUTPUT);    /* LCD_LEDK */ \
        aw.pinMode(11, OUTPUT);    /* LCD_LEDK */ \
        aw.pinMode(14, OUTPUT);    /* LCD_RST */  \
        aw.digitalWrite(8, LOW);   /* LCD_LEDK */ \
        aw.digitalWrite(9, LOW);   /* LCD_LEDK */ \
        aw.digitalWrite(10, LOW);  /* LCD_LEDK */ \
        aw.digitalWrite(11, LOW);  /* LCD_LEDK */ \
        aw.digitalWrite(14, HIGH); /* LCD_RST */  \
    }
Arduino_DataBus *bus = new Arduino_ESP32LCD8(
    45 /* DC */, GFX_NOT_DEFINED /* CS */, 10 /* WR */, GFX_NOT_DEFINED /* RD */,
    9 /* D0 */, 4 /* D1 */, 3 /* D2 */, 8 /* D3 */, 18 /* D4 */, 17 /* D5 */, 16 /* D6 */, 15 /* D7 */);
Arduino_GFX *gfx = new Arduino_ST7796(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(BLOCKCODELAB_ARCADE_LITE)
#define GFX_DEV_DEVICE BLOCKCODELAB_ARCADE_LITE
#define GFX_BL 21
Arduino_DataBus *bus = new Arduino_ESP32SPI(45 /* DC */, 10 /* CS */, 12 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, HSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 46 /* RST */, 3 /* rotation */, false /* IPS */);

#elif defined(DLC35010R)
#define GFX_DEV_DEVICE DLC35010R
#define GFX_BL 46
Arduino_DataBus *bus = new Arduino_ESP32PAR16(
    45 /* DC */, GFX_NOT_DEFINED /* CS */, 18 /* WR */, 48 /* RD */,
    47 /* D0 */, 21 /* D1 */, 14 /* D2 */, 13 /* D3 */, 12 /* D4 */, 11 /* D5 */, 10 /* D6 */, 9 /* D7 */,
    3 /* D8 */, 8 /* D9 */, 16 /* D10 */, 15 /* D11 */, 7 /* D12 */, 6 /* D13 */, 5 /* D14 */, 4 /* D15 */);
Arduino_GFX *gfx = new Arduino_ILI9488(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, false /* IPS */);

#elif defined(DRAGON_RADAR)
#define GFX_DEV_DEVICE DRAGON_RADAR
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(GFX_NOT_DEFINED /* DC */, 39 /* CS */, 48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    11 /* R0 */, 12 /* R1 */, 13 /* R2 */, 45 /* R3 */, 0 /* R4 */,
    8 /* G0 */, 19 /* G1 */, 20 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    4 /* B0 */, 5 /* B1 */, 6 /* B2 */, 7 /* B3 */, 15 /* B4 */,
    1 /* hsync_polarity */, 50 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 30 /* hsync_back_porch */,
    1 /* vsync_polarity */, 20 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 30 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type6_init_operations, sizeof(st7701_type6_init_operations));

#elif defined(ESP32_1732S019)
#define GFX_DEV_DEVICE ESP32_1732S019
#define GFX_BL 14
Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 1 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */,
    35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(ESP32_2424012)
#define GFX_DEV_DEVICE ESP32_2424012
#define GFX_BL 8
Arduino_DataBus *bus = new Arduino_ESP32SPI(2 /* DC */, 10 /* CS */, 6 /* SCK */, 7 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(ESP32_2432S028)
#define GFX_DEV_DEVICE ESP32_2432S028
#define GFX_BL 21
Arduino_DataBus *bus = new Arduino_ESP32SPI(2 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, 12 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */);

#elif defined(ESP32_3248S035)
#define GFX_DEV_DEVICE ESP32_3248S035
#define GFX_BL 27
Arduino_DataBus *bus = new Arduino_ESP32SPI(2 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, 12 /* MISO */, VSPI /* spi_num */);
Arduino_GFX *gfx = new Arduino_ST7796(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */);

#elif defined(ESP32_4827A043)
#define GFX_DEV_DEVICE ESP32_4827A043
#define GFX_BL 2
Arduino_DataBus *bus = new Arduino_ESP32LCD16(
    48 /* DC */, 45 /* CS */, 47 /* WR */, 21 /* RD */,
    5 /* D0 */, 6 /* D1 */, 7 /* D2 */, 15 /* D3 */, 16 /* D4 */, 4 /* D5 */, 8 /* D6 */, 3 /* D7 */,
    46 /* D8 */, 9 /* D9 */, 1 /* D10 */, 42 /* D11 */, 39 /* D12 */, 41 /* D13 */, 40 /* D14 */, 14 /* D15 */);
Arduino_GFX *gfx = new Arduino_NV3041A(bus, 17 /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(ESP32_4827A043_QSPI)
#define GFX_DEV_DEVICE ESP32_4827A043_QSPI
#define GFX_BL 1
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    45 /* CS */, 47 /* SCK */, 21 /* D0 */, 48 /* D1 */, 40 /* D2 */, 39 /* D3 */);
Arduino_GFX *gfx = new Arduino_NV3041A(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(ESP32_4827S043)
#define GFX_DEV_DEVICE ESP32_4827S043
#define GFX_BL 2
#define RGB_PANEL
// option 1:
// Uncomment for ILI6485 LCD 480x272
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
    0 /* hsync_polarity */, 1 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
    0 /* vsync_polarity */, 3 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 10000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 272 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);
// option 2:
// Uncomment for ST7262 IPS LCD 800x480
// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
//     45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
//     5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
//     8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
//     0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
//     0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
//     1 /* pclk_active_neg */, 16000000 /* prefer_speed */, false /* useBigEndian */,
//     0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);

// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);
// option 3:
// Uncomment for RPi DPI 1024x600
// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
//     45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
//     5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
//     8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
//     0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
//     0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
//     1 /* pclk_active_neg */, 9000000 /* prefer_speed */, false /* useBigEndian */,
//     0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     1024 /* width */, 600 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

/* 4.0" 86BOX from GUITION ESP32-4848S040 display */
#elif defined(ESP32_4848S040_86BOX_GUITION)
#define GFX_DEV_DEVICE ESP32_4848S040_86BOX_GUITION
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    11 /* R0 */, 12 /* R1 */, 13 /* R2 */, 14 /* R3 */, 0 /* R4 */,
    8 /* G0 */, 20 /* G1 */, 3 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    4 /* B0 */, 5 /* B1 */, 6 /* B2 */, 7 /* B3 */, 15 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 1 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type9_init_operations, sizeof(st7701_type9_init_operations));

#elif defined(ESP32_8048S043)
#define GFX_DEV_DEVICE ESP32_8048S043
#define GFX_BL 2
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
    0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
    0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#elif defined(ESP32_8048S070)
#define GFX_DEV_DEVICE ESP32_8048S070
#define GFX_BL 2
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */,
    0 /* hsync_polarity */, 180 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
    0 /* vsync_polarity */, 12 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#elif defined(ESP32_LCDKIT_SPI)
#define GFX_DEV_DEVICE ESP32_LCDKIT_SPI
#define GFX_BL 23
Arduino_DataBus *bus = new Arduino_ESP32SPI(19 /* DC */, 5 /* CS */, 22 /* SCK */, 21 /* MOSI */, 27 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 18 /* RST */, 1 /* rotation */);

#elif defined(ESP32_LCDKIT_PAR8A)
#define GFX_DEV_DEVICE ESP32_LCDKIT_PAR8A
Arduino_DataBus *bus = new Arduino_ESP32PAR8(
    5 /* DC */, GFX_NOT_DEFINED /* CS */, 18 /* WR */, GFX_NOT_DEFINED /* RD */,
    19 /* D0 */, 21 /* D1 */, 0 /* D2 */, 22 /* D3 */, 23 /* D4 */, 33 /* D5 */, 32 /* D6 */, 27 /* D7 */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */);

#elif defined(ESP32_LCDKIT_PAR8B)
#define GFX_DEV_DEVICE ESP32_LCDKIT_PAR8B
Arduino_DataBus *bus = new Arduino_ESP32PAR8(
    5 /* DC */, GFX_NOT_DEFINED /* CS */, 18 /* WR */, GFX_NOT_DEFINED /* RD */,
    25 /* D0 */, 26 /* D1 */, 12 /* D2 */, 13 /* D3 */, 14 /* D4 */, 15 /* D5 */, 2 /* D6 */, 4 /* D7 */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */);

#elif defined(ESP32_LCDKIT_PAR16)
#define GFX_DEV_DEVICE ESP32_LCDKIT_PAR16
Arduino_DataBus *bus = new Arduino_ESP32PAR16(
    5 /* DC */, GFX_NOT_DEFINED /* CS */, 18 /* WR */, GFX_NOT_DEFINED /* RD */,
    19 /* D0 */, 21 /* D1 */, 0 /* D2 */, 22 /* D3 */, 23 /* D4 */, 33 /* D5 */, 32 /* D6 */, 27 /* D7 */,
    25 /* D8 */, 26 /* D9 */, 12 /* D10 */, 13 /* D11 */, 14 /* D12 */, 15 /* D13 */, 2 /* D14 */, 4 /* D15 */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */);

#elif defined(ESP32_S3_313LCD)
#define GFX_DEV_DEVICE ESP32_S3_313LCD
#define GFX_BL 4
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 0 /* CS */,
    17 /* SCK */, 16 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    15 /* DE */, 5 /* VSYNC */, 6 /* HSYNC */, 7 /* PCLK */,
    19 /* R0 */, 8 /* R1 */, 18 /* R2 */, 17 /* R3 */, 16 /* R4 */,
    11 /* G0 */, 10 /* G1 */, 9 /* G2 */, 46 /* G3 */, 3 /* G4 */, 20 /* G5 */,
    47 /* B0 */, 21 /* B1 */, 14 /* B2 */, 13 /* B3 */, 12 /* B4 */,
    1 /* hsync_polarity */, 30 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 30 /* hsync_back_porch */,
    1 /* vsync_polarity */, 16 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 16 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 16000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    376 /* width */, 960 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, gc9503v_type2_init_operations, sizeof(gc9503v_type2_init_operations));

#elif defined(ESP32_S3_BOX_3)
#define GFX_DEV_DEVICE ARDUINO_ESP32_S3_BOX_3
#define GFX_BL 47
Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 5 /* CS */, 7 /* SCK */, 6 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9342(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */);

#elif defined(ESP32_S3_EYE)
#define GFX_DEV_DEVICE ESP32_S3_EYE
#define DEV_DEVICE_INIT()                            \
    {                                                \
        pinMode(3 /* camera indicator */, OUTPUT);   \
        digitalWrite(3 /* camera indicator */, LOW); \
        pinMode(48 /* BACKLIGHT */, OUTPUT);         \
        digitalWrite(48 /* BACKLIGHT */, LOW);       \
    }
Arduino_DataBus *bus = new Arduino_ESP32SPI(43 /* DC */, 44 /* CS */, 21 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 80 /* row offset 2 */);

#elif defined(ESP32_S3_RGB)
#define GFX_DEV_DEVICE ESP32_S3_RGB
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
// option 1:
// Uncomment for 4" rect display
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    4 /* R0 */, 3 /* R1 */, 2 /* R2 */, 1 /* R3 */, 0 /* R4 */,
    10 /* G0 */, 9 /* G1 */, 8 /* G2 */, 7 /* G3 */, 6 /* G4 */, 5 /* G5 */,
    15 /* B0 */, 14 /* B1 */, 13 /* B2 */, 12 /* B3 */, 11 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type1_init_operations, sizeof(st7701_type1_init_operations));

// option 2:
// Uncomment for 2.1" round display
// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
//     4 /* R0 */, 3 /* R1 */, 2 /* R2 */, 1 /* R3 */, 0 /* R4 */,
//     10 /* G0 */, 9 /* G1 */, 8 /* G2 */, 7 /* G3 */, 6 /* G4 */, 5 /* G5 */,
//     15 /* B0 */, 14 /* B1 */, 13 /* B2 */, 12 /* B3 */, 11 /* B4 */,
//     1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
//     1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
//     0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
//     0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
//     bus, GFX_NOT_DEFINED /* RST */, st7701_type5_init_operations, sizeof(st7701_type5_init_operations));

// option 3:
// Uncomment for 2.8" round display
// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
//     4 /* R0 */, 3 /* R1 */, 2 /* R2 */, 1 /* R3 */, 0 /* R4 */,
//     10 /* G0 */, 9 /* G1 */, 8 /* G2 */, 7 /* G3 */, 6 /* G4 */, 5 /* G5 */,
//     15 /* B0 */, 14 /* B1 */, 13 /* B2 */, 12 /* B3 */, 11 /* B4 */,
//     1 /* hsync_polarity */, 50 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 30 /* hsync_back_porch */,
//     1 /* vsync_polarity */, 20 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 30 /* vsync_back_porch */,
//     0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
//     0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
//     bus, GFX_NOT_DEFINED /* RST */, st7701_type6_init_operations, sizeof(st7701_type6_init_operations));

// option 4:
// Uncomment for 2.0" display
// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
//     4 /* R0 */, 3 /* R1 */, 2 /* R2 */, 1 /* R3 */, 0 /* R4 */,
//     10 /* G0 */, 9 /* G1 */, 8 /* G2 */, 7 /* G3 */, 6 /* G4 */, 5 /* G5 */,
//     15 /* B0 */, 14 /* B1 */, 13 /* B2 */, 12 /* B3 */, 11 /* B4 */,
//     1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
//     1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
//     0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
//     0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     480 /* width */, 360 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
//     bus, GFX_NOT_DEFINED /* RST */, st7701_type8_init_operations, sizeof(st7701_type8_init_operations));

// option 5:
// Uncomment for 3.5" display
// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
//     4 /* R0 */, 3 /* R1 */, 2 /* R2 */, 1 /* R3 */, 0 /* R4 */,
//     10 /* G0 */, 9 /* G1 */, 8 /* G2 */, 7 /* G3 */, 6 /* G4 */, 5 /* G5 */,
//     15 /* B0 */, 14 /* B1 */, 13 /* B2 */, 12 /* B3 */, 11 /* B4 */,
//     1 /* hsync_polarity */, 20 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 38 /* hsync_back_porch */,
//     1 /* vsync_polarity */, 4 /* vsync_front_porch */, 3 /* vsync_pulse_width */, 15 /* vsync_back_porch */,
//     10 /* pclk_active_neg */, 16000000 /* prefer_speed */, false /* useBigEndian */,
//     0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
// Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
//     640 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
//     bus, GFX_NOT_DEFINED /* RST */, st7701_type1_init_operations, sizeof(st7701_type1_init_operations));

#elif defined(ESP32_S3_RPI_DPI)
#define GFX_DEV_DEVICE ESP32_S3_RPI_DPI
// #define GFX_BL 38

#define RGB_PANEL
// e.g. Waveshare 7" RPi DPI LCD: https://www.waveshare.com/wiki/7inch_LCD_for_Pi
// dpi_timings=1024 1 40 48 128 600 1 13 3 45 0 0 0 60 0 37000000 6
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    4 /* R0 */, 3 /* R1 */, 2 /* R2 */, 1 /* R3 */, 0 /* R4 */,
    10 /* G0 */, 9 /* G1 */, 8 /* G2 */, 7 /* G3 */, 6 /* G4 */, 5 /* G5 */,
    15 /* B0 */, 14 /* B1 */, 13 /* B2 */, 12 /* B3 */, 11 /* B4 */,
    1 /* hsync_polarity */, 40 /* hsync_front_porch */, 48 /* hsync_pulse_width */, 128 /* hsync_back_porch */,
    1 /* vsync_polarity */, 13 /* vsync_front_porch */, 3 /* vsync_pulse_width */, 45 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    1024 /* width */, 600 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#elif defined(ESP32S3_2_1_TP)
#define GFX_DEV_DEVICE ESP32S3_2_1_TP
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    4 /* R0 */, 5 /* R1 */, 6 /* R2 */, 7 /* R3 */, 15 /* R4 */,
    8 /* G0 */, 20 /* G1 */, 3 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    11 /* B0 */, 12 /* B1 */, 13 /* B2 */, 14 /* B3 */, 0 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type5_init_operations, sizeof(st7701_type5_init_operations));

/* Golden Security GS-T3E */
#elif defined(GS_T3E)
#define GFX_DEV_DEVICE GS_T3E
#define GFX_BL 42
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 4 /* CS */,
    15 /* SCK */, 48 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 39 /* VSYNC */, 38 /* HSYNC */, 41 /* PCLK */,
    8 /* R0 */, 18 /* R1 */, 17 /* R2 */, 16 /* R3 */, 0 /* R4 */,
    13 /* G0 */, 12 /* G1 */, 11 /* G2 */, 10 /* G3 */, 9 /* G4 */, 3 /* G5 */,
    15 /* B0 */, 48 /* B1 */, 47 /* B2 */, 21 /* B3 */, 14 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, 4 /* RST */, st7701_type1_init_operations, sizeof(st7701_type1_init_operations));

#elif defined(IBUBLY)
#define GFX_DEV_DEVICE IBUBLY
#define GFX_BL 17 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
Arduino_DataBus *bus = new Arduino_ESP32SPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 12 /* RST */, 1 /* rotation */, true /* IPS */, 240 /* width */, 280 /* height */,
    0 /* col offset 1 */, 20 /* row offset 1 */, 0 /* col offset 2 */, 20 /* row offset 2 */);

#elif defined(JC1060P470)
#define GFX_DEV_DEVICE JC1060P470
#define GFX_BL 23 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define DSI_PANEL
Arduino_ESP32DSIPanel *dsipanel = new Arduino_ESP32DSIPanel(
    40 /* hsync_pulse_width */, 160 /* hsync_back_porch */, 160 /* hsync_front_porch */,
    10 /* vsync_pulse_width */, 23 /*vsync_back_porch  */, 12 /* vsync_front_porch */,
    48000000 /* prefer_speed */);
Arduino_DSI_Display *gfx = new Arduino_DSI_Display(
    1024 /* width */, 600 /* height */, dsipanel, 0 /* rotation */, true /* auto_flush */,
    27 /* RST */, jd9165_init_operations, sizeof(jd9165_init_operations) / sizeof(lcd_init_cmd_t));

#elif defined(JC3248W535)
#define GFX_DEV_DEVICE JC3248W535
#define GFX_BL 1
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    45 /* CS */, 47 /* SCK */, 21 /* D0 */, 48 /* D1 */, 40 /* D2 */, 39 /* D3 */);
Arduino_GFX *g = new Arduino_AXS15231B(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, false /* IPS */, 320 /* width */, 480 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */,
    axs15231b_320480_init_operations, sizeof(axs15231b_320480_init_operations));
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    320 /* width */, 480 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(JC3636W518)
#define GFX_DEV_DEVICE JC3636W518
#define GFX_BL 15
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    10 /* CS */, 9 /* SCK */, 11 /* D0 */, 12 /* D1 */, 13 /* D2 */, 14 /* D3 */);
Arduino_GFX *gfx = new Arduino_ST77916(bus, 47 /* RST */, 0 /* rotation */, true /* IPS */, 360 /* width */, 360 /* height */);

#elif defined(JC8012P4A1)
#define GFX_DEV_DEVICE JC8012P4A1
#define GFX_BL 23 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define DSI_PANEL
Arduino_ESP32DSIPanel *dsipanel = new Arduino_ESP32DSIPanel(
    20 /* hsync_pulse_width */, 20 /* hsync_back_porch */, 40 /* hsync_front_porch */,
    4 /* vsync_pulse_width */, 8 /*vsync_back_porch  */, 20 /* vsync_front_porch */,
    60000000 /* prefer_speed */);
Arduino_DSI_Display *gfx = new Arduino_DSI_Display(
    800 /* width */, 1280 /* height */, dsipanel, 0 /* rotation */, true /* auto_flush */,
    27 /* RST */, jd9365_init_operations, sizeof(jd9365_init_operations) / sizeof(lcd_init_cmd_t));

#elif defined(LILYGO_T_DECK)
#define GFX_DEV_DEVICE LILYGO_T_DECK
#define DEV_DEVICE_INIT()                                \
    {                                                    \
        pinMode(9 /* TDECK_RADIO_CS */, OUTPUT);         \
        digitalWrite(9 /* TDECK_RADIO_CS */, HIGH);      \
        pinMode(12 /* TDECK_LCD_CS */, OUTPUT);          \
        digitalWrite(12 /* TDECK_LCD_CS */, HIGH);       \
        pinMode(39 /* TDECK_SDCARD_CS */, OUTPUT);       \
        digitalWrite(39 /* TDECK_SDCARD_CS */, HIGH);    \
        pinMode(10 /* TDECK_PERI_POWERON */, OUTPUT);    \
        digitalWrite(10 /* TDECK_PERI_POWERON */, HIGH); \
        delay(500);                                      \
    }
#define GFX_BL 42
Arduino_DataBus *bus = new Arduino_HWSPI(11 /* DC */, 12 /* CS */, 40 /* SCK */, 41 /* MOSI */, 38 /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */, false /* IPS */);

#elif defined(LILYGO_T_DECK_PLUS)
#define GFX_DEV_DEVICE LILYGO_T_DECK_PLUS
#define DEV_DEVICE_INIT()                                \
    {                                                    \
        pinMode(9 /* TDECK_RADIO_CS */, OUTPUT);         \
        digitalWrite(9 /* TDECK_RADIO_CS */, HIGH);      \
        pinMode(12 /* TDECK_LCD_CS */, OUTPUT);          \
        digitalWrite(12 /* TDECK_LCD_CS */, HIGH);       \
        pinMode(39 /* TDECK_SDCARD_CS */, OUTPUT);       \
        digitalWrite(39 /* TDECK_SDCARD_CS */, HIGH);    \
        pinMode(10 /* TDECK_PERI_POWERON */, OUTPUT);    \
        digitalWrite(10 /* TDECK_PERI_POWERON */, HIGH); \
        delay(500);                                      \
    }
#define GFX_BL 42
Arduino_DataBus *bus = new Arduino_HWSPI(11 /* DC */, 12 /* CS */, 40 /* SCK */, 41 /* MOSI */, 38 /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */, false /* IPS */);

#elif defined(LILYGO_T_DISPLAY)
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY
#define GFX_BL 4
Arduino_DataBus *bus = new Arduino_ESP32SPI(16 /* DC */, 5 /* CS */, 18 /* SCK */, 19 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 23 /* RST */, 0 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */,
    52 /* col offset 1 */, 40 /* row offset 1 */, 53 /* col offset 2 */, 40 /* row offset 2 */);

#elif defined(LILYGO_T_DISPLAY_S3)
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3
#define DEV_DEVICE_INIT()                 \
    {                                     \
        pinMode(15 /* PWD */, OUTPUT);    \
        digitalWrite(15 /* PWD */, HIGH); \
    }
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_ESP32LCD8(
    7 /* DC */, 6 /* CS */, 8 /* WR */, 9 /* RD */,
    39 /* D0 */, 40 /* D1 */, 41 /* D2 */, 42 /* D3 */, 45 /* D4 */, 46 /* D5 */, 47 /* D6 */, 48 /* D7 */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 5 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */,
    35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(LILYGO_T_Display_S3_AMOLED)
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3_AMOLED
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    6 /* CS */, 47 /* SCK */, 18 /* D0 */, 7 /* D1 */, 48 /* D2 */, 5 /* D3 */);
Arduino_GFX *gfx = new Arduino_RM67162(bus, 17 /* RST */, 0 /* rotation */);

#elif defined(LILYGO_T_Display_S3_AMOLED_1_64)
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3_AMOLED_1_64
#define DEV_DEVICE_INIT()                    \
    {                                        \
        pinMode(16 /* LCD_EN */, OUTPUT);    \
        digitalWrite(16 /* LCD_EN */, HIGH); \
    }
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    10 /* CS */, 12 /* SCK */, 11 /* D0 */, 13 /* D1 */, 14 /* D2 */, 15 /* D3 */);
Arduino_GFX *g = new Arduino_CO5300(
    bus, 17 /* RST */, 0 /* rotation */, false /* IPS */, 280, 456,
    20 /* col offset 1 */, 0 /* row offset 1 */, 180 /* col_offset2 */, 24 /* row_offset2 */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    280 /* width */, 456 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(LILYGO_T_Display_S3_LONG)
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3_LONG
#define GFX_BL 1
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    12 /* CS */, 17 /* SCK */, 13 /* D0 */, 18 /* D1 */, 21 /* D2 */, 14 /* D3 */);
Arduino_GFX *g = new Arduino_AXS15231B(
    bus, 47 /* RST */, 0 /* rotation */, false /* IPS */, 180 /* width */, 640 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */,
    axs15231b_180640_init_operations, sizeof(axs15231b_180640_init_operations));
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    180 /* width */, 640 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(LILYGO_T_DISPLAY_S3_PRO)
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3_PRO
#define DEV_DEVICE_INIT()                       \
    {                                           \
        pinMode(14 /* SDCARD_CS */, OUTPUT);    \
        digitalWrite(14 /* SDCARD_CS */, HIGH); \
    }
#define GFX_BL 48
Arduino_DataBus *bus = new Arduino_ESP32SPI(9 /* DC */, 39 /* CS */, 18 /* SCK */, 17 /* MOSI */, 8 /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7796(
    bus, 47 /* RST */, 0 /* rotation */, true /* IPS */, 222 /* width */, 480 /* height */,
    49 /* col offset 1 */, 0 /* row offset 1 */, 49 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(LILYGO_T_QT_PRO)
#define GFX_DEV_DEVICE LILYGO_T_QT_PRO
#define DEV_DEVICE_INIT()               \
    {                                   \
        pinMode(10 /* BL */, OUTPUT);   \
        digitalWrite(10 /* BL */, LOW); \
    }
Arduino_DataBus *bus = new Arduino_ESP32SPI(6 /* DC */, 5 /* CS */, 3 /* SCK */, 2 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9107(bus, 1 /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(LILYGO_T_RGB)
#define GFX_DEV_DEVICE LILYGO_T_RGB
#include <Wire.h>
#define DEV_DEVICE_INIT()                                           \
    {                                                               \
        Wire.begin(8 /* SDA */, 48 /* SCL */, 800000L /* speed */); \
    }
#define GFX_BL 46
Arduino_DataBus *bus = new Arduino_XL9535SWSPI(
    8 /* SDA */, 48 /* SCL */, 2 /* XL PWD */, 3 /* XL CS */, 5 /* XL SCK */, 4 /* XL MOSI */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    45 /* DE */, 41 /* VSYNC */, 47 /* HSYNC */, 42 /* PCLK */,
    21 /* R0 */, 18 /* R1 */, 17 /* R2 */, 16 /* R3 */, 15 /* R4 */,
    14 /* G0 */, 13 /* G1 */, 12 /* G2 */, 11 /* G3 */, 10 /* G4 */, 9 /* G5 */,
    7 /* B0 */, 6 /* B1 */, 5 /* B2 */, 3 /* B3 */, 2 /* B4 */,
    1 /* hsync_polarity */, 50 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 30 /* hsync_back_porch */,
    1 /* vsync_polarity */, 20 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 30 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type4_init_operations, sizeof(st7701_type4_init_operations));

#elif defined(LILYGO_T_TRACK)
#define GFX_DEV_DEVICE LILYGO_T_TRACK
#define DEV_DEVICE_INIT()                  \
    {                                      \
        pinMode(4 /* POWER */, OUTPUT);    \
        digitalWrite(4 /* POWER */, HIGH); \
    }
Arduino_DataBus *bus = new Arduino_ESP32SPIDMA(7 /* DC */, 9 /* CS */, 5 /* SCK */, 6 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_G *g = new Arduino_JD9613(bus, 8 /* RST */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    126 /* width */, 294 /* height */, g, , 0 /* output_x */, 0 /* output_y */, 3 /* rotation */);

#elif defined(LILYGO_T_WATCH_2021)
#define GFX_DEV_DEVICE LILYGO_T_WATCH_2021
#define GFX_BL 21
Arduino_DataBus *bus = new Arduino_ESP32SPI(19 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 27 /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(LILYGO_T4_S3)
#define GFX_DEV_DEVICE LILYGO_T4_S3
#define DEV_DEVICE_INIT()                  \
    {                                      \
        pinMode(9 /* POWER */, OUTPUT);    \
        digitalWrite(9 /* POWER */, HIGH); \
    }
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    11 /* CS */, 15 /* SCK */, 14 /* D0 */, 10 /* D1 */, 16 /* D2 */, 12 /* D3 */);
Arduino_GFX *g = new Arduino_RM690B0(
    bus, 13 /* RST */, 0 /* rotation */, 450 /* width */, 600 /* height */,
    16 /* col offset 1 */, 0 /* row offset 1 */, 16 /* col offset 2 */, 0 /* row offset 2 */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    450 /* width */, 600 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(MAKERFABS_TFT_TOUCH_3_5)
#define GFX_DEV_DEVICE MAKERFABS_TFT_TOUCH_3_5
Arduino_DataBus *bus = new Arduino_ESP32SPI(33 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, 12 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */, false /* IPS */);

#elif defined(MAKERFABS_ESP32_S3_TFT_4_0)
#define GFX_DEV_DEVICE MAKERFABS_ESP32_S3_TFT_4_0
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 1 /* CS */,
    12 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    45 /* DE */, 4 /* VSYNC */, 5 /* HSYNC */, 21 /* PCLK */,
    39 /* R0 */, 40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,
    0 /* G0 */, 9 /* G1 */, 14 /* G2 */, 47 /* G3 */, 48 /* G4 */, 3 /* G5 */,
    6 /* B0 */, 7 /* B1 */, 15 /* B2 */, 16 /* B3 */, 8 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type1_init_operations, sizeof(st7701_type1_init_operations));

#elif defined(MAKERFABS_ESP32_S3_TFT_4_3_v1_3)
#define GFX_DEV_DEVICE MAKERFABS_ESP32_S3_TFT_4_3_v1_3
#define GFX_BL 2
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
    0 /* hsync_polarity */, 40 /* hsync_front_porch */, 48 /* hsync_pulse_width */, 88 /* hsync_back_porch */,
    0 /* vsync_polarity */, 13 /* vsync_front_porch */, 3 /* vsync_pulse_width */, 32 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#elif defined(WAVESHARE_ESP32_C6_LCD_1_47)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_C6_LCD_1_47
#define DEV_DEVICE_INIT()                  \
    {                                      \
        pinMode(4 /* SD_CS */, OUTPUT);    \
        digitalWrite(4 /* SD_CS */, HIGH); \
    }
#define GFX_BL 22
Arduino_DataBus *bus = new Arduino_HWSPI(15 /* DC */, 14 /* CS */, 7 /* SCK */, 6 /* MOSI */, 5 /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 21 /* RST */, 0 /* rotation */, true /* IPS */, 172 /* width */, 320 /* height */,
    34 /* col offset 1 */, 0 /* row offset 1 */, 34 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(WAVESHARE_ESP32_C6_LCD_1_9)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_C6_LCD_1_9
#define DEV_DEVICE_INIT()                   \
    {                                       \
        pinMode(15 /* LCD_BL */, OUTPUT);   \
        digitalWrite(15 /* LCD_BL */, LOW); \
    }
Arduino_DataBus *bus = new Arduino_HWSPI(6 /* DC */, 7 /* CS */, 5 /* SCK */, 4 /* MOSI */, 19 /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 14 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */,
    35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(WAVESHARE_ESP32_S3_LCD_1_3)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_LCD_1_3
#define GFX_BL 20
Arduino_DataBus *bus = new Arduino_ESP32SPI(38 /* DC */, 39 /* CS */, 40 /* SCK */, 41 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 42 /* RST */, 2 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 80 /* row offset 2 */);

#elif defined(WAVESHARE_ESP32_S3_LCD_1_3_PRISM)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_LCD_1_3_PRISM
#define GFX_BL 20
Arduino_DataBus *bus = new Arduino_ESP32SPI(38 /* DC */, 39 /* CS */, 40 /* SCK */, 41 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 42 /* RST */, 6 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 80 /* row offset 2 */);

#elif defined(WAVESHARE_ESP32_S3_LCD_1_46)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_LCD_1_46
#define GFX_BL 5
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    21 /* CS */, 40 /* SCK */, 46 /* D0 */, 45 /* D1 */, 42 /* D2 */, 41 /* D3 */);
Arduino_GFX *g = new Arduino_SPD2010(bus, GFX_NOT_DEFINED /* RST */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    412 /* width */, 412 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(WAVESHARE_ESP32_S3_LCD_1_47)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_LCD_1_47
#define GFX_BL 48
Arduino_DataBus *bus = new Arduino_ESP32SPI(41 /* DC */, 42 /* CS */, 40 /* SCK */, 45 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 39 /* RST */, 0 /* rotation */, true /* IPS */, 172 /* width */, 320 /* height */,
    34 /* col offset 1 */, 0 /* row offset 1 */, 34 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(WAVESHARE_ESP32_S3_LCD_2_8)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_LCD_2_8
#define GFX_BL 5
Arduino_DataBus *bus = new Arduino_HWSPI(41 /* DC */, 42 /* CS */, 40 /* SCK */, 45 /* MOSI */, 46 /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 39 /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(WAVESHARE_ESP32_S3_TOUCH_AMOLED_1_64)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_TOUCH_AMOLED_1_64
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    9 /* CS */, 10 /* SCK */, 11 /* D0 */, 12 /* D1 */, 13 /* D2 */, 14 /* D3 */);
Arduino_GFX *g = new Arduino_CO5300(
    bus, 21 /* RST */, 0 /* rotation */, false /* IPS */, 280 /* width */, 456 /* height */,
    20 /* col offset 1 */, 0 /* row offset 1 */, 180 /* col_offset2 */, 24 /* row_offset2 */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    280 /* width */, 456 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(WAVESHARE_ESP32_S3_TOUCH_AMOLED_1_8)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_TOUCH_AMOLED_1_8
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    12 /* CS */, 11 /* SCK */, 4 /* D0 */, 5 /* D1 */, 6 /* D2 */, 7 /* D3 */);
Arduino_SH8601 *g = new Arduino_SH8601(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, 368 /* width */, 448 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col_offset2 */, 0 /* row_offset2 */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    368 /* width */, 448 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(WAVESHARE_ESP32_S3_TOUCH_AMOLED_2_41)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_TOUCH_AMOLED_2_41
#define DEV_DEVICE_INIT()                     \
    {                                         \
        pinMode(16 /* BAT_PWR */, OUTPUT);    \
        digitalWrite(16 /* BAT_PWR */, HIGH); \
    }
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    9 /* CS */, 10 /* SCK */, 11 /* D0 */, 12 /* D1 */, 13 /* D2 */, 14 /* D3 */);
Arduino_GFX *g = new Arduino_RM690B0(
    bus, 21 /* RST */, 0 /* rotation */, 450 /* width */, 600 /* height */,
    16 /* col offset 1 */, 0 /* row offset 1 */, 16 /* col offset 2 */, 0 /* row offset 2 */);
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    450 /* width */, 600 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(WAVESHARE_ESP32_S3_TOUCH_LCD_3_49)
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_TOUCH_LCD_3_49
#define DEV_DEVICE_INIT()              \
    {                                  \
        pinMode(8 /* BL */, OUTPUT);   \
        digitalWrite(8 /* BL */, LOW); \
    }
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    9 /* CS */, 10 /* SCK */, 11 /* D0 */, 12 /* D1 */, 13 /* D2 */, 14 /* D3 */);
Arduino_GFX *g = new Arduino_AXS15231B(
    bus, 21 /* RST */, 0 /* rotation */, false /* IPS */, 172 /* width */, 640 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 0 /* row offset 2 */,
    axs15231b_180640_init_operations, sizeof(axs15231b_180640_init_operations));
#define CANVAS
Arduino_Canvas *gfx = new Arduino_Canvas(
    172 /* width */, 640 /* height */, g, 0 /* output_x */, 0 /* output_y */, 0 /* rotation */);

#elif defined(WAVESHARE_RP2040_LCD_0_96)
#define GFX_DEV_DEVICE WAVESHARE_RP2040_LCD_0_96
#define GFX_BL 25
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, spi1 /* spi */);
Arduino_GFX *gfx = new Arduino_ST7735(
    bus, 12 /* RST */, 0 /* rotation */, true /* IPS */, 80 /* width */, 160 /* height */,
    26 /* col offset 1 */, 1 /* row offset 1 */, 26 /* col offset 2 */, 1 /* row offset 2 */);

#elif defined(WAVESHARE_RP2040_LCD_1_28)
#define GFX_DEV_DEVICE WAVESHARE_RP2040_LCD_1_28
#define GFX_BL 25
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, spi1 /* spi */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 12 /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(WAVESHARE_RP2350_LCD_1_47)
#define GFX_DEV_DEVICE WAVESHARE_RP2350_LCD_1_47
#define GFX_BL 21
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(16 /* DC */, 17 /* CS */, 18 /* SCK */, 19 /* MOSI */, GFX_NOT_DEFINED /* MISO */, spi0 /* spi */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, 20 /* RST */, 0 /* rotation */, true /* IPS */, 172 /* width */, 320 /* height */,
    34 /* col offset 1 */, 0 /* row offset 1 */, 34 /* col offset 2 */, 0 /* row offset 2 */);

#elif defined(WT32_SC01)
#define GFX_DEV_DEVICE WT32_SC01
#define GFX_BL 23
Arduino_DataBus *bus = new Arduino_ESP32SPI(21 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7796(bus, 22 /* RST */, 3 /* rotation */);

#elif defined(WZ8048C050)
#define GFX_DEV_DEVICE WZ8048C050
#define GFX_BL 2
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 0 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
    0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
    0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#elif defined(XIAO_SAMD21_ROUND_DISPLAY)
#define GFX_DEV_DEVICE XIAO_SAMD21_ROUND_DISPLAY
Arduino_DataBus *bus = new Arduino_HWSPI(3 /* DC */, 1 /* CS */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(XIAO_ESP32C3_ROUND_DISPLAY)
#define GFX_DEV_DEVICE XIAO_ESP32C3_ROUND_DISPLAY
Arduino_DataBus *bus = new Arduino_ESP32SPI(5 /* DC */, 3 /* CS */, 8 /* SCK */, 10 /* MOSI */, 9 /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(XIAO_ESP32S3_ROUND_DISPLAY)
#define GFX_DEV_DEVICE XIAO_ESP32S3_ROUND_DISPLAY
Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 2 /* CS */, 7 /* SCK */, 9 /* MOSI */, 8 /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(ZX2D10GE10R_V4848)
#define GFX_DEV_DEVICE ZX2D10GE10R_V4848
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 21 /* CS */,
    47 /* SCK */, 41 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    39 /* DE */, 48 /* VSYNC */, 40 /* HSYNC */, 45 /* PCLK */,
    10 /* R0 */, 16 /* R1 */, 9 /* R2 */, 15 /* R3 */, 46 /* R4 */,
    8 /* G0 */, 13 /* G1 */, 18 /* G2 */, 12 /* G3 */, 11 /* G4 */, 17 /* G5 */,
    47 /* B0 */, 41 /* B1 */, 0 /* B2 */, 42 /* B3 */, 14 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 10 /* hsync_pulse_width */, 10 /* hsync_back_porch */,
    1 /* vsync_polarity */, 14 /* vsync_front_porch */, 2 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type7_init_operations, sizeof(st7701_type7_init_operations));

#elif defined(ZX3D50CE02S)
#define GFX_DEV_DEVICE ZX3D50CE02S
#define GFX_BL 45
Arduino_DataBus *bus = new Arduino_ESP32LCD8(
    0 /* DC */, GFX_NOT_DEFINED /* CS */, 47 /* WR */, GFX_NOT_DEFINED /* RD */,
    9 /* D0 */, 46 /* D1 */, 3 /* D2 */, 8 /* D3 */, 18 /* D4 */, 17 /* D5 */, 16 /* D6 */, 15 /* D7 */);
Arduino_GFX *gfx = new Arduino_ST7796(bus, 4 /* RST */, 0 /* rotation */, true /* IPS */);

#elif defined(ZX3D95CE01S_AR)
#define GFX_DEV_DEVICE ZX3D95CE01S_AR
#define GFX_BL 45
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 0 /* CS */,
    10 /* SCK */, 9 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    13 /* DE */, 12 /* VSYNC */, 11 /* HSYNC */, 14 /* PCLK */,
    2 /* R0 */, 17 /* R1 */, 16 /* R2 */, 1 /* R3 */, 15 /* R4 */,
    41 /* G0 */, 46 /* G1 */, 3 /* G2 */, 42 /* G3 */, 8 /* G4 */, 18 /* G5 */,
    10 /* B0 */, 9 /* B1 */, 40 /* B2 */, 20 /* B3 */, 19 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, gc9503v_type1_init_operations, sizeof(gc9503v_type1_init_operations));

#elif defined(ZX3D95CE01S_TR)
#define GFX_DEV_DEVICE ZX3D95CE01S_TR
#define GFX_BL 5
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 38 /* CS */,
    45 /* SCK or SCLK */, 48 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 41 /* VSYNC */, 42 /* HSYNC */, 39 /* PCLK */,
    18 /* R0 */, 8 /* R1 */, 3 /* R2 */, 46 /* R3 */, 10 /* R4 */,
    14 /* G0 */, 13 /* G1 */, 12 /* G2 */, 11 /* G3 */, 16 /* G4 */, 17 /* G5 */,
    45 /* B0 */, 48 /* B1 */, 47 /* B2 */, 0 /* B3 */, 21 /* B4 */,
    1 /* hsync_polarity */, 8 /* hsync_front_porch */, 10 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 8 /* vsync_front_porch */, 10 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, 41 /* RST */, gc9503v_type1_init_operations, sizeof(gc9503v_type1_init_operations));

#elif defined(ZX7D00CE01S)
#define GFX_DEV_DEVICE ZX7D00CE01S
#define GFX_BL 45
#define RGB_PANEL
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    39 /* DE */, 38 /* VSYNC */, 5 /* HSYNC */, 9 /* PCLK */,
    10 /* R0 */, 11 /* R1 */, 12 /* R2 */, 13 /* R3 */, 14 /* R4 */,
    21 /* G0 */, 0 /* G1 */, 46 /* G2 */, 3 /* G3 */, 8 /* G4 */, 18 /* G5 */,
    17 /* B0 */, 16 /* B1 */, 15 /* B2 */, 7 /* B3 */, 6 /* B4 */,
    0 /* hsync_polarity */, 0 /* hsync_front_porch */, 210 /* hsync_pulse_width */, 30 /* hsync_back_porch */,
    0 /* vsync_polarity */, 0 /* vsync_front_porch */, 22 /* vsync_pulse_width */, 13 /* vsync_back_porch */,
    0 /* pclk_active_neg */, 12000000 /* prefer_speed */, false /* useBigEndian */,
    0 /* de_idle_high */, 0 /* pclk_idle_high */, 0 /* bounce_buffer_size_px */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

/* Wio Terminal */
#elif defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#define GFX_DEV_DEVICE WIO_TERMINAL
// #define GFX_BL LCD_BACKLIGHT
Arduino_DataBus *bus = new Arduino_HWSPI(LCD_DC /* DC */, LCD_SS_PIN /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 1 /* rotation */);

/* ESP32-S3-BOX */
#elif defined(ARDUINO_ESP32_S3_BOX)
#define GFX_DEV_DEVICE ARDUINO_ESP32_S3_BOX
#define GFX_BL 45
Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 5 /* CS */, 7 /* SCK */, 6 /* MOSI */, 0 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9342(bus, 48 /* RST */, 0 /* rotation */);

/* M5Stack */
#elif defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
#define GFX_DEV_DEVICE ARDUINO_M5Stack_Core_ESP32
// #define GFX_BL 32
Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 14 /* CS */, SCK, MOSI, MISO);
Arduino_GFX *gfx = new Arduino_ILI9342(bus, 33 /* RST */, 2 /* rotation */);

#elif defined(ARDUINO_M5Stack_ATOMS3)
#define GFX_DEV_DEVICE ARDUINO_M5Stack_ATOMS3
#define GFX_BL 16
Arduino_DataBus *bus = new Arduino_ESP32SPI(33 /* DC */, 15 /* CS */, 17 /* SCK */, 21 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9107(bus, 34 /* RST */, 0 /* rotation */, true /* IPS */);

/* Odroid-Go */
#elif defined(ARDUINO_ODROID_ESP32)
#define GFX_DEV_DEVICE ARDUINO_ODROID_ESP32
// #define GFX_BL 14
Arduino_DataBus *bus = new Arduino_ESP32SPI(21 /* DC */, 5 /* CS */, SCK, MOSI, MISO);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, GFX_NOT_DEFINED /* RST */, 3 /* rotation */);
// Arduino_ST7789 *gfx = new Arduino_ST7789(bus, GFX_NOT_DEFINED /* RST */, 3 /* rotation */, true /* IPS */);

/* LILYGO T-Watch */
#elif defined(ARDUINO_T) || defined(ARDUINO_TWATCH_BASE) || defined(ARDUINO_TWATCH_2020_V1) || defined(ARDUINO_TWATCH_2020_V2)
#define GFX_DEV_DEVICE ARDUINO_T_WATCH
#define GFX_BL 12
Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 18 /* SCK */, 19 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_ST7789(
    bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */, 240 /* width */, 240 /* height */,
    0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 80 /* row offset 2 */);

/* Waveshare RP2040-LCD-1.28 */
#elif defined(ARDUINO_WAVESHARE_RP2040_LCD_1_28)
#define GFX_DEV_DEVICE ARDUINO_WAVESHARE_RP2040_LCD_1_28
#define GFX_BL 25
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, 12 /* MISO */, spi1 /* spi */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 12 /* RST */, 0 /* rotation */, true /* IPS */);

#endif
