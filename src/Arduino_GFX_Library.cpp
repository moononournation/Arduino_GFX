#include "Arduino_GFX_Library.h"

Arduino_DataBus *create_default_Arduino_DataBus()
{
#if defined(ARDUINO_ARCH_NRF52840)
    return new Arduino_NRFXSPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
#elif defined(ESP32)
    return new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
#elif defined(ESP8266)
    return new Arduino_ESP8266SPI(TFT_DC, TFT_CS);
#else
    return new Arduino_HWSPI(TFT_DC, TFT_CS);
#endif
}

Arduino_GFX *create_default_Arduino_GFX()
{
    Arduino_DataBus *bus = create_default_Arduino_DataBus();
#if defined(ESP32_LCDKIT_SPI)
    return new Arduino_ILI9341(bus, TFT_RST, 1 /* rotation */);
#elif defined(TTGO_T_DISPLAY)
    return new Arduino_ST7789(bus, TFT_RST, 2 /* rotation */, true /* IPS */, 135 /* width */, 240 /* height */, 53 /* col offset 1 */, 40 /* row offset 1 */, 52 /* col offset 2 */, 40 /* row offset 2 */);
#elif defined(WT32_SC01)
    return new Arduino_ST7796(bus, TFT_RST, 3 /* rotation */);
#elif defined(WIO_TERMINAL)
    return new Arduino_ILI9341(bus, TFT_RST, 3 /* rotation */);
#elif defined(M5STACK_CORE)
    return new Arduino_ILI9341_M5STACK(bus, TFT_RST, 1 /* rotation */);
#elif defined(ODROID_GO)
    return new Arduino_ILI9341(bus, TFT_RST, 3 /* rotation */);
#elif defined(TTGO_T_WATCH)
    return new Arduino_ST7789(bus, TFT_RST, 2 /* rotation */, true /* IPS */, 240, 240, 0, 80);
#else
    return new Arduino_ILI9341(bus, TFT_RST, 0 /* rotation */);
#endif
}
