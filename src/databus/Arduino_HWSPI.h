/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _ARDUINO_HWSPI_H_
#define _ARDUINO_HWSPI_H_

// HARDWARE CONFIG ---------------------------------------------------------

#if defined(__AVR__)
typedef uint8_t ARDUINOGFX_PORT_t;
#define USE_FAST_PINIO ///< Use direct PORT register access
#elif defined(ESP32)
typedef uint32_t ARDUINOGFX_PORT_t;
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
#elif defined(ESP8266)
typedef uint32_t ARDUINOGFX_PORT_t;
#define USE_FAST_PINIO ///< Use direct PORT register access
#elif defined(ARDUINO_STM32_FEATHER)
typedef uint32_t ARDUINOGFX_PORT_t;
#elif defined(__arm__)
#if defined(ARDUINO_ARCH_SAMD)
// Adafruit M0, M4
typedef uint32_t ARDUINOGFX_PORT_t;
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
#elif defined(CORE_TEENSY)
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)
// PJRC Teensy 4.x
typedef uint32_t ARDUINOGFX_PORT_t;
#else
// PJRC Teensy 3.x
typedef uint8_t ARDUINOGFX_PORT_t;
#endif
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
#else
// Arduino Due?
typedef uint32_t ARDUINOGFX_PORT_t;
// USE_FAST_PINIO not available here (yet)...Due has a totally different
// GPIO register set and will require some changes elsewhere (e.g. in
// constructors especially).
#endif
#else  // !ARM
// Unknow architecture, USE_FAST_PINIO is not available here (yet)
// but don't worry about it too much...the digitalWrite() implementation
// on these platforms is reasonably efficient and already RAM-resident,
// only gotcha then is no parallel connection support for now.
typedef uint32_t ARDUINOGFX_PORT_t;
#endif // end !ARM
typedef volatile ARDUINOGFX_PORT_t *PORTreg_t;

#if defined(ADAFRUIT_PYPORTAL) || defined(ADAFRUIT_PYBADGE_M4_EXPRESS) || defined(ADAFRUIT_PYGAMER_M4_EXPRESS) || defined(ADAFRUIT_HALLOWING_M4_EXPRESS)
#define USE_SPI_DMA ///< Auto DMA if using PyPortal
#else
//#define USE_SPI_DMA               ///< If set, use DMA if available
#endif
// Another "oops" name -- this now also handles parallel DMA.
// If DMA is enabled, Arduino sketch MUST #include <Adafruit_ZeroDMA.h>
// Estimated RAM usage:
// 4 bytes/pixel on display major axis + 8 bytes/pixel on minor axis,
// e.g. 320x240 pixels = 320 * 4 + 240 * 8 = 3,200 bytes.

#if !defined(ARDUINO_ARCH_SAMD)
#undef USE_SPI_DMA ///< DMA currently for SAMD chips only
#endif

#if defined(USE_SPI_DMA)
#include <Adafruit_ZeroDMA.h>
#endif

#include "Arduino_DataBus.h"

#if defined(ARDUINO_ARCH_ARC32) || defined(ARDUINO_MAXIM)
#define SPI_DEFAULT_FREQ 16000000
// Teensy 3.0, 3.1/3.2, 3.5, 3.6
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
#define SPI_DEFAULT_FREQ 40000000
// Teensy 4.x
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
#define SPI_DEFAULT_FREQ 40000000
#elif defined(__AVR__) || defined(TEENSYDUINO)
#define SPI_DEFAULT_FREQ 8000000
#elif defined(ESP8266) || defined(ESP32)
#define SPI_DEFAULT_FREQ 40000000
#elif defined(RASPI)
#define SPI_DEFAULT_FREQ 80000000
#elif defined(ARDUINO_ARCH_STM32F1)
#define SPI_DEFAULT_FREQ 36000000
#else
#define SPI_DEFAULT_FREQ 24000000 ///< Default SPI data clock frequency
#endif

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
  PORTreg_t csPortSet; ///< PORT register for chip select SET
  PORTreg_t csPortClr; ///< PORT register for chip select CLEAR
  PORTreg_t dcPortSet; ///< PORT register for data/command SET
  PORTreg_t dcPortClr; ///< PORT register for data/command CLEAR
#else                  // !HAS_PORT_SET_CLR
  PORTreg_t csPort;               ///< PORT register for chip select
  PORTreg_t dcPort;               ///< PORT register for data/command
#endif                 // end HAS_PORT_SET_CLR
#endif                 // end USE_FAST_PINIO

#if defined(USE_SPI_DMA)             // Used by hardware SPI and tft8
  Adafruit_ZeroDMA dma;              ///< DMA instance
  DmacDescriptor *dptr = NULL;       ///< 1st descriptor
  DmacDescriptor *descriptor = NULL; ///< Allocated descriptor list
  uint16_t *pixelBuf[2];             ///< Working buffers
  uint16_t maxFillLen;               ///< Max pixels per DMA xfer
  uint16_t lastFillColor = 0;        ///< Last color used w/fill
  uint32_t lastFillLen = 0;          ///< # of pixels w/last fill
  uint8_t onePixelBuf;               ///< For hi==lo fill
#endif
#if defined(USE_FAST_PINIO)
#if defined(HAS_PORT_SET_CLR)
#if !defined(KINETISK)
  ARDUINOGFX_PORT_t csPinMask; ///< Bitmask for chip select
  ARDUINOGFX_PORT_t dcPinMask; ///< Bitmask for data/command
#endif                         // end !KINETISK
#else                          // !HAS_PORT_SET_CLR
  ARDUINOGFX_PORT_t csPinMaskSet; ///< Bitmask for chip select SET (OR)
  ARDUINOGFX_PORT_t csPinMaskClr; ///< Bitmask for chip select CLEAR (AND)
  ARDUINOGFX_PORT_t dcPinMaskSet; ///< Bitmask for data/command SET (OR)
  ARDUINOGFX_PORT_t dcPinMaskClr; ///< Bitmask for data/command CLEAR (AND)
#endif                         // end HAS_PORT_SET_CLR
#endif                         // end USE_FAST_PINIO
};

#endif // _ARDUINO_HWSPI_H_
