/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _Arduino_DataBus_H_
#define _Arduino_DataBus_H_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include "WProgram.h"
#endif

#if defined(__AVR__)
#define USE_FAST_PINIO ///< Use direct PORT register access
typedef uint8_t ARDUINOGFX_PORT_t;
#elif defined(ARDUINO_ARCH_NRF52840)
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
typedef uint32_t ARDUINOGFX_PORT_t;
#elif defined(ESP32)
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
typedef uint32_t ARDUINOGFX_PORT_t;
#elif defined(ESP8266)
#define USE_FAST_PINIO ///< Use direct PORT register access
typedef uint32_t ARDUINOGFX_PORT_t;
#elif defined(ARDUINO_STM32_FEATHER)
// TODO: fast pin IO?
#elif defined(__arm__)
#if defined(ARDUINO_ARCH_SAMD)
// Adafruit M0, M4
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
typedef uint32_t ARDUINOGFX_PORT_t;
#elif defined(CORE_TEENSY)
#define USE_FAST_PINIO   ///< Use direct PORT register access
#define HAS_PORT_SET_CLR ///< PORTs have set & clear registers
#if defined(__IMXRT1052__) || defined(__IMXRT1062__)
// PJRC Teensy 4.x
typedef uint32_t ARDUINOGFX_PORT_t;
#else
// PJRC Teensy 3.x
typedef uint8_t ARDUINOGFX_PORT_t;
#endif
#else
// Arduino Due?
// USE_FAST_PINIO not available here (yet)...Due has a totally different
// GPIO register set and will require some changes elsewhere (e.g. in
// constructors especially).
#endif
#else  // !ARM
// Unknow architecture, USE_FAST_PINIO is not available here (yet)
// but don't worry about it too much...the digitalWrite() implementation
// on these platforms is reasonably efficient and already RAM-resident,
// only gotcha then is no parallel connection support for now.
#endif // !ARM

#ifdef USE_FAST_PINIO
typedef volatile ARDUINOGFX_PORT_t *PORTreg_t;
#endif

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
#elif defined(ARDUINO_ARCH_NRF52840)
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

#define UNUSED(x) (void)(x)

#define MSB_16_SET(var, val)                                 \
    {                                                        \
        (var) = (((val)&0xFF00) >> 8) | (((val)&0xFF) << 8); \
    }
#define MSB_32_SET(var, val)                                      \
    {                                                             \
        uint8_t *v = (uint8_t *)&(val);                           \
        (var) = v[3] | (v[2] << 8) | (v[1] << 16) | (v[0] << 24); \
    }
#define MSB_32_16_16_SET(var, v1, v2)                                                                                       \
    {                                                                                                                       \
        (var) = (((uint32_t)v2 & 0xff00) << 8) | (((uint32_t)v2 & 0xff) << 24) | ((v1 & 0xff00) >> 8) | ((v1 & 0xff) << 8); \
    }
#define MSB_32_8_ARRAY_SET(var, a)                                      \
    {                                                                   \
        (var) = ((uint32_t)a[0] << 8 | a[1] | a[2] << 24 | a[3] << 16); \
    }

#if defined(ESP32)
#define INLINE __attribute__((always_inline)) inline
#else
#define INLINE inline
#endif

typedef enum
{
    BEGIN_WRITE,
    WRITE_COMMAND_8,
    WRITE_COMMAND_16,
    WRITE_COMMAND_32,
    WRITE_DATA_8,
    WRITE_DATA_16,
    WRITE_DATA_32,
    END_WRITE,
    DELAY,
    SEND_COMMAND_8,
    SEND_COMMAND_16,
    SEND_COMMAND_32,
    SEND_DATA_8,
    SEND_DATA_16,
    SEND_DATA_32,
} spi_operation_type_t;

struct spi_operation_t
{
    spi_operation_type_t type;
    uint32_t value;
};

class Arduino_DataBus
{
public:
    Arduino_DataBus();

    virtual void begin(int32_t speed, int8_t dataMode = -1) = 0;
    virtual void beginWrite() = 0;
    virtual void writeCommand(uint8_t c) = 0;
    virtual void writeCommand16(uint16_t c) = 0;
    virtual void writeCommand32(uint32_t c) = 0;
    virtual void write(uint8_t) = 0;
    virtual void write16(uint16_t) = 0;
    virtual void write32(uint32_t) = 0;
    virtual void writeC8D8(uint8_t c, uint8_t d);
    virtual void writeC8D16(uint8_t c, uint16_t d);
    virtual void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2);
    virtual void writeRepeat(uint16_t p, uint32_t len) = 0;
    virtual void writeBytes(uint8_t *data, uint32_t len) = 0;
    virtual void writePixels(uint16_t *data, uint32_t len) = 0;
    virtual void writePattern(uint8_t *data, uint8_t len, uint32_t repeat) = 0;
    virtual void endWrite() = 0;
    virtual void batchOperation(spi_operation_t batch[], uint8_t len);

#if defined(ARDUINO_ARCH_SAMD) || defined(ESP8266) || defined(ESP32)
    virtual void writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len);
    virtual void writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len);
#endif // defined(ARDUINO_ARCH_SAMD) || defined(ESP8266) || defined(ESP32)

    virtual void sendCommand(uint8_t c) = 0;
    virtual void sendCommand16(uint16_t c) = 0;
    virtual void sendCommand32(uint32_t c) = 0;
    virtual void sendData(uint8_t d) = 0;
    virtual void sendData16(uint16_t d) = 0;
    virtual void sendData32(uint32_t d) = 0;

protected:
    int32_t _speed;
    int8_t _dataMode;
};

#endif
