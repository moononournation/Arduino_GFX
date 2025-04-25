#pragma once

#include "Arduino_DataBus.h"

#if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32P4)

#include <stdio.h>
#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_timer.h>
#include <esp_check.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_mipi_dsi.h>
#include <esp_lcd_panel_io.h>
#include <esp_ldo_regulator.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>

#define EXAMPLE_MIPI_DSI_PHY_PWR_LDO_CHAN 3 // LDO_VO3 连接至 VDD_MIPI_DPHY
#define EXAMPLE_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV 2500
#define DEFAULT_MIPI_DSI_LANE_BIT_RATE_MBPS 750 // 新增宏定义

typedef struct
{
  int cmd;               /*<! The specific LCD command */
  const void *data;      /*<! Buffer that holds the command specific data */
  size_t data_bytes;     /*<! Size of `data` in memory, in bytes */
  unsigned int delay_ms; /*<! Delay in milliseconds after this command */
} lcd_init_cmd_t;

class Arduino_ESP32DSIPanel
{
public:
  Arduino_ESP32DSIPanel(
      uint32_t hsync_pulse_width, uint32_t hsync_back_porch, uint32_t hsync_front_porch,
      uint32_t vsync_pulse_width, uint32_t vsync_back_porch, uint32_t vsync_front_porch,
      uint32_t prefer_speed = GFX_NOT_DEFINED,uint32_t lane_bit_rate = DEFAULT_MIPI_DSI_LANE_BIT_RATE_MBPS /*新增成员变量*/);

  bool begin(int16_t w, int16_t h, int32_t speed = GFX_NOT_DEFINED, const lcd_init_cmd_t *init_operations = NULL, size_t init_operations_len = GFX_NOT_DEFINED);

  uint16_t *getFrameBuffer();

protected:
private:
  uint32_t _hsync_pulse_width;
  uint32_t _hsync_back_porch;
  uint32_t _hsync_front_porch;
  uint32_t _vsync_pulse_width;
  uint32_t _vsync_back_porch;
  uint32_t _vsync_front_porch;
  uint32_t _prefer_speed;
  uint32_t _lane_bit_rate; // 新增成员变量

  esp_lcd_panel_handle_t _panel_handle = NULL;
};

#endif // #if defined(ESP32) && (CONFIG_IDF_TARGET_ESP32P4)
