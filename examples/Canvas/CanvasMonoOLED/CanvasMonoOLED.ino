/*******************************************************************************
 * Mono OLED operate with Arduino_Canvas_Mono class Hello World example
 ******************************************************************************/
#define I2C_SCL 1
#define I2C_SDA 2

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#define DEV_DEVICE_INIT()         \
  {                               \
    Wire.begin(I2C_SDA, I2C_SCL); \
  }
Arduino_DataBus *bus = new Arduino_Wire(0x3C /* i2c_addr */, 0x00 /* commandPrefix */, 0x40 /* dataPrefix */, &Wire /* wire */);
Arduino_G *g = new Arduino_SSD1306(bus, GFX_NOT_DEFINED /* RST */, 128 /* width */, 64 /* height */);
// Arduino_G *g = new Arduino_SH1106(bus, GFX_NOT_DEFINED /* RST */, 128 /* width */, 64 /* height */);
#define CANVAS
Arduino_GFX *gfx = new Arduino_Canvas_Mono(128 /* width */, 64 /* height */, g, 0 /* output_x */, 0 /* output_y */, true /* verticalByte */);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void setup(void)
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_Canvas_Mono SSD1306 Hello World example");

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);
#ifdef CANVAS
  gfx->flush();
#endif

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  gfx->setCursor(10, 10);
  gfx->setTextColor(RGB565_RED);
  gfx->println("Hello World!");
#ifdef CANVAS
  gfx->flush();
#endif

  delay(5000); // 5 seconds
}

void loop()
{
  gfx->fillScreen(RGB565_BLACK);
  gfx->setCursor(random(gfx->width()), random(gfx->height()));
  gfx->setTextSize(random(4) /* x scale */, random(4) /* y scale */, random(2) /* pixel_margin */);
  gfx->println("Hello World!");
#ifdef CANVAS
  gfx->flush();
#endif

  delay(1000); // 1 second
}
