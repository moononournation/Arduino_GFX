// RST pin used
#define TFT_RST 2 // ESP8266 e-paper proto board

// SPI TFTs (DC, CS)
//Arduino_DataBus *bus = new Arduino_HWSPI(0, SS); // ESP8266
//Arduino_DataBus *bus = new Arduino_HWSPI(8, SS); // UNO

//Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST);
//Arduino_GFX *gfx = new Arduino_ILI9486_18bit(bus, TFT_RST, 0, false);
//Arduino_GFX *gfx = new Arduino_SSD1283A(bus, TFT_RST, 0);
//Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0, false,
//                                      128 /* width */, 128 /* height */,
//                                      2 /* col offset 1 */, 3 /* row offset 1 */,
//                                      2 /* col offset 2 */, 1 /* row offset 2 */); // 1.44
//Arduino_GFX *gfx = new Arduino_ST7735(bus, TFT_RST, 0); // 1.8
//Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 0, false); // ESP8266
//Arduino_GFX *gfx = new Arduino_ST7789(bus, 9, 0, true); // UNO

// MCUFriend UNO TFT shields, 8bit parallel
Arduino_DataBus *bus = new Arduino_UNOPAR8;

Arduino_GFX *gfx = new Arduino_ILI9341(bus, A4);
//Arduino_GFX *gfx = new Arduino_ILI9486(bus, A4, 0, false);

// Arduino MEGA shield, 16bit parallel
// Arduino_AVRPAR16(int8_t dc, int8_t cs, int8_t wr, int8_t rd, uint8_t portLow, uint8_t portHigh);
//Arduino_DataBus *bus = new Arduino_AVRPAR16(38, 40, 39, 43, 3, 1);

//Arduino_GFX *gfx = new Arduino_ILI9488(bus, 41, 0, false);
//Arduino_GFX *gfx = new Arduino_ILI9486(bus, 41, 0, false);
//Arduino_GFX *gfx = new Arduino_NT35510(bus, 41, 0, false);

// Arduino MEGA shield on DUE, 16bit parallel
//#include "databus/Arduino_DUEPAR16.h"
//Arduino_DataBus *bus = new Arduino_DUEPAR16;

//Arduino_GFX *gfx = new Arduino_NT35510(bus, 41, 0, false);
