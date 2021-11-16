/*******************************************************************************
   PNG Image Viewer
   This is a simple PNG image viewer example
   Image Source: https://github.com/logos
   
   use GIMP to rescale your Image according to Display Resolution
   https://www.gimp.org/
   
   Options for converting images to the format used here include:
   http://www.rinkydinkelectronics.com/t_imageconverter565.php
 ******************************************************************************/
#include "jetpacktocat.h"         //Include Header File of Image PROGRAM MEMORY
/*******************************************************************************
   Start of Arduino_GFX setting

   Arduino_GFX try to find the settings depends on selected board in Arduino IDE
   Or you can define the display dev kit not in the board list
   Defalult pin list for non display dev kit:
   Arduino Nano, Micro and more: TFT_CS:  9, TFT_DC:  8, TFT_RST:  7, TFT_BL:  6
   ESP32 various dev board     : TFT_CS:  5, TFT_DC: 27, TFT_RST: 33, TFT_BL: 22
   ESP32-C3 various dev board  : TFT_CS:  7, TFT_DC:  2, TFT_RST:  1, TFT_BL:  3
   ESP32-S2 various dev board  : TFT_CS: 34, TFT_DC: 26, TFT_RST: 33, TFT_BL: 21
   ESP8266 various dev board   : TFT_CS: 15, TFT_DC:  4, TFT_RST:  2, TFT_BL:  5
   Raspberry Pi Pico dev board : TFT_CS: 17, TFT_DC: 27, TFT_RST: 26, TFT_BL: 28
   RTL872x various dev board   : TFT_CS: 18, TFT_DC: 17, TFT_RST:  2, TFT_BL: 23
   Seeeduino XIAO dev board    : TFT_CS:  3, TFT_DC:  2, TFT_RST:  1, TFT_BL:  0
   Teensy 4.1 dev board        : TFT_CS: 39, TFT_DC: 41, TFT_RST: 40, TFT_BL: 22
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 3 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
   End of Arduino_GFX setting
 ******************************************************************************/
void setup()
{
  Serial.begin(115200);
  Serial.println("Embed Image Viewer");

  gfx->begin();             // Init Display
  gfx->fillScreen(BLACK);   // Clear Display

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

  gfx->setCursor(0, 0);
  gfx->draw16bitRGBBitmap(40 /* x */, 0 /* y */, jetpacktocat /*Image PROG Name*/, IMG_WIDTH /*Width*/, IMG_HEIGHT /*Height*/);  //Draw Image on Display
  /*void draw16bitRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);*/        //Actual Function API from Lib for drawing Image
}

void loop()
{
}
