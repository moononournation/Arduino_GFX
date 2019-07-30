/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 * https://github.com/ananevilya/Arduino-ST7789-Library.git
 */
#ifndef _ARDUINO_HWSPI_H_
#define _ARDUINO_HWSPI_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Arduino_DataBus.h"

#if defined (ARDUINO_ARCH_ARC32) || defined (ARDUINO_MAXIM)
  #define SPI_DEFAULT_FREQ  16000000
// Teensy 3.0, 3.1/3.2, 3.5, 3.6
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
  #define SPI_DEFAULT_FREQ  40000000
#elif defined (__AVR__) || defined(TEENSYDUINO)
  #define SPI_DEFAULT_FREQ  8000000
#elif defined(ESP8266) || defined(ESP32)
  #define SPI_DEFAULT_FREQ  40000000
#elif defined(RASPI)
  #define SPI_DEFAULT_FREQ  80000000
#elif defined(ARDUINO_ARCH_STM32F1)
  #define SPI_DEFAULT_FREQ  36000000
#else
  #define SPI_DEFAULT_FREQ  24000000  ///< Default SPI data clock frequency
#endif

#if defined(__AVR__) || defined(CORE_TEENSY)
#include <avr/pgmspace.h>
#define USE_FAST_IO
typedef volatile uint8_t RwReg;
#elif defined(ARDUINO_STM32_FEATHER)
typedef volatile uint32 RwReg;
#define USE_FAST_IO
#elif defined(ARDUINO_FEATHER52)
typedef volatile uint32_t RwReg;
#define USE_FAST_IO
#elif defined(ESP8266)
#include <pgmspace.h>
#elif defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#include <include/pio.h>
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
typedef unsigned char prog_uchar;
#endif

class Arduino_HWSPI : public Arduino_DataBus
{
public:
#if defined(ESP32)
  Arduino_HWSPI(int8_t dc, int8_t cs = -1, int8_t _sck = -1, int8_t _mosi = -1, int8_t _miso = -1); // Constructor
#else
  Arduino_HWSPI(int8_t dc, int8_t cs = -1); // Constructor
#endif

  virtual void begin(uint32_t speed = 0);
  virtual void beginWrite();
  virtual void writeCommand(uint8_t);
  virtual void writeCommand16(uint16_t);
  virtual void write(uint8_t);
  virtual void write16(uint16_t);
  virtual void write32(uint32_t);
  virtual void writePixels(uint16_t, uint32_t);
  virtual void endWrite();

  virtual void sendCommand(uint8_t);
  virtual void sendCommand16(uint16_t);
  virtual void sendData(uint8_t);
  virtual void sendData16(uint16_t);
  virtual void sendData32(uint32_t);
  virtual void setDataMode(uint8_t dataMode);

private:
  inline void CS_HIGH(void);
  inline void CS_LOW(void);
  inline void DC_HIGH(void);
  inline void DC_LOW(void);

  int8_t _dc, _cs;
  uint32_t _speed;
#if defined(ESP32)
  int8_t _sck, _mosi, _miso;
#endif

#if defined(USE_FAST_IO)
  volatile RwReg *csport, *dcport;

#if defined(__AVR__) || defined(CORE_TEENSY) // 8 bit!
  uint8_t cspinmask, dcpinmask;
#else // 32 bit!
  uint32_t cspinmask, dcpinmask;
#endif
#endif // #if defined(USE_FAST_IO)
};

#endif
