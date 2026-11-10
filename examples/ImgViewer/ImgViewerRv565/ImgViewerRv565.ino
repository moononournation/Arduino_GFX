/*******************************************************************************
 * RV565 Video Viewer
 * This is a simple RV565 video viewer example
 * RV565 stores each frame as an independent zlib stream that inflates straight
 * to big-endian RGB565, so playback is inflate + draw, with no colour
 * conversion and no loss. Format: https://github.com/elixpo/rv565
 *
 * Convert a video on a computer:
 *   pip install rv565
 *   rv565 encode input.mp4 clip.rv565 --size 240x240 --fps 15 --seconds 10
 * or in the browser, with no upload: https://oreo.elixpo.com/rv565/convert/
 *
 * Dependent libraries:
 * RV565: https://github.com/elixpo/rv565.git (Arduino Library Manager: RV565)
 *
 * Setup steps:
 * 1. Change your LCD parameters in Arduino_GFX setting
 * 2. Upload the RV565 file
 *   LittleFS (ESP32 / Pico):
 *     upload LittleFS data with:
 *     ESP32: https://github.com/lorol/arduino-esp32fs-plugin
 *     Pico: https://github.com/earlephilhower/arduino-pico-littlefs-plugin.git
 *   SD:
 *     Most Arduino system built-in support SD file system.
 ******************************************************************************/
#define RV565_FILENAME "/clip.rv565"

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C2/3 various dev board: CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-C5 various dev board  : CS: 23, DC: 24, RST: 25, BL: 26, SCK: 10, MOSI:  8, MISO: nil
 * ESP32-C6 various dev board  : CS: 18, DC: 22, RST: 23, BL: 15, SCK: 21, MOSI: 19, MISO: nil
 * ESP32-H2 various dev board  : CS:  0, DC: 12, RST:  8, BL: 22, SCK: 10, MOSI: 25, MISO: nil
 * ESP32-P4 various dev board  : CS: 26, DC: 27, RST: 25, BL: 24, SCK: 36, MOSI: 32, MISO: nil
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

#if defined(TARGET_RP2040) || defined(PICO_RP2350)
#include <LittleFS.h>
#include <SD.h>
#elif defined(ESP32)
#include <FFat.h>
#include <LittleFS.h>
#include <SPIFFS.h>
#include <SD.h>
#include <SD_MMC.h>
#else
#error "The RV565 library supports ESP32 and RP2040/RP2350 boards."
#endif

#include <RV565.h>

static File rv565File;
static uint16_t clip_w, clip_h, clip_fps, clip_frames;
static uint8_t *payload_buf;  // compressed frame
static uint8_t *frame_buf;    // decoded frame, big-endian RGB565
static size_t payload_buf_size, frame_bytes;
static int16_t draw_x, draw_y;

/* variables */
static int total_frames = 0;
static unsigned long total_read_video = 0;
static unsigned long total_decode_video = 0;
static unsigned long total_show_video = 0;
static unsigned long start_ms, curr_ms;

static uint32_t rd32(const uint8_t *p)
{
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

// Read and check the 12-byte header, then scan the length fields once to size
// the payload buffer. Returns false for anything that is not an RV565 v6 file.
static bool rv565_begin(File &f)
{
  uint8_t hdr[12];
  if (f.read(hdr, sizeof(hdr)) != sizeof(hdr))
    return false;
  rv565_header_t h;
  if (rv565_parse_header(hdr, sizeof(hdr), 0, 0, &h) != RV565_OK)
    return false;
  clip_w = h.width;
  clip_h = h.height;
  clip_fps = h.fps;
  clip_frames = h.frames;

  payload_buf_size = 0;
  for (uint16_t i = 0; i < clip_frames; ++i)
  {
    uint8_t len[4];
    if (f.read(len, 4) != 4)
      return false;
    const uint32_t l = rd32(len);
    if (l == 0 || !f.seek(f.position() + l))
      return false;
    if (l > payload_buf_size)
      payload_buf_size = l;
  }
  f.seek(12);
  frame_bytes = rv565_frame_bytes(&h);
  return true;
}

void setup()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX RV565 Video Viewer example");

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

#if defined(TARGET_RP2040) || defined(PICO_RP2350)
  if (!LittleFS.begin())
  // if (!SD.begin(SS))
#elif defined(ESP32)
  // if (!FFat.begin())
  if (!LittleFS.begin())
  // if (!SPIFFS.begin())
  // if (!SD.begin(SS))
#endif
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
    return;
  }

#if defined(TARGET_RP2040) || defined(PICO_RP2350)
  rv565File = LittleFS.open(RV565_FILENAME, "r");
  // rv565File = SD.open(RV565_FILENAME, "r");
#elif defined(ESP32)
  // rv565File = FFat.open(RV565_FILENAME, "r");
  rv565File = LittleFS.open(RV565_FILENAME, "r");
  // rv565File = SPIFFS.open(RV565_FILENAME, "r");
  // rv565File = SD.open(RV565_FILENAME, "r");
#endif

  if (!rv565File || rv565File.isDirectory())
  {
    Serial.println(F("ERROR: Failed to open " RV565_FILENAME " file for reading"));
    gfx->println(F("ERROR: Failed to open " RV565_FILENAME " file for reading"));
    return;
  }
  if (!rv565_begin(rv565File))
  {
    Serial.println(F("ERROR: " RV565_FILENAME " is not a valid RV565 file"));
    gfx->println(F("ERROR: " RV565_FILENAME " is not a valid RV565 file"));
    rv565File.close();
    return;
  }
  Serial.printf("RV565 %ux%u, %u fps, %u frames\n", clip_w, clip_h, clip_fps, clip_frames);

  payload_buf = (uint8_t *)malloc(payload_buf_size);
  frame_buf = (uint8_t *)malloc(frame_bytes);
  if (!payload_buf || !frame_buf)
  {
    Serial.println(F("RV565 buffer malloc failed!"));
    rv565File.close();
    return;
  }

  // Centre the clip; a clip larger than the display is clipped by Arduino_GFX.
  draw_x = (gfx->width() - (int16_t)clip_w) / 2;
  draw_y = (gfx->height() - (int16_t)clip_h) / 2;

  Serial.println(F("RV565 video start"));
  start_ms = millis();
  const unsigned long frame_ms = 1000 / clip_fps;
  unsigned long next_ms = start_ms;
  for (uint16_t i = 0; i < clip_frames; ++i)
  {
    // Read the compressed frame
    curr_ms = millis();
    uint8_t len[4];
    rv565File.read(len, 4);
    const uint32_t l = rd32(len);
    if (rv565File.read(payload_buf, l) != l)
    {
      Serial.println(F("ERROR: short read"));
      break;
    }
    total_read_video += millis() - curr_ms;

    // Inflate it to big-endian RGB565
    curr_ms = millis();
    rv565_status_t st = rv565_inflate(payload_buf, l, frame_buf, frame_bytes);
    if (st != RV565_OK)
    {
      Serial.printf("ERROR: frame %u: %s\n", i, rv565_strerror(st));
      break;
    }
    total_decode_video += millis() - curr_ms;

    // Draw it
    curr_ms = millis();
    gfx->draw16bitBeRGBBitmap(draw_x, draw_y, (uint16_t *)frame_buf, clip_w, clip_h);
    total_show_video += millis() - curr_ms;
    ++total_frames;

    // Hold the clip's frame rate when decoding is faster than it
    next_ms += frame_ms;
    const long wait_ms = (long)(next_ms - millis());
    if (wait_ms > 0)
      delay(wait_ms);
  }
  const int time_used = millis() - start_ms;
  Serial.println(F("RV565 video end"));
  rv565File.close();
  free(payload_buf);
  free(frame_buf);

  const float fps = 1000.0 * total_frames / time_used;
  Serial.printf("Total frames: %d\n", total_frames);
  Serial.printf("Time used: %d ms\n", time_used);
  Serial.printf("Average FPS: %0.1f\n", fps);
  Serial.printf("Read: %lu ms (%0.1f %%)\n", total_read_video, 100.0 * total_read_video / time_used);
  Serial.printf("Inflate: %lu ms (%0.1f %%)\n", total_decode_video, 100.0 * total_decode_video / time_used);
  Serial.printf("Draw: %lu ms (%0.1f %%)\n", total_show_video, 100.0 * total_show_video / time_used);

  gfx->setCursor(0, 0);
  gfx->setTextColor(RGB565_WHITE, RGB565_BLACK);
  gfx->printf("Frames: %d\nFPS: %0.1f\nRead: %lu ms\nInflate: %lu ms\nDraw: %lu ms\n",
              total_frames, fps, total_read_video, total_decode_video, total_show_video);
}

void loop()
{
}
