/*******************************************************************************
 * LVGL Hello World
 * This is a simple examplle for LVGL - Light and Versatile Graphics Library
 *
 * Dependent libraries:
 * LVGL: https://github.com/lvgl/lvgl.git
 ******************************************************************************/
#include <lvgl.h>

/*******************************************************************************
 * Start of Arduino_GFX setting
 * 
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: TFT_CS:  9, TFT_DC:  8, TFT_RST:  7, TFT_BL:  6
 * ESP32 various dev board     : TFT_CS:  5, TFT_DC: 27, TFT_RST: 33, TFT_BL: 22
 * ESP32-C3 various dev board  : TFT_CS:  7, TFT_DC:  2, TFT_RST:  1, TFT_BL:  3
 * ESP32-S2 various dev board  : TFT_CS: 34, TFT_DC: 26, TFT_RST: 33, TFT_BL: 21
 * ESP8266 various dev board   : TFT_CS: 15, TFT_DC:  4, TFT_RST:  2, TFT_BL:  5
 * Raspberry Pi Pico dev board : TFT_CS: 17, TFT_DC: 27, TFT_RST: 26, TFT_BL: 28
 * RTL8720 BW16 old patch core : TFT_CS: 18, TFT_DC: 17, TFT_RST:  2, TFT_BL: 23
 * RTL8720_BW16 Official core  : TFT_CS:  9, TFT_DC:  8, TFT_RST:  6, TFT_BL:  3
 * RTL8722 dev board           : TFT_CS: 18, TFT_DC: 17, TFT_RST: 22, TFT_BL: 23
 * RTL8722_mini dev board      : TFT_CS: 12, TFT_DC: 14, TFT_RST: 15, TFT_BL: 13
 * Seeeduino XIAO dev board    : TFT_CS:  3, TFT_DC:  2, TFT_RST:  1, TFT_BL:  0
 * Teensy 4.1 dev board        : TFT_CS: 39, TFT_DC: 41, TFT_RST: 40, TFT_BL: 22
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
   uint32_t w = (area->x2 - area->x1 + 1);
   uint32_t h = (area->y2 - area->y1 + 1);

   gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

   lv_disp_flush_ready(disp);
}

void setup()
{
   Serial.begin(115200);
   // while (!Serial);
   Serial.println("LVGL Hello World");

   // Init Display
   gfx->begin();
   gfx->fillScreen(BLACK);

#ifdef TFT_BL
   pinMode(TFT_BL, OUTPUT);
   digitalWrite(TFT_BL, HIGH);
#endif

   lv_init();

   screenWidth = gfx->width();
   screenHeight = gfx->height();
   disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 10);
   if (!disp_draw_buf)
   {
      Serial.println("LVGL disp_draw_buf allocate failed!");
   }
   else
   {
      lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 10);

      /* Initialize the display */
      lv_disp_drv_init(&disp_drv);
      /* Change the following line to your display resolution */
      disp_drv.hor_res = screenWidth;
      disp_drv.ver_res = screenHeight;
      disp_drv.flush_cb = my_disp_flush;
      disp_drv.draw_buf = &draw_buf;
      lv_disp_drv_register(&disp_drv);

      /* Initialize the (dummy) input device driver */
      static lv_indev_drv_t indev_drv;
      lv_indev_drv_init(&indev_drv);
      indev_drv.type = LV_INDEV_TYPE_POINTER;
      lv_indev_drv_register(&indev_drv);

      /* Create simple label */
      lv_obj_t *label = lv_label_create(lv_scr_act());
      lv_label_set_text(label, "Hello Arduino! (V8.0.X)");
      lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

      Serial.println("Setup done");
   }
}

void loop()
{
   lv_timer_handler(); /* let the GUI do its work */
   delay(5);
}
