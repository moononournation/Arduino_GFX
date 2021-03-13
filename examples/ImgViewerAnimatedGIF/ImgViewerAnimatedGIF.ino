/*******************************************************************************
 * Animated GIF Image Viewer
 * This is a simple Animated GIF image viewer exsample
 * Image Source: https://www.pexels.com/video/earth-rotating-video-856356/
 * cropped: x: 598 y: 178 width: 720 height: 720 resized: 240x240
 * optimized with ezgif.com
 *
 * Setup steps:
 * 1. Change your LCD parameters in Arduino_GFX setting
 * 2. Upload Animated GIF file
 *   SPIFFS (ESP32):
 *     upload SPIFFS data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/me-no-dev/arduino-esp32fs-plugin
 *   LittleFS (ESP8266):
 *     upload LittleFS data with ESP8266 LittleFS Data Upload:
 *     ESP8266: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin
 *   SD:
 *     Most Arduino system built-in support SD file system.
 *     Wio Terminal require extra dependant Libraries:
 *     - Seeed_Arduino_FS: https://github.com/Seeed-Studio/Seeed_Arduino_FS.git
 *     - Seeed_Arduino_SFUD: https://github.com/Seeed-Studio/Seeed_Arduino_SFUD.git
 ******************************************************************************/
/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#define GIF_FILENAME "/ezgif.com-optimize.gif"
#elif defined(ESP32)
#define GIF_FILENAME "/ezgif.com-optimize.gif"
#else
#define GIF_FILENAME "/ezgif.com-resize.gif"
#endif

/*******************************************************************************
 * Start of Arduino_GFX setting
 * 
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * ESP32 various dev board     : TFT_CS:  5, TFT_DC: 27, TFT_RST: 33, TFT_BL: 22
 * ESP8266 various dev board   : TFT_CS: 15, TFT_DC:  5, TFT_RST: 16, TFT_BL:  4
 * RTL872x various dev board   : TFT_CS: 22, TFT_DC: 17, TFT_RST: 23, TFT_BL: 16
 * Arduino Nano, Micro and more: TFT_CS:  9, TFT_DC:  8, TFT_RST:  7, TFT_BL:  6
 ******************************************************************************/
// #define ESP32_LCDKIT_SPI
// #define TTGO_T_DISPLAY
// #define WT32_SC01
#include <Arduino_GFX_Library.h>

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 0 /* rotation */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#include <SD/Seeed_SD.h>
#elif defined(ESP32)
#include <SPIFFS.h>
// #include <SD.h>
#elif defined(ESP8266)
#include <LittleFS.h>
#include <SD.h>
#else
#include <SD.h>
#endif

#include "GifClass.h"
static GifClass gifClass;

void setup()
{
  Serial.begin(115200);

  // Init Display
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
#elif defined(ESP32)
  if (!SPIFFS.begin())
  // if (!SD.begin(SS))
#elif defined(ESP8266)
  if (!LittleFS.begin())
  // if (!SD.begin(SS))
#else
  if (!SD.begin())
#endif
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
  }
  else
  {
/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
    File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ESP32)
    File gifFile = SPIFFS.open(GIF_FILENAME, "r");
    // File gifFile = SD.open(GIF_FILENAME, "r");
#elif defined(ESP8266)
    File gifFile = LittleFS.open(GIF_FILENAME, "r");
    // File gifFile = SD.open(GIF_FILENAME, "r");
#else
    File gifFile = SD.open(GIF_FILENAME, FILE_READ);
#endif
    if (!gifFile || gifFile.isDirectory())
    {
      Serial.println(F("ERROR: open gifFile Failed!"));
      gfx->println(F("ERROR: open gifFile Failed!"));
    }
    else
    {
      // read GIF file header
      gd_GIF *gif = gifClass.gd_open_gif(&gifFile);
      if (!gif)
      {
        Serial.println(F("gd_open_gif() failed!"));
      }
      else
      {
        uint8_t *buf = (uint8_t *)malloc(gif->width * gif->height);
        if (!buf)
        {
          Serial.println(F("buf malloc failed!"));
        }
        else
        {
          int16_t x = (gfx->width() - gif->width) / 2;
          int16_t y = (gfx->height() - gif->height) / 2;

          Serial.println(F("GIF video start"));
          uint32_t t_fstart, t_delay = 0, t_real_delay, delay_until;
          int32_t res;
          uint32_t duration = 0, remain = 0;
          while (1)
          {
            t_fstart = millis();
            t_delay = gif->gce.delay * 10;
            res = gifClass.gd_get_frame(gif, buf);
            if (res < 0)
            {
              Serial.println(F("ERROR: gd_get_frame() failed!"));
              break;
            }
            else if (res == 0)
            {
              Serial.print(F("rewind, duration: "));
              Serial.print(duration);
              Serial.print(F(", remain: "));
              Serial.print(remain);
              Serial.print(F(" ("));
              Serial.print(100.0 * remain / duration);
              Serial.println(F("%)"));
              duration = 0;
              remain = 0;
              gifClass.gd_rewind(gif);
              continue;
            }

            gfx->drawIndexedBitmap(x, y, buf, gif->palette->colors, gif->width, gif->height);

            t_real_delay = t_delay - (millis() - t_fstart);
            duration += t_delay;
            remain += t_real_delay;
            delay_until = millis() + t_real_delay;
            do
            {
              delay(1);
            } while (millis() < delay_until);
          }
          Serial.println(F("GIF video end"));
          Serial.print(F("duration: "));
          Serial.print(duration);
          Serial.print(F(", remain: "));
          Serial.print(remain);
          Serial.print(F(" ("));
          Serial.print(100.0 * remain / duration);
          Serial.println(F("%)"));

          gifClass.gd_close_gif(gif);
          free(buf);
        }
      }
    }
  }
}

void loop()
{
}
