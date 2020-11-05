#ifndef _ARDUINO_ST7701_H_
#define _ARDUINO_ST7701_H_

#include "Arduino.h"
#include "Print.h"
#include "Arduino_GFX.h"
#include "Arduino_TFT.h"

#define ST7701_TFTWIDTH 480
#define ST7701_TFTHEIGHT 480

#define ST7701_RST_DELAY 120    ///< delay ms wait for reset finish
#define ST7701_SLPIN_DELAY 120  ///< delay ms wait for sleep in finish
#define ST7701_SLPOUT_DELAY 120 ///< delay ms wait for sleep out finish

#define ST7701_NOP       0x00
#define ST7701_SWRESET   0x01  ///< Software reset
#define ST7701_RDDPM     0x0A  ///< Read Display Power Mode
#define ST7701_RDDCOLMOD 0x0C  ///< Read Display Pixel Format
#define ST7701_RDDIM     0x0D  ///< Read Display Image Mode
#define ST7701_RDDSM     0x0E  ///< Read Display Signal Mode
#define ST7701_RDDSDR    0x0F  ///< Read Display Self-diagnostic result
#define ST7701_SLPIN     0x10  ///< Sleep in
#define ST7701_SLPOUT    0x11  ///< Sleep out
#define ST7701_PTLON     0x12  ///< Partial mode on
#define ST7701_NORON     0x13  ///< Normal display mode on
#define ST7701_INVOFF    0x20  ///< Display inversion off (normal)
#define ST7701_INVON     0x21  ///< Display inversion on
#define ST7701_ALLPON    0x23  ///< All Pixels On
#define ST7701_DISPOFF   0x28  ///< Display off
#define ST7701_DISPON    0x29  ///< Display on
#define ST7701_TEON      0x35  ///< Tearing effect line on
#define ST7701_MADCTL    0x36  ///< Display data access control
#define ST7701_COLMOD    0x3A  ///< Interface Pixel Format
#define ST7701_WRDISBV   0x51  ///< Write display brightness
#define ST7701_SDIR      0xC7  ///< Source direction control
#define ST7701_RDID1     0xDA  ///< Read ID1
#define ST7701_RDID2     0xDB  ///< Read ID2
#define ST7701_RDID3     0xDC  ///< Read ID3

#define ST7701_CND2BKxSEL 0xFF  ///< Command2 BKx Selection

#define ST7701_BK0_PVGAMCTRL 0xB0  ///< Positive Voltage Gamma Control
#define ST7701_BK0_NVGAMCTRL 0xB1  ///< Negative Voltage Gamma Control
#define ST7701_BK0_LNESET    0xC0  ///< Display Line setting
#define ST7701_BK0_PORCTRL   0xC1  ///< Porch control
#define ST7701_BK0_INVSEL    0xC2  ///< Inversion selection & Frame Rate Control
#define ST7701_BK0_RGBCTRL   0xC3  ///< RGB control
#define ST7701_BK0_PARCTRL   0xC5  ///< Partial mode Control
#define ST7701_BK0_SDIR      0xC7  ///< Source direction control

#define ST7701_BK1_VRHS    0xB0  ///< Vop amplitude setting
#define ST7701_BK1_VCOM    0xB1  ///< VCOM amplitude setting
#define ST7701_BK1_VGHSS   0xB2  ///< VGH Voltage setting
#define ST7701_BK1_TESTCMD 0xB3  ///< TEST Command Setting
#define ST7701_BK1_VGLS    0xB5  ///< VGL Voltage setting
#define ST7701_BK1_PWCTLR1 0xB7  ///< Power Control 1
#define ST7701_BK1_PWCTLR2 0xB8  ///< Power Control 2
#define ST7701_BK1_SPD1    0xC1  ///< Source pre_drive timing set1
#define ST7701_BK1_SPD2    0xC2  ///< Source EQ2 Setting

#define DSI_CMD2BKX_SEL			   0xFF  ///< Command2 BKx selection command
#define ST7701_CMD2BKxSEL_NONE 0x00  ///< Command2 BKx Selection None
#define ST7701_CMD2BK0SEL      0x10  ///< Command2 BK0 Selection
#define ST7701_CMD2BK1SEL      0x11  ///< Command2 BK1 Selection

#define ST7701_CASET 0x2A
#define ST7701_RASET 0x2B
#define ST7701_RAMWR 0x2C
#define ST7701_RAMRD 0x2E

#define ST7701_MADCTL_MY 0x80
#define ST7701_MADCTL_MX 0x40
#define ST7701_MADCTL_MV 0x20
#define ST7701_MADCTL_ML 0x10
#define ST7701_MADCTL_RGB 0x00
#define ST7701_MADCTL_BGR 0x08
#define ST7701_MADCTL_MH 0x04

class Arduino_ST7701 : public Arduino_TFT
{
public:
  Arduino_ST7701(
      Arduino_DataBus *bus, int8_t rst = -1, uint8_t r = 0,
      bool ips = false, int16_t w = ST7701_TFTWIDTH, int16_t h = ST7701_TFTHEIGHT,
      uint8_t col_offset1 = 0, uint8_t row_offset1 = 0, uint8_t col_offset2 = 0, uint8_t row_offset2 = 0);

  virtual void begin(int speed = 0);
  virtual void writeAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  virtual void setRotation(uint8_t r);
  virtual void invertDisplay(bool);
  virtual void displayOn();
  virtual void displayOff();

protected:
  virtual void tftInit();
  virtual void display_cmd(const uint8_t cmd, const uint8_t count, const uint8_t *data);

private:
};

#endif
