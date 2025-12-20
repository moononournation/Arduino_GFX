/*******************************************************************************
 * ESP32-C5 WiFi Analyzer
 * Requires ESP32-C5 board
 ******************************************************************************/
#if CONFIG_SOC_WIFI_SUPPORT_5G

// POWER SAVING SETTING
#define SCAN_INTERVAL 3000
// #define SCAN_COUNT_SLEEP 5
// #define LCD_PWR_PIN 0
// #define LCD_PWR_PIN1 1
// #define LCD_PWR_PIN2 2
// #define LCD_PWR_PIN3 3

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * ESP32-C5 various dev board  : CS: 23, DC: 24, RST: 25, BL: 26, SCK: 10, MOSI:  8, MISO: nil
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
Arduino_GFX *gfx = new Arduino_ILI9341(bus, DF_GFX_RST, 1 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include <WiFi.h>

int16_t w, h, banner_text_size, banner_height, graph24_baseline, graph50_baseline, graph_baseline, graph_height, channel24_width, channel50_width, signal_width;

// RSSI RANGE
#define RSSI_CEILING -30
#define RSSI_SHOW_SSID -70
#define RSSI_FLOOR -100

// Channel legend mapping
uint8_t channel_legend[] = {
    1, 2, 3, 4, 5, 6, 7,     // 0-6:     1,  2,  3,  4,  5,  6,  7,
    8, 9, 10, 11, 12, 13, 0, // 7-13:    8,  9, 10, 11, 12, 13, 14,
    0, 0, 36, 0, 0, 0, 44,   // 14-20:  32, 34, 36, 38, 40, 42, 44,
    0, 0, 0, 52, 0, 0, 0,    // 21-27:  46, 48, 50, 52, 54, 56, 58,
    60, 0, 0, 0, 68, 0, 0,   // 28-34:  60, 62, 64,N/A, 68,N/A,N/A,
    0, 0, 0, 0, 0, 0, 0,     // 35-41: N/A,N/A,N/A,N/A,N/A,N/A, 96,
    0, 100, 0, 0, 0, 108, 0, // 42-48: N/A,100,102,104,106,108,110,
    0, 0, 116, 0, 0, 0, 124, // 49-55: 112,114,116,118,120,122,124,
    0, 0, 0, 132, 0, 0, 0,   // 56-62: 126,128,N/A,132,134,136,138,
    140, 0, 0, 0, 149, 0, 0, // 63-69: 140,142,144,N/A,149,151,153,
    0, 157, 0, 0, 0, 165, 0, // 70-76: 155,157,159,161,163,165,167,
    0, 0, 173, 0, 0, 0};     // 77-82: 169,171,173,175,177,N/A

// Channel color mapping
uint16_t channel_color[] = {
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_WHITE, RGB565_CYAN, RGB565_WHITE, RGB565_WHITE,
    RGB565_WHITE, RGB565_WHITE, RGB565_WHITE, RGB565_WHITE, RGB565_WHITE, RGB565_WHITE, RGB565_MAGENTA,
    RGB565_WHITE, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_WHITE, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_WHITE, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_WHITE};

uint8_t scan_count = 0;

uint16_t channelIdx(int channel)
{
  if (channel <= 14) // 2.4 GHz, channel 1-14
  {
    return channel - 1;
  }
  if (channel <= 68) // 5 GHz, channel 32 - 64
  {
    return 14 + ((channel - 32) / 2);
  }
  if (channel <= 144) // channel 96 - 144
  {
    return 41 + ((channel - 96) / 2);
  }
  if (channel <= 177) // channel 149 - 177
  {
    return 67 + ((channel - 149) / 2);
  }
  return 82;
}

void setup()
{
#if defined(LCD_PWR_PIN)
  pinMode(LCD_PWR_PIN, OUTPUT);    // sets the pin as output
  digitalWrite(LCD_PWR_PIN, HIGH); // power on
#endif
#if defined(LCD_PWR_PIN)
  pinMode(LCD_PWR_PIN1, OUTPUT);    // sets the pin as output
  pinMode(LCD_PWR_PIN2, OUTPUT);    // sets the pin as output
  pinMode(LCD_PWR_PIN3, OUTPUT);    // sets the pin as output
  digitalWrite(LCD_PWR_PIN1, HIGH); // power on
  digitalWrite(LCD_PWR_PIN2, HIGH); // power on
  digitalWrite(LCD_PWR_PIN3, HIGH); // power on
#endif

#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX ESP32-C5 WiFi Analyzer UTF8 example");

  // Enable Station Interface
  WiFi.STA.begin();

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

  w = gfx->width();
  h = gfx->height();
  banner_text_size = (w < 444) ? 1 : 2;
  banner_height = (banner_text_size * 8) + 2;
  graph_height = ((h - banner_height) / 2) - 20; // minus 2 text lines
  graph24_baseline = banner_height + graph_height;
  graph50_baseline = graph24_baseline + 20 + graph_height;
  channel24_width = w / (14 + 2);
  channel50_width = w / (sizeof(channel_legend) - 14 + 4);

  // init banner
  gfx->fillRect(0, 0, w, banner_text_size * 8, RGB565_PURPLE);
  gfx->setTextSize(banner_text_size);
  gfx->setCursor(0, 0);
  gfx->setTextColor(RGB565_WHITE, RGB565_RED);
  gfx->print(" ESP32");
  gfx->setTextColor(RGB565_WHITE, RGB565_DARKORANGE);
  gfx->print("-C5 ");
  gfx->setTextColor(RGB565_WHITE, RGB565_LIMEGREEN);
  gfx->print(" Dual Band ");
  gfx->setTextColor(RGB565_WHITE, RGB565_MEDIUMBLUE);
  gfx->print(" WiFi ");
  gfx->setTextColor(RGB565_WHITE, RGB565_PURPLE);
  gfx->print(" Analyzer");
  gfx->setTextSize(1);

#ifdef CANVAS
  gfx->flush();
#endif
}

bool matchBssidPrefix(uint8_t *a, uint8_t *b)
{
  for (uint8_t i = 0; i < 5; i++)
  { // only compare first 5 bytes
    if (a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

void loop()
{
  uint16_t ap24_count = 0, ap50_count = 0;
  uint8_t ap_count_list[sizeof(channel_legend)];
  int32_t peak_list[sizeof(channel_legend)];
  int16_t peak_id_list[sizeof(channel_legend)];
  int32_t channel;
  int16_t idx;
  int32_t rssi;
  uint8_t *bssid;
  String ssid;
  uint16_t color;
  int16_t height, offset, text_width;

  // init array value
  for (int i = 0; i < sizeof(channel_legend); ++i)
  {
    ap_count_list[i] = 0;
    peak_list[i] = RSSI_FLOOR;
    peak_id_list[i] = -1;
  }

  WiFi.setBandMode((scan_count < 1) ? WIFI_BAND_MODE_2G_ONLY : WIFI_BAND_MODE_AUTO);
  // WiFi.setBandMode(WIFI_BAND_MODE_5G_ONLY);

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks(false /* async */, true /* show_hidden */, false /* passive */, (scan_count < 2) ? 30 : 300 /* max_ms_per_chan */);

  // clear old graph
  gfx->fillRect(0, banner_height, w, h - banner_height, RGB565_BLACK);

  if (n == 0)
  {
    gfx->setTextColor(RGB565_WHITE);
    gfx->setCursor(0, banner_height);
    gfx->println("No networks found");
  }
  else
  {
    for (int i = 0; i < n; i++)
    {
      channel = WiFi.channel(i);
      idx = channelIdx(channel);
      rssi = WiFi.RSSI(i);
      bssid = WiFi.BSSID(i);
      ssid = WiFi.SSID(i);

      // channel peak stat, find peak ssid
      if (ssid.length() > 0)
      {
        if (peak_list[idx] < rssi)
        {
          peak_list[idx] = rssi;
          peak_id_list[idx] = i;
        }
      }

      // check signal come from same AP
      bool duplicate_SSID = false;
      for (int j = 0; j < i; j++)
      {
        if ((WiFi.channel(j) == channel) && matchBssidPrefix(WiFi.BSSID(j), bssid))
        {
          duplicate_SSID = true;
          break;
        }
      }

      if (!duplicate_SSID)
      {
        ap_count_list[idx]++;
        if (channel <= 14)
        {
          ap24_count++;
        }
        else
        {
          ap50_count++;
        }
      }
    }
    Serial.printf("ap24_count: %d, ap50_count: %d\n", ap24_count, ap50_count);

    // plot found WiFi info
    for (int i = 0; i < n; i++)
    {
      channel = WiFi.channel(i);
      idx = channelIdx(channel);
      rssi = WiFi.RSSI(i);
      color = channel_color[idx];
      height = constrain(map(rssi, RSSI_FLOOR, RSSI_CEILING, 1, graph_height), 1, graph_height);
      if (idx < 14)
      {
        graph_baseline = graph24_baseline;
        signal_width = channel24_width * 2;
        offset = (idx + 2) * channel24_width;
      }
      else
      {
        graph_baseline = graph50_baseline;
        signal_width = channel50_width * 4;
        offset = (idx - 14 + 4) * channel50_width;
      }

      // trim rssi with RSSI_FLOOR
      if (rssi < RSSI_FLOOR)
      {
        rssi = RSSI_FLOOR;
      }

      // plot chart
      // gfx->drawLine(offset, graph_baseline - height, offset - signal_width, graph_baseline + 1, color);
      // gfx->drawLine(offset, graph_baseline - height, offset + signal_width, graph_baseline + 1, color);
      gfx->startWrite();
      gfx->writeEllipseHelper(offset, graph_baseline + 1, signal_width, height, 0b0011, color);
      gfx->endWrite();

      if ((rssi >= RSSI_SHOW_SSID) && (i == peak_id_list[idx]))
      {
        // Print SSID, signal strengh and if not encrypted
        ssid = WiFi.SSID(i);
        if (ssid.length() == 0)
        {
          ssid = WiFi.BSSIDstr(i);
        }
        text_width = (ssid.length() + 6) * 6;
        if (text_width > w)
        {
          offset = 0;
        }
        else
        {
          offset -= signal_width;
          if ((offset + text_width) > w)
          {
            offset = w - text_width;
          }
        }
        gfx->setTextColor(color);
        gfx->setCursor(offset, ((height + 8) > graph_height) ? (graph_baseline - graph_height) : (graph_baseline - 10 - height));
        gfx->print(ssid);
        gfx->print('(');
        gfx->print(rssi);
        gfx->print(')');
        if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)
        {
          gfx->print('*');
        }
      }
    }
  }

  // draw 2.4 GHz graph base axle
  gfx->drawFastHLine(0, graph24_baseline, w, RGB565_WHITE);
  for (idx = 0; idx < 14; idx++)
  {
    channel = channel_legend[idx];
    offset = (idx + 2) * channel24_width;
    if (channel > 0)
    {
      gfx->setTextColor(channel_color[idx]);
      gfx->setCursor(offset - ((channel < 10) ? 3 : 6), graph24_baseline + 2);
      gfx->print(channel);
    }
    if (ap_count_list[idx] > 0)
    {
      gfx->setTextColor(RGB565_LIGHTGREY);
      gfx->setCursor(offset - ((ap_count_list[idx] < 10) ? 3 : 6), graph24_baseline + 8 + 2);
      gfx->print(ap_count_list[idx]);
    }
  }

  // draw 5 GHz graph base axle
  gfx->drawFastHLine(0, graph50_baseline, w, RGB565_WHITE);
  for (idx = 14; idx < sizeof(channel_legend); idx++)
  {
    channel = channel_legend[idx];
    offset = (idx - 14 + 4) * channel50_width;
    if (channel > 0)
    {
      gfx->setTextColor(channel_color[idx]);
      gfx->setCursor(offset - ((channel < 100) ? 6 : 9), graph50_baseline + 2);
      gfx->print(channel);
    }
    if (ap_count_list[idx] > 0)
    {
      gfx->setTextColor(RGB565_LIGHTGREY);
      gfx->setCursor(offset - ((ap_count_list[idx] < 10) ? 3 : 6), graph50_baseline + 8 + 2);
      gfx->print(ap_count_list[idx]);
    }
  }
  gfx->setTextColor(RGB565_MEDIUMBLUE, RGB565_BLACK);
  gfx->setCursor(w - 18, graph24_baseline + 8 + 2);
  gfx->printf("%03d", ap24_count);
  gfx->setTextColor(RGB565_LIMEGREEN, RGB565_BLACK);
  gfx->setCursor(w - 18, graph50_baseline + 8 + 2);
  gfx->printf("%03d", ap50_count);

  gfx->setTextSize(2);
  gfx->setTextColor(RGB565_WHITE, RGB565_MEDIUMBLUE);
  gfx->setCursor(0, graph24_baseline + 1);
  gfx->print("2.4");
  gfx->setTextColor(RGB565_WHITE, RGB565_LIMEGREEN);
  gfx->setCursor(0, graph50_baseline + 1);
  gfx->print("5");
  gfx->setTextSize(1);

#ifdef CANVAS
  gfx->flush();
#endif

  // Wait a bit before scanning again
  delay((scan_count < 2) ? 0 : SCAN_INTERVAL);
  ++scan_count;

#if defined(SCAN_COUNT_SLEEP)
  // POWER SAVING
  if (scan_count >= SCAN_COUNT_SLEEP)
  {
#if defined(LCD_PWR_PIN)
    pinMode(LCD_PWR_PIN, INPUT); // disable pin
#endif
#if defined(LCD_PWR_PIN1)
    pinMode(LCD_PWR_PIN1, INPUT); // disable pin
    pinMode(LCD_PWR_PIN2, INPUT); // disable pin
    pinMode(LCD_PWR_PIN3, INPUT); // disable pin
#endif

#if defined(GFX_BL)
    pinMode(GFX_BL, INPUT); // disable pin
#endif

    esp_deep_sleep_start();
  }
#endif // defined(SCAN_COUNT_SLEEP)
}

#endif // #if CONFIG_SOC_WIFI_SUPPORT_5G
