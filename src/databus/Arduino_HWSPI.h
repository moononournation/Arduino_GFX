/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HWSPI_H_
#define _ARDUINO_HWSPI_H_

#include "Arduino_DataBus.h"

// HARDWARE CONFIG ---------------------------------------------------------

class Arduino_HWSPI : public Arduino_DataBus
{
public:
#if defined(ESP32)
  Arduino_HWSPI(int8_t dc, int8_t cs = -1, int8_t sck = -1, int8_t mosi = -1, int8_t miso = -1, bool is_shared_interface = true); // Constructor
#else
  Arduino_HWSPI(int8_t dc, int8_t cs = -1, bool is_shared_interface = true); // Constructor
#endif

  virtual void begin(int32_t speed = 0, int8_t dataMode = -1);
  virtual void beginWrite();
  virtual void writeCommand(uint8_t);
  virtual void writeCommand16(uint16_t);
  virtual void writeCommand32(uint32_t);
  virtual void write(uint8_t);
  virtual void write16(uint16_t);
  virtual void write32(uint32_t);
  virtual void writeRepeat(uint16_t p, uint32_t len);
  virtual void writeBytes(uint8_t *data, uint32_t len);
  virtual void writePixels(uint16_t *data, uint32_t len);
  virtual void writePattern(uint8_t *data, uint8_t len, uint32_t repeat);
  virtual void endWrite();

  virtual void sendCommand(uint8_t);
  virtual void sendCommand16(uint16_t);
  virtual void sendCommand32(uint32_t);
  virtual void sendData(uint8_t);
  virtual void sendData16(uint16_t);
  virtual void sendData32(uint32_t);

private:
  INLINE void CS_HIGH(void);
  INLINE void CS_LOW(void);
  INLINE void DC_HIGH(void);
  INLINE void DC_LOW(void);

  int8_t _dc, _cs;
#if defined(ESP32)
  int8_t _sck, _mosi, _miso;
#endif
  bool _is_shared_interface;

  // CLASS INSTANCE VARIABLES --------------------------------------------

  // Here be dragons! There's a big union of three structures here --
  // one each for hardware SPI, software (bitbang) SPI, and parallel
  // interfaces. This is to save some memory, since a display's connection
  // will be only one of these. The order of some things is a little weird
  // in an attempt to get values to align and pack better in RAM.

#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
  PORTreg_t _csPortSet; ///< PORT register for chip select SET
  PORTreg_t _csPortClr; ///< PORT register for chip select CLEAR
  PORTreg_t _dcPortSet; ///< PORT register for data/command SET
  PORTreg_t _dcPortClr; ///< PORT register for data/command CLEAR
#if !defined(KINETISK)
  ARDUINOGFX_PORT_t _csPinMask; ///< Bitmask for chip select
  ARDUINOGFX_PORT_t _dcPinMask; ///< Bitmask for data/command
#endif                         // !KINETISK
#else                          // !HAS_PORT_SET_CLR
  PORTreg_t _csPort;               ///< PORT register for chip select
  PORTreg_t _dcPort;               ///< PORT register for data/command
  ARDUINOGFX_PORT_t _csPinMaskSet; ///< Bitmask for chip select SET (OR)
  ARDUINOGFX_PORT_t _csPinMaskClr; ///< Bitmask for chip select CLEAR (AND)
  ARDUINOGFX_PORT_t _dcPinMaskSet; ///< Bitmask for data/command SET (OR)
  ARDUINOGFX_PORT_t _dcPinMaskClr; ///< Bitmask for data/command CLEAR (AND)
#endif                         // HAS_PORT_SET_CLR
#endif                         // !defined(USE_FAST_PINIO)
};

#endif // _ARDUINO_HWSPI_H_
