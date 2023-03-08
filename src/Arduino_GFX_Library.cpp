#include "Arduino_GFX_Library.h"

Arduino_DataBus *create_default_Arduino_DataBus()
{
#if defined(ARDUINO_ARCH_NRF52840)
  return new Arduino_NRFXSPI(DF_GFX_DC, DF_GFX_CS, DF_GFX_SCK, DF_GFX_MOSI, DF_GFX_MISO);
#elif defined(TARGET_RP2040)
  return new Arduino_RPiPicoSPI(DF_GFX_DC, DF_GFX_CS, DF_GFX_SCK, DF_GFX_MOSI, DF_GFX_MISO, DF_GFX_SPI);
#elif defined(ESP32)
  return new Arduino_ESP32SPI(DF_GFX_DC, DF_GFX_CS, DF_GFX_SCK, DF_GFX_MOSI, DF_GFX_MISO);
#elif defined(ESP8266)
  return new Arduino_ESP8266SPI(DF_GFX_DC, DF_GFX_CS);
#else
  return new Arduino_HWSPI(DF_GFX_DC, DF_GFX_CS);
#endif
}

Arduino_GFX *create_default_Arduino_GFX()
{
  Arduino_DataBus *bus = create_default_Arduino_DataBus();
#if defined(WIO_TERMINAL)
  return new Arduino_ILI9341(bus, DF_GFX_RST, 1 /* rotation */);
#elif defined(ESP32_S3_BOX)
  return new Arduino_ILI9342(bus, DF_GFX_RST, 0 /* rotation */);
#elif defined(M5STACK_CORE)
  return new Arduino_ILI9342(bus, DF_GFX_RST, 2 /* rotation */);
#elif defined(ODROID_GO)
  return new Arduino_ILI9341(bus, DF_GFX_RST, 3 /* rotation */);
#elif defined(TTGO_T_WATCH)
  return new Arduino_ST7789(bus, DF_GFX_RST, 0 /* rotation */, true /* IPS */, 240, 240, 0, 80);
#elif defined(WAVESHARE_RP2040_LCD_1_28)
  return new Arduino_ST7789(bus, DF_GFX_RST, 0 /* rotation */, true /* IPS */, 240, 240, 0, 80);
#else
  return new Arduino_GC9A01(bus, DF_GFX_RST, 0 /* rotation */, true /* IPS */);
#endif
}

void gfx_draw_bitmap_to_framebuffer(
    uint16_t *from_bitmap, int16_t bitmap_w, int16_t bitmap_h,
    uint16_t *framebuffer, int16_t x, int16_t y, int16_t framebuffer_w, int16_t framebuffer_h)
{
  uint16_t max_x = framebuffer_w - 1;
  uint16_t max_y = framebuffer_h - 1;
  if (
      ((x + bitmap_w - 1) < 0) || // Outside left
      ((y + bitmap_h - 1) < 0) || // Outside top
      (x > max_x) ||              // Outside right
      (y > max_y)                 // Outside bottom
  )
  {
    return;
  }
  else
  {
    int16_t xskip = 0;
    if ((y + bitmap_h - 1) > max_y)
    {
      bitmap_h -= (y + bitmap_h - 1) - max_y;
    }
    if (y < 0)
    {
      from_bitmap -= y * bitmap_w;
      bitmap_h += y;
      y = 0;
    }
    if ((x + bitmap_w - 1) > max_x)
    {
      xskip = (x + bitmap_w - 1) - max_x;
      bitmap_w -= xskip;
    }
    if (x < 0)
    {
      from_bitmap -= x;
      xskip -= x;
      bitmap_w += x;
      x = 0;
    }
    uint16_t *row = framebuffer;
    row += y * framebuffer_w;
    row += x;
    if (((framebuffer_w & 1) == 0) && ((xskip & 1) == 0) && ((bitmap_w & 1) == 0))
    {
      uint32_t *row2 = (uint32_t *)row;
      uint32_t *from_bitmap2 = (uint32_t *)from_bitmap;
      int16_t framebuffer_w2 = framebuffer_w >> 1;
      int16_t xskip2 = xskip >> 1;
      int16_t w2 = bitmap_w >> 1;

      for (int16_t j = 0; j < bitmap_h; j++)
      {
        for (int16_t i = 0; i < w2; i++)
        {
          row2[i] = *from_bitmap2++;
        }
        from_bitmap2 += xskip2;
        row2 += framebuffer_w2;
      }
    }
    else
    {
      for (int j = 0; j < bitmap_h; j++)
      {
        for (int i = 0; i < bitmap_w; i++)
        {
          row[i] = *from_bitmap++;
        }
        from_bitmap += xskip;
        row += framebuffer_w;
      }
    }
  }
}
