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

    virtual void begin(uint32_t speed) = 0;
    virtual void beginWrite() = 0;
    virtual void writeCommandCore(uint8_t c) = 0;
    virtual void write(uint8_t) = 0;
    virtual void write16(uint16_t) = 0;
    virtual void write32(uint32_t) = 0;
    virtual void endWrite() = 0;
    virtual void writeCommand(uint8_t c) = 0;
    virtual void writeData(uint8_t d) = 0;
    virtual void writePixels(uint16_t p, uint32_t len) = 0;
};

#endif
