/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HWSPI_NO_DC_H_
#define _ARDUINO_HWSPI_NO_DC_H_

#include <SPI.h>
#include "Arduino_DataBus.h"

#if !defined(LITTLE_FOOT_PRINT)
#ifndef SPI_MAX_PIXELS_AT_ONCE
#define SPI_MAX_PIXELS_AT_ONCE 32
#endif
#endif

// HARDWARE CONFIG ---------------------------------------------------------

class Arduino_HWSPI_NoDC : public Arduino_DataBus
{
public:
#if defined(ESP32)
  Arduino_HWSPI_NoDC(int8_t cs = GFX_NOT_DEFINED, int8_t sck = GFX_NOT_DEFINED, int8_t mosi = GFX_NOT_DEFINED, int8_t miso = GFX_NOT_DEFINED, SPIClass *spi = &SPI, bool is_shared_interface = true, uint16_t commandValue = 0x0000, uint16_t dataValue = 0x8000); // Constructor
#elif defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  Arduino_HWSPI_NoDC(int8_t cs = GFX_NOT_DEFINED, SPIClass *spi = &LCD_SPI, bool is_shared_interface = true, uint16_t commandValue = 0x0000, uint16_t dataValue = 0x8000, uint16_t readValue = 0x4000, uint16_t writeValue= 0x0000); // Constructor
#elif defined(RTL8722DM) && defined(BOARD_RTL8722DM)
  Arduino_HWSPI_NoDC(int8_t cs = GFX_NOT_DEFINED, SPIClass *spi = &SPI1, bool is_shared_interface = true, uint16_t commandValue = 0x0000, uint16_t dataValue = 0x8000, uint16_t readValue = 0x4000, uint16_t writeValue= 0x0000); // Constructor
#else
  Arduino_HWSPI_NoDC(int8_t cs = GFX_NOT_DEFINED, SPIClass *spi = &SPI, bool is_shared_interface = true, uint16_t commandValue = 0x0000, uint16_t dataValue = 0x8000, uint16_t readValue = 0x4000, uint16_t writeValue= 0x0000); // Constructor
#endif

  bool begin(int32_t speed = GFX_NOT_DEFINED, int8_t dataMode = GFX_NOT_DEFINED) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void writeCommandBytes(uint8_t *data, uint32_t len) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;
  void writeRepeat(uint16_t p, uint32_t len) override;
  void writeBytes(uint8_t *data, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

#if !defined(LITTLE_FOOT_PRINT)
  void writePattern(uint8_t *data, uint8_t len, uint32_t repeat) override;
#endif // !defined(LITTLE_FOOT_PRINT)

#if defined(INCLUDE_READ_OPERATIONS)
  uint8_t receive(uint8_t commandByte, uint8_t index = 0) override;
  uint16_t receive16(uint16_t addr) override;
#endif

private:
  INLINE uint16_t WRITE16(uint16_t d);
  INLINE void WRITEBUF(uint8_t *buf, size_t count);
  INLINE void CS_HIGH(void);
  INLINE void CS_LOW(void);


  int8_t _cs;
#if defined(ESP32)
  int8_t _sck, _mosi, _miso;
#endif
  SPIClass *_spi;
  bool _is_shared_interface;
  uint16_t _commandValue, _dataValue, _readValue, _writeValue;

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
#if !defined(KINETISK)
  ARDUINOGFX_PORT_t _csPinMask; ///< Bitmask for chip select
#endif                          // !KINETISK
#else                           // !HAS_PORT_SET_CLR
  PORTreg_t _csPort;               ///< PORT register for chip select
  ARDUINOGFX_PORT_t _csPinMaskSet; ///< Bitmask for chip select SET (OR)
  ARDUINOGFX_PORT_t _csPinMaskClr; ///< Bitmask for chip select CLEAR (AND)
#endif                          // HAS_PORT_SET_CLR
#endif                          // !defined(USE_FAST_PINIO)

#if !defined(LITTLE_FOOT_PRINT)
  union
  {
    uint8_t v8[SPI_MAX_PIXELS_AT_ONCE * 2] = {0};
    uint16_t v16[SPI_MAX_PIXELS_AT_ONCE];
  } _buffer;
#endif // !defined(LITTLE_FOOT_PRINT)
};

#endif // _ARDUINO_HWSPI_H_
