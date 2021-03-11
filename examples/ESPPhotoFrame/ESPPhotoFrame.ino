/*******************************************************************************
 * ESP32 Photo Frame
 * This is a simple IoT photo frame sample
 * Please find more details at instructables:
 * https://www.instructables.com/id/Face-Aware-OSD-Photo-Frame/
 * 
 * Setup steps:
 * 1. Fill your own SSID_NAME, SSID_PASSWORD and URL_TEMPLATE
 * 2. Change your LCD parameters in Arduino_GFX setting
 ******************************************************************************/

/* WiFi settings */
#define SSID_NAME "YourAP"
#define SSID_PASSWORD "PleaseInputYourPasswordHere"

#define URL_TEMPLATE "http://YourServerNameOrIP:8080/?w=%d&h=%d"

#define HTTP_TIMEOUT 60000 // in ms, wait a while for server processing

/*******************************************************************************
 * Start of Arduino_GFX setting
 * 
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * ESP32 various dev board     : TFT_CS:  5, TFT_DC: 27, TFT_RST: 33, TFT_BL: 22
 * ESP8266 various dev board   : TFT_CS: 15, TFT_DC:  5, TFT_RST: 16, TFT_BL:  4
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

#if defined(ESP32)
#include <esp_task_wdt.h>
#include <WiFi.h>
#include <HTTPClient.h>
#else // ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif

#include "JpegDec.h"
static JpegDec jpegDec;

static uint32_t len, offset;
static unsigned long next_show_millis = 0;

WiFiClient client;
HTTPClient http;
char url[1024];

void setup()
{
  Serial.begin(115200);
  gfx->begin();
  gfx->fillScreen(BLACK);

  // setup url query value with LCD dimension
  sprintf(url, URL_TEMPLATE, gfx->width(), gfx->height());

  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

#if defined(ESP32)
  // set WDT timeout a little bit longer than HTTP timeout
  esp_task_wdt_init((HTTP_TIMEOUT / 1000) + 1, true);
  enableLoopWDT();
#endif
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    // wait for WiFi connection
    delay(500);
  }
  else if (millis() < next_show_millis)
  {
    delay(1000);
  }
  else
  {
    next_show_millis = ((millis() / 60000L) + 1) * 60000L; // next minute
    Serial.print("[HTTP] begin...\n");
    http.begin(client, url);
    http.setTimeout(HTTP_TIMEOUT);
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();

    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    // HTTP header has been send and Server response header has been handled
    if (httpCode <= 0)
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    else
    {
      if (httpCode != HTTP_CODE_OK)
      {
        Serial.printf("[HTTP] Not OK!\n");
        delay(5000);
      }
      else
      {
        // get lenght of document (is -1 when Server sends no Content-Length header)
        len = http.getSize();
        Serial.printf("[HTTP] size: %d\n", len);

        if (len <= 0)
        {
          Serial.printf("[HTTP] Unknow content size: %d\n", len);
        }
        else
        {
          // get tcp stream
          WiFiClient *http_stream = http.getStreamPtr();
          jpegDec.prepare(http_stream_reader, http_stream);
          jpegDec.decode(JPG_SCALE_NONE, jpegDec.gfx_writer, gfx);
        }
      }
    }
    http.end();
  }

#if defined(ESP32)
  // notify WDT still working
  feedLoopWDT();
#else // ESP8266
  yield();
#endif
}

static size_t http_stream_reader(JpegDec *jpegDec, size_t index, uint8_t *buf, size_t len)
{
  WiFiClient *http_stream = (WiFiClient *)jpegDec->input;
  uint8_t wait = 0;
  if (buf)
  {
    // Serial.printf("[HTTP] read: %d\n", len);
    size_t a = http_stream->available();
    size_t r = 0;
    while ((r < len) && (wait < 10))
    {
      if (a)
      {
        r += http_stream->readBytes(buf + r, min((len - r), a));
      }
      else
      {
        delay(10);
        wait++;
      }
      a = http_stream->available();
    }
    return r;
  }
  else
  {
    // Serial.printf("[HTTP] skip: %d\n", len);
    size_t l = len;
    while ((--l) && (wait < 10))
    {
      if (http_stream->available())
      {
        http_stream->read();
      }
      else
      {
        delay(10);
        wait++;
      }
    }
    return len;
  }
}
