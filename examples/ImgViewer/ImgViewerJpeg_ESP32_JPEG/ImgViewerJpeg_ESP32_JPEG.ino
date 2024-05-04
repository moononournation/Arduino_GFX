/*******************************************************************************
 * ESP32 SIMD JPEG Image Viewer
 * This is a simple JPEG image viewer example using ESP32 SIMD
 * Image Source: https://github.com/esp-arduino-libs/ESP32_JPEG.git
 *
 * Dependent libraries:
 * ESP32_JPEG: https://github.com/esp-arduino-libs/ESP32_JPEG.git
 *
 * Setup steps:
 * 1. Change your LCD parameters in Arduino_GFX setting
 * 2. Upload JPEG file
 *   FFat/LittleFS:
 *     upload FFat (FatFS) data with ESP32 Sketch Data Upload:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *   SD:
 *     Most Arduino system built-in support SD file system.
 ******************************************************************************/
#define ROOT "/root"
#define JPEG_FILENAME ROOT "/img_test_320_240.jpg"

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
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

#include <FFat.h>
#include <LittleFS.h>
#include <SPIFFS.h>
#include <SD.h>
#include <SD_MMC.h>

#include <ESP32_JPEG_Library.h>
uint8_t *image_jpeg = NULL;
uint8_t *image_rgb = NULL;
size_t image_size = 0;
size_t image_jpeg_size = 0;

static jpeg_error_t esp_jpeg_decoder(const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  FILE *f = fopen(path, "r");
  if (!f)
  {
    Serial.println("- failed to open file for reading");
    return JPEG_ERR_MEM;
  }

  size_t len = fread(image_jpeg, 1, image_size, f);
  fclose(f);
  Serial.printf("Read file done len: %d\n", len);

  jpeg_error_t ret = JPEG_ERR_OK;
  int inbuf_consumed = 0;

  // Generate default configuration
  jpeg_dec_config_t config = {
      .output_type = JPEG_RAW_TYPE_RGB565_BE,
      .rotate = JPEG_ROTATE_0D,
  };

  // Empty handle to jpeg_decoder
  jpeg_dec_handle_t *jpeg_dec = NULL;

  // Create jpeg_dec
  jpeg_dec = jpeg_dec_open(&config);

  // Create io_callback handle
  jpeg_dec_io_t *jpeg_io = (jpeg_dec_io_t *)calloc(1, sizeof(jpeg_dec_io_t));
  if (jpeg_io == NULL)
  {
    return JPEG_ERR_MEM;
  }

  // Create out_info handle
  jpeg_dec_header_info_t *out_info = (jpeg_dec_header_info_t *)calloc(1, sizeof(jpeg_dec_header_info_t));
  if (out_info == NULL)
  {
    return JPEG_ERR_MEM;
  }
  // Set input buffer and buffer len to io_callback
  jpeg_io->inbuf = image_jpeg;
  jpeg_io->inbuf_len = len;

  // Parse jpeg picture header and get picture for user and decoder
  ret = jpeg_dec_parse_header(jpeg_dec, jpeg_io, out_info);
  if (ret < 0)
  {
    Serial.println("JPEG decode parse failed");
    goto _exit;
  }

  jpeg_io->outbuf = image_rgb;
  inbuf_consumed = jpeg_io->inbuf_len - jpeg_io->inbuf_remain;
  jpeg_io->inbuf = image_jpeg + inbuf_consumed;
  jpeg_io->inbuf_len = jpeg_io->inbuf_remain;

  // Start decode jpeg raw data
  ret = jpeg_dec_process(jpeg_dec, jpeg_io);
  if (ret < 0)
  {
    Serial.println("JPEG decode process failed");
    goto _exit;
  }

_exit:
  // Decoder deinitialize
  jpeg_dec_close(jpeg_dec);
  free(out_info);
  free(jpeg_io);
  return ret;
}

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX ESP32 SIMD JPEG Image Viewer example");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  // if (!FFat.begin(false, ROOT))
  if (!LittleFS.begin(false, ROOT))
  // if (!SPIFFS.begin(false, ROOT))
  // SPI.begin(12 /* CLK */, 13 /* D0/MISO */, 11 /* CMD/MOSI */);
  // if (!SD.begin(10 /* CS */, SPI, 80000000L, ROOT))
  // pinMode(10 /* CS */, OUTPUT);
  // digitalWrite(SD_CS, HIGH);
  // SD_MMC.setPins(12 /* CLK */, 11 /* CMD/MOSI */, 13 /* D0/MISO */);
  // if (!SD_MMC.begin(ROOT, true /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_DEFAULT))
  // SD_MMC.setPins(12 /* CLK */, 11 /* CMD/MOSI */, 13 /* D0/MISO */, 14 /* D1 */, 15 /* D2 */, 10 /* D3/CS */);
  // if (!SD_MMC.begin(ROOT, false /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_HIGHSPEED))
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
  }
  else
  {
    image_size = gfx->width() * gfx->height() * 2;
    image_jpeg_size = image_size / 7;
    image_rgb = (uint8_t *)aligned_alloc(16, image_size);
    image_jpeg = (uint8_t *)aligned_alloc(16, image_jpeg_size);

    jpeg_error_t ret = JPEG_ERR_OK;
    uint32_t t = millis();
    ret = esp_jpeg_decoder(JPEG_FILENAME);
    if (ret != JPEG_ERR_OK)
    {
      Serial.printf("JPEG decode failed - %d\n", (int)ret);
    }
    else
    {
      gfx->draw16bitBeRGBBitmap(0, 0, (uint16_t *)image_rgb, gfx->width(), gfx->height());
    }
  }
}

void loop()
{
  int w = gfx->width();
  int h = gfx->height();

  unsigned long start = millis();

  jpeg_error_t ret = JPEG_ERR_OK;
  uint32_t t = millis();
  ret = esp_jpeg_decoder(JPEG_FILENAME);
  if (ret != JPEG_ERR_OK)
  {
    Serial.printf("JPEG decode failed - %d\n", (int)ret);
  }
  else
  {
    gfx->draw16bitBeRGBBitmap(0, 0, (uint16_t *)image_rgb, gfx->width(), gfx->height());
  }

  Serial.printf("Time used: %lu\n", millis() - start);

  delay(1000);
}
