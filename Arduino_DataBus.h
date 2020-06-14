/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#ifndef _Arduino_DataBus_H_
#define _Arduino_DataBus_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Arduino_DataBus
{
public:
    Arduino_DataBus();

    virtual void begin(uint32_t speed, int8_t dataMode = -1) = 0;
    virtual void beginWrite() = 0;
    virtual void writeCommand(uint8_t c) = 0;
    virtual void writeCommand16(uint16_t c) = 0;
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
#if defined(ESP32)
    virtual void writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len);
    virtual void writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len);
#endif // ESP32

    virtual void sendCommand(uint8_t c) = 0;
    virtual void sendCommand16(uint16_t c) = 0;
    virtual void sendData(uint8_t d) = 0;
    virtual void sendData16(uint16_t d) = 0;
    virtual void sendData32(uint32_t d) = 0;

protected:
    int8_t _dataMode;
};

#endif
