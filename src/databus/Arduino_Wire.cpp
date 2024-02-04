// Databus Adapter for the Arduino Wire bus (I2C bus)

#include "Arduino_Wire.h"

Arduino_Wire::
    Arduino_Wire(uint8_t i2c_addr, int8_t commandPrefix, int8_t dataPrefix, TwoWire *wire)
    : _i2c_addr(i2c_addr), _command_prefix(commandPrefix), _data_prefix(dataPrefix), _wire(wire) {}

bool Arduino_Wire::begin(int32_t speed, int8_t)
{
  // printf("Wire::begin _command_prefix=0x%02x _data_prefix=0x%02x.\n", _command_prefix, _data_prefix);

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
    // printf("Wire::Device not found.\n");
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
  // printf("Wire::beginWrite()\n");
  if (_speed != GFX_NOT_DEFINED)
  {
    _wire->setClock(_speed);
  }
  _wire->beginTransmission(_i2c_addr);
}

void Arduino_Wire::endWrite()
{
  // printf("Wire::endWrite()\n");
  _wire->endTransmission();
}

void Arduino_Wire::write(uint8_t d)
{
  // printf("Wire::write(0x%02x)\n", d);
  _wire->write(d);
}

void Arduino_Wire::writeCommand(uint8_t c)
{
  // printf("Wire::writeCommand(0x%02x 0x%02x)\n", _command_prefix, c);
  _wire->write(_command_prefix);
  _wire->write(c);
}

void Arduino_Wire::writeCommand16(uint16_t)
{
  // printf("Wire::writeCommand16()\n");
  // not implemented.
}

void Arduino_Wire::writeCommandBytes(uint8_t *data, uint32_t len)
{
  // printf("Wire::writeCommandBytes(...)\n");
  uint16_t bytesOut;

  _wire->write(_command_prefix);
  bytesOut = 1;

  while (len--)
  {
    if (bytesOut >= TWI_BUFFER_LENGTH)
    {
      // finish this packet and start a new one.
      _wire->endTransmission();
      _wire->beginTransmission(_i2c_addr);
      _wire->write(_command_prefix);
      bytesOut = 1;
    }
    _wire->write(*data++);
    bytesOut++;
  }
}

void Arduino_Wire::write16(uint16_t)
{
  // printf("Wire::write16()\n");
  // not implemented
}

void Arduino_Wire::writeRepeat(uint16_t, uint32_t)
{
  // printf("Wire::writeRepeat()\n");
  // not implemented
}

void Arduino_Wire::writePixels(uint16_t *, uint32_t)
{
  // printf("Wire::writePixels()\n");
  // not implemented
}

// write data to the bus using the data prefix
// when len exceeds the TWI_BUFFER_LENGTH then a new transfer protocol is started.
void Arduino_Wire::writeBytes(uint8_t *data, uint32_t len)
{
  // printf("Wire::writeBytes(...)\n");
  uint16_t bytesOut = 0;

  _wire->write(_data_prefix);
  bytesOut++;

  while (len--)
  {
    if (bytesOut >= TWI_BUFFER_LENGTH)
    {
      // finish this packet and start a new one.
      _wire->endTransmission();
      _wire->beginTransmission(_i2c_addr);
      bytesOut = 0;
      _wire->write(_data_prefix);
      bytesOut++;
    }
    _wire->write(*data++);
    bytesOut++;
  }
}

void Arduino_Wire::writeRegister(uint8_t, uint8_t *, size_t)
{
  // printf("Wire::writeRegister()\n");
}

uint8_t Arduino_Wire::readRegister(uint8_t, uint8_t *, size_t)
{
  // printf("Wire::readRegister()\n");
  return 0;
}
