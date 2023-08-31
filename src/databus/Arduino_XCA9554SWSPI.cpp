#include "Arduino_XCA9554SWSPI.h"

Arduino_XCA9554SWSPI::Arduino_XCA9554SWSPI(int8_t rst, int8_t cs, int8_t sck, int8_t mosi, TwoWire *wire, uint8_t i2c_addr)
  : _rst(rst), _cs(cs), _sck(sck), _mosi(mosi), _wire(wire), _address(i2c_addr)
{
}

bool Arduino_XCA9554SWSPI::begin(int32_t, int8_t)
{
  _wire->begin();
  _wire->beginTransmission(_address);
  if (!_wire->endTransmission())
  {
    Serial.println("Found xCA9554");
    is_found = true;
    if (_rst != GFX_NOT_DEFINED)
    {
      this->pinMode(_rst, OUTPUT);
      this->digitalWrite(_rst, 0);
      delay(10);
      this->digitalWrite(_rst, 1);
      delay(100);
    }
    this->pinMode(_cs, OUTPUT);
    this->digitalWrite(_cs, 1);
    this->pinMode(_sck, OUTPUT);
    this->digitalWrite(_sck, 1);
    this->pinMode(_mosi, OUTPUT);
    this->digitalWrite(_mosi, 1);
  }
  else
  {
    Serial.println("xCA9554 not found");
    is_found = false;
  }

  return true;
}

void Arduino_XCA9554SWSPI::beginWrite()
{
  this->digitalWrite(_cs, 0);
}

void Arduino_XCA9554SWSPI::endWrite()
{
  this->digitalWrite(_cs, 1);
}

void Arduino_XCA9554SWSPI::writeCommand(uint8_t c)
{
  // D/C bit, command
  this->digitalWrite(_mosi, 0);
  this->digitalWrite(_sck, 0);
  this->digitalWrite(_sck, 1);

  uint8_t bit = 0x80;
  while (bit)
  {
    if (c & bit)
    {
      this->digitalWrite(_mosi, 1);
    }
    else
    {
      this->digitalWrite(_mosi, 0);
    }
    this->digitalWrite(_sck, 0);
    bit >>= 1;
    this->digitalWrite(_sck, 1);
  }
}

void Arduino_XCA9554SWSPI::writeCommand16(uint16_t)
{
}

void Arduino_XCA9554SWSPI::write(uint8_t d)
{
  // D/C bit, data
  this->digitalWrite(_mosi, 1);
  this->digitalWrite(_sck, 0);
  this->digitalWrite(_sck, 1);

  uint8_t bit = 0x80;
  while (bit)
  {
    if (d & bit)
    {
      this->digitalWrite(_mosi, 1);
    }
    else
    {
      this->digitalWrite(_mosi, 0);
    }
    this->digitalWrite(_sck, 0);
    bit >>= 1;
    this->digitalWrite(_sck, 1);
  }
}

void Arduino_XCA9554SWSPI::write16(uint16_t)
{
  // not implemented
}

void Arduino_XCA9554SWSPI::writeRepeat(uint16_t, uint32_t)
{
  // not implemented
}

void Arduino_XCA9554SWSPI::writePixels(uint16_t *, uint32_t)
{
  // not implemented
}

#if !defined(LITTLE_FOOT_PRINT)
void Arduino_XCA9554SWSPI::writeBytes(uint8_t *data, uint32_t len)
{
  // not implemented
}
#endif // !defined(LITTLE_FOOT_PRINT)

void Arduino_XCA9554SWSPI::writeRegister(uint8_t reg, uint8_t *data, size_t len)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  for (size_t i = 0; i < len; i++)
  {
    _wire->write(data[i]);
  }
  _wire->endTransmission();
}

uint8_t Arduino_XCA9554SWSPI::readRegister(uint8_t reg, uint8_t *data, size_t len)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->endTransmission();
  _wire->requestFrom(_address, len);
  size_t index = 0;
  while (index < len)
    data[index++] = _wire->read();
  return 0;
}

void Arduino_XCA9554SWSPI::pinMode(uint8_t pin, uint8_t mode)
{
  if (is_found)
  {
    uint8_t port = 0;
    this->readRegister(XCA9554_CONFIG_PORT_REG, &port, 1);
    if (mode == OUTPUT)
      {
        port = port & (~(1 << pin));
      }
    else
      {
        port = port | (1 << pin);
      }
    this->writeRegister(XCA9554_CONFIG_PORT_REG, &port, 1);
  }
  else
  {
    Serial.println("xCA9554 not found");
  }
}

void Arduino_XCA9554SWSPI::digitalWrite(uint8_t pin, uint8_t val)
{
  if (is_found)
  {
    uint8_t port = 0;
    uint8_t reg_data = 0;

    this->readRegister(XCA9554_OUTPUT_PORT_REG, &reg_data, 1);
    reg_data = reg_data & (~(1 << pin));
    port = reg_data | val << pin;
    this->writeRegister(XCA9554_OUTPUT_PORT_REG, &port, 1);
  }
  else
  {
    Serial.println("xCA9554 not found");
  }
}

int Arduino_XCA9554SWSPI::digitalRead(uint8_t pin)
{
  if (is_found)
  {
    int state = 0;
    uint8_t port = 0;
    this->readRegister(XCA9554_INPUT_PORT_REG, &port, 1);
    state = port & pin ? 1 : 0;
    return state;
  }
  else
  {
    Serial.println("xCA9554 not found");
  }
  return 0;
}
