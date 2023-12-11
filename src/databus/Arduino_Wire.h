// Databus Adapter for the Arduino Wire bus (I2C bus)

// The setOption function was added to the Arduino_DataBus class to allow configuration fom the
// display into the bus. This is helpful as the display chips require their specific protocol
// behaviors. e.g. The D/C (data/command) switching is based on specific GPIO pin or the 9-th
// bit in SPI. The TWI /Wire protocol adds a byte ino the protocol. 

#ifndef _Arduino_Wire_H_
#define _Arduino_Wire_H_

#include <Wire.h>
#include "Arduino_DataBus.h"

class Arduino_Wire : public Arduino_DataBus
{
public:
  // Constructors
  Arduino_Wire(uint8_t i2c_addr, TwoWire *wire = &Wire);

  void setOption(option_key option_key, uint16_t option_value) override;

  bool begin(int32_t speed = GFX_NOT_DEFINED, int8_t dataMode = GFX_NOT_DEFINED) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;
  void writeRepeat(uint16_t p, uint32_t len) override;
  void writePixels(uint8_t *data, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

#if !defined(LITTLE_FOOT_PRINT)
  void writeBytes(uint8_t *data, uint32_t len) override;
#endif // !defined(LITTLE_FOOT_PRINT)

protected:
  void writeRegister(uint8_t reg, uint8_t *data, size_t len);
  uint8_t readRegister(uint8_t reg, uint8_t *data, size_t len);

  uint8_t output_buf = 0;
  bool is_found;

  uint8_t _i2c_addr;
  TwoWire *_wire;

  bool _use_prefix;
  uint8_t _command_prefix;
  uint8_t _data_prefix;

  int32_t _speed;
private:
};

#endif // _Arduino_Wire_H_
