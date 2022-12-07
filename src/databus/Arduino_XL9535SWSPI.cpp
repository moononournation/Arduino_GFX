#include "Arduino_XL9535SWSPI.h"

Arduino_XL9535SWSPI::Arduino_XL9535SWSPI(int8_t sda, int8_t scl, int8_t pwd, int8_t cs, int8_t sck, int8_t mosi)
    : _sda(sda), _scl(scl), _pwd(pwd), _cs(cs), _sck(sck), _mosi(mosi)
{
}

void Arduino_XL9535SWSPI::begin(int32_t speed, int8_t dataMode)
{
  if (speed == GFX_NOT_DEFINED)
  {
    speed = 800000L;
  }
  Wire.begin(_sda, _scl, speed);
  _address = XL9535_IIC_ADDRESS | (A2 << 3) | (A1 << 2) | (A0 << 1);
  Wire.beginTransmission(_address);
  if (!Wire.endTransmission()) {
    is_found = true;
    this->digitalWrite(_pwd, 0);
  } else {
    is_found = false;
  }

  UNUSED(dataMode);
}

void Arduino_XL9535SWSPI::beginWrite()
{
  this->digitalWrite(_cs, 0);
}

void Arduino_XL9535SWSPI::endWrite()
{
  this->digitalWrite(_cs, 1);
}

void Arduino_XL9535SWSPI::writeCommand(uint8_t c)
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

void Arduino_XL9535SWSPI::writeCommand16(uint16_t)
{
}

void Arduino_XL9535SWSPI::write(uint8_t d)
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

void Arduino_XL9535SWSPI::write16(uint16_t)
{
  // not implemented
}

void Arduino_XL9535SWSPI::writeRepeat(uint16_t p, uint32_t len)
{
  // not implemented
}

void Arduino_XL9535SWSPI::writePixels(uint16_t *data, uint32_t len)
{
  // not implemented
}

void Arduino_XL9535SWSPI::writeBytes(uint8_t *data, uint32_t len)
{
  // not implemented
}

void Arduino_XL9535SWSPI::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  // not implemented
}

void Arduino_XL9535SWSPI::writeRegister(uint8_t reg, uint8_t *data, uint8_t len) {
  Wire.beginTransmission(_address);
  Wire.write(reg);
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
}
uint8_t Arduino_XL9535SWSPI::readRegister(uint8_t reg, uint8_t *data, uint8_t len) {
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(_address, len);
  uint8_t index = 0;
  while (index < len)
    data[index++] = Wire.read();
  return 0;
}

void Arduino_XL9535SWSPI::pinMode(uint8_t pin, uint8_t mode) {
  if (is_found) {
    uint8_t port = 0;
    if (pin > 7) {
      readRegister(XL9535_CONFIG_PORT_1_REG, &port, 1);
      if (mode == OUTPUT) {
        port = port & (~(1 << (pin - 10)));
      } else {
        port = port | (1 << (pin - 10));
      }
      writeRegister(XL9535_CONFIG_PORT_1_REG, &port, 1);

    } else {
      readRegister(XL9535_CONFIG_PORT_0_REG, &port, 1);
      if (mode == OUTPUT) {
        port = port & (~(1 << pin));
      } else {
        port = port | (1 << pin);
      }
      writeRegister(XL9535_CONFIG_PORT_0_REG, &port, 1);
    }
  } else {
    // Serial.println("xl9535 not found");
  }
}
void Arduino_XL9535SWSPI::pinMode8(uint8_t port, uint8_t pin, uint8_t mode) {
  if (is_found) {
    uint8_t _pin = (mode != OUTPUT) ? pin : ~pin;
    if (port) {
      writeRegister(XL9535_CONFIG_PORT_1_REG, &_pin, 1);
    } else {
      writeRegister(XL9535_CONFIG_PORT_0_REG, &_pin, 1);
    }
  } else {
    // Serial.println("xl9535 not found");
  }
}

void Arduino_XL9535SWSPI::digitalWrite(uint8_t pin, uint8_t val) {
  if (is_found) {
    uint8_t port = 0;
    uint8_t reg_data = 0;
    if (pin > 7) {
      readRegister(XL9535_OUTPUT_PORT_1_REG, &reg_data, 1);
      reg_data = reg_data & (~(1 << (pin - 10)));
      port = reg_data | val << (pin - 10);
      writeRegister(XL9535_OUTPUT_PORT_1_REG, &port, 1);
    } else {
      readRegister(XL9535_OUTPUT_PORT_0_REG, &reg_data, 1);
      reg_data = reg_data & (~(1 << pin));
      port = reg_data | val << pin;
      writeRegister(XL9535_OUTPUT_PORT_0_REG, &port, 1);
    }
  } else {
    // Serial.println("xl9535 not found");
  }
}

int Arduino_XL9535SWSPI::digitalRead(uint8_t pin) {
  if (is_found) {
    int state = 0;
    uint8_t port = 0;
    if (pin > 7) {
      readRegister(XL9535_INPUT_PORT_1_REG, &port, 1);
      state = port & (pin - 10) ? 1 : 0;
    } else {
      readRegister(XL9535_INPUT_PORT_0_REG, &port, 1);
      state = port & pin ? 1 : 0;
    }
    return state;
  } else {
    // Serial.println("xl9535 not found");
  }
  return 0;
}

void Arduino_XL9535SWSPI::read_all_reg() {
  uint8_t data;
  for (uint8_t i = 0; i < 8; i++) {
    readRegister(i, &data, 1);
    // Serial.printf("0x%02x : 0x%02X \r\n", i, data);
  }
}
