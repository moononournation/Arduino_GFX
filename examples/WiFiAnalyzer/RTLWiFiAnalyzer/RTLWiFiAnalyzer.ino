/*******************************************************************************
 * Rtl WiFi Analyzer
 * Requires RTL872x board
 *
 * Add realtek ameba core support to Arduino IDE:
 * https://github.com/ambiot/ambd_arduino
 *
 * Old patch realtek ameba core variant.cpp to RTL8720DN pinout:
 * https://github.com/mikey60/BW16-RTL8720DN-Module-Arduino
 ******************************************************************************/

#define SCAN_INTERVAL 3000

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
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

#include <lwip_netconf.h>
#include <wifi_conf.h>
#include <wifi_constants.h>
#include <wifi_structures.h>
#include <wl_definitions.h>
#include <wl_types.h>

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
    60, 0, 0, 0, 68, 0, 0,   // 28-34:  60, 62, 64,N/A, 68,N/A, 96,
    0, 100, 0, 0, 0, 108, 0, // 35-41: N/A,100,102,104,106,108,110,
    0, 0, 116, 0, 0, 0, 124, // 42-48: 112,114,116,118,120,122,124,
    0, 0, 0, 132, 0, 0, 0,   // 49-55: 126,128,N/A,132,134,136,138,
    140, 0, 0, 0,            // 56-59: 140,142,144,N/A,
    149, 0, 0, 0, 157, 0, 0, // 60-66: 149,151,153,155,157,159,161,
    0, 165, 0, 0, 0, 173, 0, // 67-73: 163,165,167,169,171,173,175,
    0, 0};                   // 74-75: 177,N/A

// Channel color mapping
uint16_t channel_color[] = {
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_WHITE, RGB565_CYAN, RGB565_WHITE, RGB565_MAGENTA,
    RGB565_WHITE, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_WHITE, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_WHITE,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_LIME, RGB565_CYAN, RGB565_DODGERBLUE, RGB565_MAGENTA,
    RGB565_RED, RGB565_WHITE};

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
    return 34 + ((channel - 96) / 2);
  }
  if (channel <= 177) // channel 149 - 177
  {
    return 60 + ((channel - 149) / 2);
  }
  return 75;
}

uint8_t _networkCount;
char _networkSsid[WL_NETWORKS_LIST_MAXNUM][WL_SSID_MAX_LENGTH];
int32_t _networkRssi[WL_NETWORKS_LIST_MAXNUM];
uint32_t _networkEncr[WL_NETWORKS_LIST_MAXNUM];
uint8_t _networkChannel[WL_NETWORKS_LIST_MAXNUM];
char _networkMac[WL_NETWORKS_LIST_MAXNUM][18];

rtw_result_t wifidrv_scan_result_handler(rtw_scan_handler_result_t *malloced_scan_result)
{
  rtw_scan_result_t *record;

  if (malloced_scan_result->scan_complete != RTW_TRUE)
  {
    record = &malloced_scan_result->ap_details;
    record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

    if (_networkCount < WL_NETWORKS_LIST_MAXNUM)
    {
      strcpy(_networkSsid[_networkCount], (char *)record->SSID.val);
      _networkRssi[_networkCount] = record->signal_strength;
      _networkEncr[_networkCount] = record->security;
      _networkChannel[_networkCount] = record->channel;
      sprintf(_networkMac[_networkCount], "%02X:%02X:%02X:%02X:%02X:%02X",
              record->BSSID.octet[0], record->BSSID.octet[1], record->BSSID.octet[2],
              record->BSSID.octet[3], record->BSSID.octet[4], record->BSSID.octet[5]);

      _networkCount++;
    }
  }

  return RTW_SUCCESS;
}

int8_t scanNetworks()
{
  uint8_t attempts = 10;

  _networkCount = 0;
  if (wifi_scan_networks(wifidrv_scan_result_handler, NULL) != RTW_SUCCESS)
  {
    return WL_FAILURE;
  }

  do
  {
    delay(SCAN_INTERVAL);
  } while ((_networkCount == 0) && (--attempts > 0));
  return _networkCount;
}

void setup()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX RTL WiFi Analyzer example");

  LwIP_Init();
  wifi_on(RTW_MODE_STA);

#if defined(LCD_PWR_PIN)
  pinMode(LCD_PWR_PIN, OUTPUT);    // sets the pin as output
  digitalWrite(LCD_PWR_PIN, HIGH); // power on
#endif

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);

  w = gfx->width();
  h = gfx->height();
  banner_text_size = (w < 384) ? 1 : 2;
  banner_height = (banner_text_size * 8) + 2;
  graph_height = ((h - banner_height) / 2) - 20; // minus 2 text lines
  graph24_baseline = banner_height + graph_height;
  graph50_baseline = graph24_baseline + 20 + graph_height;
  channel24_width = w / 16;
  channel50_width = w / 56;

  // init banner
  gfx->fillRect(0, 0, w, banner_text_size * 8, RGB565_PURPLE);
  gfx->setTextSize(banner_text_size);
  gfx->setCursor(0, 0);
  gfx->setTextColor(RGB565_WHITE, RGB565_LIME);
  gfx->print(" RTL ");
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

  // WiFi.scanNetworks will return the number of networks found
  int n = scanNetworks();

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
      channel = _networkChannel[i];
      idx = channelIdx(channel);
      rssi = _networkRssi[i];
      bssid = _networkMac[i];
      ssid = _networkSsid[i];

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
      }
    }

    // plot found WiFi info
    for (int i = 0; i < n; i++)
    {
      channel = _networkChannel[i];
      idx = channelIdx(channel);
      rssi = _networkRssi[i];
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
        signal_width = channel50_width * 2;
        offset = (idx - 14 + 3) * channel50_width;
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
        ssid = _networkSsid[i];
        if (ssid.length() == 0)
        {
          ssid = _networkMac[i];
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
        if (_networkEncr[i] == RTW_SECURITY_OPEN)
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
    offset = (idx - 14 + 3) * channel50_width;
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
  delay(SCAN_INTERVAL);
}
