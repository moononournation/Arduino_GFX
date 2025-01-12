/*******************************************************************************
 * TIFF Image Viewer
 * This is a simple TIFF image viewer example
 * Image Source: https://people.math.sc.edu/Burkardt/data/tif/tif.html
 * ImageMagick script: convert at3_1m4_01.tif -threshold 40% -compress group4 at3_1m4_01_g4.tif
 *
 * Dependent libraries:
 * TIFF_G4: https://github.com/bitbank2/TIFF_G4.git
 *
 * Setup steps:
 * 1. Change your LCD parameters in Arduino_GFX setting
 * 2. Upload TIFF file
 *   FFat (ESP32):
 *     upload FFat (FatFS) data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *   LittleFS (ESP32 / ESP8266 / Pico):
 *     upload LittleFS data with ESP8266 LittleFS Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *     ESP8266: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin
 *     Pico: https://github.com/earlephilhower/arduino-pico-littlefs-plugin.git
 *   SPIFFS (ESP32):
 *     upload SPIFFS data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *   SD:
 *     Most Arduino system built-in support SD file system.
 *     Wio Terminal require extra dependant Libraries:
 *     - Seeed_Arduino_FS: https://github.com/Seeed-Studio/Seeed_Arduino_FS.git
 *     - Seeed_Arduino_SFUD: https://github.com/Seeed-Studio/Seeed_Arduino_SFUD.git
 ******************************************************************************/
#define TIFF_FILENAME "/at3_1m4_01_g4.tif"

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, DF_GFX_RST, 3 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#include <SD/Seeed_SD.h>
#elif defined(TARGET_RP2040) || defined(PICO_RP2350)
#include <LittleFS.h>
#include <SD.h>
#elif defined(ESP32)
#include <FFat.h>
#include <LittleFS.h>
#include <SPIFFS.h>
#include <SD.h>
#include <SD_MMC.h>
#elif defined(ESP8266)
#include <LittleFS.h>
#include <SD.h>
#else
#include <SD.h>
#endif

#include <TIFF_G4.h>
TIFFG4 tiff;
int16_t display_width, display_height;
const uint16_t us2BppToRGB565[4] = {0xffff, 0xa514, 0x528a, 0x0000};
uint8_t *tiffBuf;

void TIFFDraw(TIFFDRAW *pDraw)
{
  // Serial.println("TIFFDraw");
  uint8_t c, *src;
  int i, l;
  uint16_t usTemp[640], *d;

  if (pDraw->y >= display_height)
  {
    return; // beyond bottom of the display
  }

  src = pDraw->pPixels;
  if (pDraw->ucPixelType == TIFF_PIXEL_1BPP)
  {
    // Serial.printf("TIFF_PIXEL_1BPP, pDraw->iScaledWidth: %d\n", pDraw->iScaledWidth);
    memset(usTemp, 0, sizeof(usTemp)); // start with black
    for (i = 0; i < (pDraw->iScaledWidth + 7) / 8; i++)
    {
      d = &usTemp[i * 8];
      c = *src++;
      while (c)
      {
        if (c & 0x80)
          *d = 0xffff;
        c <<= 1;
        d++;
      }
    }
  }
  else if (pDraw->ucPixelType == TIFF_PIXEL_2BPP) // 2-bit gray anti-aliased pixels
  {
    // Serial.printf("TIFF_PIXEL_2BPP, pDraw->iScaledWidth: %d\n", pDraw->iScaledWidth);
    d = usTemp;
    l = pDraw->iScaledWidth;
    for (i = 0; i < l; i++)
    {
      if ((i & 0b11) == 0)
      {
        c = *src++;
      }
      *d++ = us2BppToRGB565[c >> 6];
      c <<= 2;
    }
  }
  gfx->draw16bitBeRGBBitmap(0, pDraw->y, usTemp, pDraw->iScaledWidth, 1);
} /* TIFFDraw() */

void setup()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX TIFF Image Viewer example");

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  display_width = gfx->width();
  display_height = gfx->height();

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 4000000UL))
#elif defined(TARGET_RP2040) || defined(PICO_RP2350)
  if (!LittleFS.begin())
  // if (!SD.begin(SS))
#elif defined(ESP32)
  // if (!FFat.begin())
  if (!LittleFS.begin())
  // if (!SPIFFS.begin())
  // SPI.begin(12 /* CLK */, 13 /* D0/MISO */, 11 /* CMD/MOSI */);
  // if (!SD.begin(10 /* CS */, SPI))
  // pinMode(10 /* CS */, OUTPUT);
  // digitalWrite(10 /* CS */, HIGH);
  // SD_MMC.setPins(12 /* CLK */, 11 /* CMD/MOSI */, 13 /* D0/MISO */);
  // if (!SD_MMC.begin("/root", true /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_DEFAULT))
  // SD_MMC.setPins(12 /* CLK */, 11 /* CMD/MOSI */, 13 /* D0/MISO */, 14 /* D1 */, 15 /* D2 */, 10 /* D3/CS */);
  // if (!SD_MMC.begin("/root", false /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_HIGHSPEED))
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
    File tiffFile = SD.open(TIFF_FILENAME, "r");
#elif defined(TARGET_RP2040) || defined(PICO_RP2350)
    File tiffFile = LittleFS.open(TIFF_FILENAME, "r");
    // File tiffFile = SD.open(TIFF_FILENAME, "r");
#elif defined(ESP32)
    // File tiffFile = FFat.open(TIFF_FILENAME, "r");
    File tiffFile = LittleFS.open(TIFF_FILENAME, "r");
    // File tiffFile = SPIFFS.open(TIFF_FILENAME, "r");
    // File tiffFile = SD.open(TIFF_FILENAME, "r");
#elif defined(ESP8266)
    File tiffFile = LittleFS.open(TIFF_FILENAME, "r");
    // File tiffFile = SD.open(TIFF_FILENAME, "r");
#else
    File tiffFile = SD.open(TIFF_FILENAME, FILE_READ);
#endif
    if (!tiffFile || tiffFile.isDirectory())
    {
      Serial.println(F("ERROR: Failed to open " TIFF_FILENAME " file for reading"));
      gfx->println(F("ERROR: Failed to open " TIFF_FILENAME " file for reading"));
    }
    else
    {
      size_t fileSize = tiffFile.size();
      tiffBuf = (uint8_t *)malloc(fileSize);
      tiffFile.read(tiffBuf, fileSize);
      if (!tiff.openTIFF(tiffBuf, fileSize, TIFFDraw))
      {
        Serial.println(F("ERROR: Failed to decode " TIFF_FILENAME " file"));
        gfx->println(F("ERROR: Failed to decode " TIFF_FILENAME " file"));
      }
      else
      {
        int cx = tiff.getWidth(); // scale the image to fit the display
        float fScale = (float)display_width / (float)cx;
        // Use TIFF_PIXEL_1BPP for bitonal only, 2BPP for anti-aliased grayscale
        tiff.setDrawParameters(fScale, TIFF_PIXEL_2BPP, 0, 0, display_width * 2, display_height * 2, NULL);
        tiff.decode();
        tiff.close();
      }
    }
  }
}

void loop()
{
}
