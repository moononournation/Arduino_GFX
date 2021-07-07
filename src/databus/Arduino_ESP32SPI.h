/*
 * start rewrite from:
 * https://github.com/espressif/arduino-esp32.git
 */
#include "Arduino_DataBus.h"

#if (CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2)

#ifndef _ARDUINO_ESP32SPI_H_
#define _ARDUINO_ESP32SPI_H_

#include "soc/spi_struct.h"
#if CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/gpio.h"
#else
#include "soc/dport_reg.h"
#endif

#define SPI_MAX_PIXELS_AT_ONCE 32

#if CONFIG_IDF_TARGET_ESP32
#define MOSI_BIT_LEN _spi->dev->mosi_dlen.usr_mosi_dbitlen
#define MISO_BIT_LEN _spi->dev->miso_dlen.usr_miso_dbitlen
#elif CONFIG_IDF_TARGET_ESP32S2
#define MOSI_BIT_LEN _spi->dev->mosi_dlen.usr_mosi_bit_len
#define MISO_BIT_LEN _spi->dev->miso_dlen.usr_miso_bit_len
#elif CONFIG_IDF_TARGET_ESP32C3
#define MOSI_BIT_LEN _spi->dev->ms_dlen.ms_data_bitlen
#define MISO_BIT_LEN _spi->dev->ms_dlen.ms_data_bitlen
#endif

class Arduino_ESP32SPI : public Arduino_DataBus
{
public:
#if CONFIG_IDF_TARGET_ESP32
  Arduino_ESP32SPI(int8_t dc = -1, int8_t cs = -1, int8_t sck = -1, int8_t mosi = -1, int8_t miso = -1, uint8_t spi_num = VSPI, bool is_shared_interface = true); // Constructor
#else
  Arduino_ESP32SPI(int8_t dc = -1, int8_t cs = -1, int8_t sck = -1, int8_t mosi = -1, int8_t miso = -1, uint8_t spi_num = FSPI, bool is_shared_interface = true); // Constructor
#endif

  void begin(int32_t speed = 0, int8_t dataMode = SPI_MODE0) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;
  void writeRepeat(uint16_t p, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

  void writeC8D8(uint8_t c, uint8_t d) override;
  void writeC8D16(uint8_t c, uint16_t d) override;
  void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2) override;
  void writeBytes(uint8_t *data, uint32_t len) override;
  void writePattern(uint8_t *data, uint8_t len, uint32_t repeat) override;

  void writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len) override;
  void writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len) override;

protected:
  void flush_data_buf();

private:
  INLINE void WRITE8BIT(uint8_t c);
  INLINE void WRITE9BIT(uint32_t c);
  INLINE void DC_HIGH(void);
  INLINE void DC_LOW(void);
  INLINE void CS_HIGH(void);
  INLINE void CS_LOW(void);

  int8_t _dc, _cs;
  int8_t _sck, _mosi, _miso;
  uint8_t _spi_num;
  bool _is_shared_interface;
  int32_t _speed;
  uint32_t _div = 0;

  PORTreg_t _dcPortSet; ///< PORT register for data/command SET
  PORTreg_t _dcPortClr; ///< PORT register for data/command CLEAR
  PORTreg_t _csPortSet; ///< PORT register for chip select SET
  PORTreg_t _csPortClr; ///< PORT register for chip select CLEAR
  uint32_t _dcPinMask;  ///< Bitmask for data/command
  uint32_t _csPinMask;  ///< Bitmask for chip select

  spi_t *_spi;
  uint8_t _bitOrder = SPI_MSBFIRST;
  union
  {
    uint8_t _buffer[SPI_MAX_PIXELS_AT_ONCE * 2] = {0};
    uint32_t _buffer32[SPI_MAX_PIXELS_AT_ONCE / 2];
  };
  uint16_t _data_buf_bit_idx = 0;
};

#endif // _ARDUINO_ESP32SPI_H_

#endif // #if (CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2)
