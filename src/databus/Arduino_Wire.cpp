// Databus Adapter for the Arduino Wire bus (I2C bus)

// The Wire protocol implementation allows specifying prefix data and command bytes instead of using a separate D/C signal.
// when sending

#include "Arduino_Wire.h"

Arduino_Wire::
    Arduino_Wire(uint8_t i2c_addr, TwoWire *wire)
    : _i2c_addr(i2c_addr), _wire(wire) {}

void Arduino_Wire::setOption(option_key key, uint16_t value)
{
  if (key == TWI_COMMAND_PREFIX)
  {
    _use_prefix = true;
    _command_prefix = (uint8_t)value;
  }
  else if (key == TWI_DATA_PREFIX)
  {
    _use_prefix = true;
    _data_prefix = (uint8_t)value;
  }
  else
  {
    Arduino_DataBus::setOption(key, value);
  }
}

bool Arduino_Wire::begin(int32_t speed, int8_t)
{

  if (speed != GFX_NOT_DEFINED)
  {
    _speed = speed;
    _wire->setClock(_speed);
  }

  _wire->begin();

  // find device
  _wire->beginTransmission(_i2c_addr);
  if (_wire->endTransmission())
  {
    // println("Wire::Device not found.");
    is_found = false;
  }
  else
  {
    is_found = true;
  }

  return is_found;
}

void Arduino_Wire::beginWrite()
{
  // println("Wire::beginWrite()");
  if (_speed != GFX_NOT_DEFINED)
  {
    _wire->setClock(_speed);
  }
  _wire->beginTransmission(_i2c_addr);
}

void Arduino_Wire::endWrite()
{
  // println("\nWire::endWrite()");
  _wire->endTransmission();
}

void Arduino_Wire::write(uint8_t d)
{
  // printf("(0x%02x) ", d);
  _wire->write(d);
}

void Arduino_Wire::writeCommand(uint8_t c)
{
  if (_use_prefix)
  {
    _wire->write(_command_prefix);
  }
  _wire->write(c);
}

void Arduino_Wire::writeCommand16(uint16_t)
{
  // println("Wire::writeCommand16()");
  // not implemented.
}

void Arduino_Wire::write16(uint16_t)
{
  // println("Wire::write16()");
  // not implemented
}

void Arduino_Wire::writeRepeat(uint16_t, uint32_t)
{
  // println("Wire::writeRepeat()");
  // not implemented
}

void Arduino_Wire::writePixels(uint16_t *, uint32_t)
{
  // println("Wire::writePixels()");
  // not implemented
}

// write data to the bus using the data prefix
// when len exceeds the TWI_BUFFER_LENGTH then a new transfer protocol is started.
void Arduino_Wire::writePixels(uint8_t *data, uint32_t len)
{
  uint16_t bytesOut = 0;

  if (_use_prefix)
  {
    _wire->write(_data_prefix);
    bytesOut++;
  }

  while (len--)
  {
    if (bytesOut >= TWI_BUFFER_LENGTH)
    {
      // finish this packet and start a new one.
      _wire->endTransmission();
      _wire->beginTransmission(_i2c_addr);
      bytesOut = 0;
      if (_use_prefix)
      {
        _wire->write(_data_prefix);
        bytesOut++;
      }
    }
    _wire->write(*data++);
    bytesOut++;
  }
}

#if !defined(LITTLE_FOOT_PRINT)
void Arduino_Wire::writeBytes(uint8_t *data, uint32_t len)
{
  // println("Wire::writeBytes()");
  // not implemented
}
#endif // !defined(LITTLE_FOOT_PRINT)

void Arduino_Wire::writeRegister(uint8_t reg, uint8_t *data, size_t len)
{
  // println("Wire::writeRegister()");
}

uint8_t Arduino_Wire::readRegister(uint8_t reg, uint8_t *data, size_t len)
{
  // println("Wire::readRegister()");
  return 0;
}
