/*
 * start rewrite from:
 * https://github.com/espressif/arduino-esp32.git
 */
#ifdef ESP32
#ifndef _ARDUINO_ESP32SPI_H_
#define _ARDUINO_ESP32SPI_H_

#include "esp32-hal-spi.h"
#include "esp32-hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "rom/ets_sys.h"
#include "esp_attr.h"
#include "esp_intr.h"
#include "rom/gpio.h"
#include "soc/spi_reg.h"
#include "soc/spi_struct.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_sig_map.h"
#include "soc/dport_reg.h"
#include "soc/rtc.h"

typedef volatile uint32_t *PORTreg_t; ///< PORT register type

#include "Arduino_DataBus.h"

#define SPI_DEFAULT_FREQ 40000000

#define SPI_CLK_IDX(p) ((p == 0) ? SPICLK_OUT_IDX : ((p == 1) ? SPICLK_OUT_IDX : ((p == 2) ? HSPICLK_OUT_IDX : ((p == 3) ? VSPICLK_OUT_IDX : 0))))
#define SPI_MISO_IDX(p) ((p == 0) ? SPIQ_OUT_IDX : ((p == 1) ? SPIQ_OUT_IDX : ((p == 2) ? HSPIQ_OUT_IDX : ((p == 3) ? VSPIQ_OUT_IDX : 0))))
#define SPI_MOSI_IDX(p) ((p == 0) ? SPID_IN_IDX : ((p == 1) ? SPID_IN_IDX : ((p == 2) ? HSPID_IN_IDX : ((p == 3) ? VSPID_IN_IDX : 0))))

#define SPI_SPI_SS_IDX(n) ((n == 0) ? SPICS0_OUT_IDX : ((n == 1) ? SPICS1_OUT_IDX : ((n == 2) ? SPICS2_OUT_IDX : SPICS0_OUT_IDX)))
#define SPI_HSPI_SS_IDX(n) ((n == 0) ? HSPICS0_OUT_IDX : ((n == 1) ? HSPICS1_OUT_IDX : ((n == 2) ? HSPICS2_OUT_IDX : HSPICS0_OUT_IDX)))
#define SPI_VSPI_SS_IDX(n) ((n == 0) ? VSPICS0_OUT_IDX : ((n == 1) ? VSPICS1_OUT_IDX : ((n == 2) ? VSPICS2_OUT_IDX : VSPICS0_OUT_IDX)))
#define SPI_SS_IDX(p, n) ((p == 0) ? SPI_SPI_SS_IDX(n) : ((p == 1) ? SPI_SPI_SS_IDX(n) : ((p == 2) ? SPI_HSPI_SS_IDX(n) : ((p == 3) ? SPI_VSPI_SS_IDX(n) : 0))))

#define SPI_INUM(u) (2)
#define SPI_INTR_SOURCE(u) ((u == 0) ? ETS_SPI0_INTR_SOURCE : ((u == 1) ? ETS_SPI1_INTR_SOURCE : ((u == 2) ? ETS_SPI2_INTR_SOURCE : ((p == 3) ? ETS_SPI3_INTR_SOURCE : 0))))

#define MSB_32_SET(var, val)                                  \
  {                                                           \
    uint8_t *v = (uint8_t *)&(val);                           \
    (var) = v[3] | (v[2] << 8) | (v[1] << 16) | (v[0] << 24); \
  }
#define MSB_16_SET(var, val)                             \
  {                                                      \
    (var) = (((val)&0xFF00) >> 8) | (((val)&0xFF) << 8); \
  }

struct spi_struct_t
{
  spi_dev_t *dev;
#if !CONFIG_DISABLE_HAL_LOCKS
  xSemaphoreHandle lock;
#endif
  uint8_t num;
};

#if CONFIG_DISABLE_HAL_LOCKS
#define SPI_MUTEX_LOCK()
#define SPI_MUTEX_UNLOCK()

static spi_t _spi_bus_array[4] = {
    {(volatile spi_dev_t *)(DR_REG_SPI0_BASE), 0},
    {(volatile spi_dev_t *)(DR_REG_SPI1_BASE), 1},
    {(volatile spi_dev_t *)(DR_REG_SPI2_BASE), 2},
    {(volatile spi_dev_t *)(DR_REG_SPI3_BASE), 3}};
#else // !CONFIG_DISABLE_HAL_LOCKS
#define SPI_MUTEX_LOCK() \
  do                     \
  {                      \
  } while (xSemaphoreTake(_spi->lock, portMAX_DELAY) != pdPASS)
#define SPI_MUTEX_UNLOCK() xSemaphoreGive(_spi->lock)

static spi_t _spi_bus_array[4] = {
    {(volatile spi_dev_t *)(DR_REG_SPI0_BASE), NULL, 0},
    {(volatile spi_dev_t *)(DR_REG_SPI1_BASE), NULL, 1},
    {(volatile spi_dev_t *)(DR_REG_SPI2_BASE), NULL, 2},
    {(volatile spi_dev_t *)(DR_REG_SPI3_BASE), NULL, 3}};
#endif // CONFIG_DISABLE_HAL_LOCKS

class Arduino_ESP32SPI : public Arduino_DataBus
{
public:
  Arduino_ESP32SPI(int8_t dc = -1, int8_t cs = -1, int8_t sck = -1, int8_t mosi = -1, int8_t miso = -1, uint8_t spi_num = VSPI); // Constructor

  virtual void begin(uint32_t speed = 0, int8_t dataMode = SPI_MODE0);
  virtual void beginWrite();
  virtual void writeCommand(uint8_t);
  virtual void writeCommand16(uint16_t);
  virtual void write(uint8_t);
  virtual void write16(uint16_t);
  virtual void write32(uint32_t);
  virtual void writeC8D8(uint8_t c, uint8_t d);
  virtual void writeC8D16(uint8_t c, uint16_t d);
  virtual void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2);
  virtual void writeRepeat(uint16_t p, uint32_t len);
  virtual void writeBytes(uint8_t *data, uint32_t len);
  virtual void writePixels(uint16_t *data, uint32_t len);
  virtual void writePattern(uint8_t *data, uint8_t len, uint32_t repeat);
  virtual void endWrite();

  virtual void sendCommand(uint8_t);
  virtual void sendCommand16(uint16_t);
  virtual void sendData(uint8_t);
  virtual void sendData16(uint16_t);
  virtual void sendData32(uint32_t);

protected:
  virtual void write9bit(uint32_t c);
  virtual void flush_data_buf();

private:
  inline void CS_HIGH(void);
  inline void CS_LOW(void);
  inline void DC_HIGH(void);
  inline void DC_LOW(void);

  int8_t _dc, _cs, _sck, _mosi, _miso;
  uint8_t _spi_num;
  uint32_t _speed;
  uint32_t _div = 0;

  PORTreg_t dcPortSet; ///< PORT register for data/command SET
  PORTreg_t dcPortClr; ///< PORT register for data/command CLEAR
  PORTreg_t csPortSet; ///< PORT register for chip select SET
  PORTreg_t csPortClr; ///< PORT register for chip select CLEAR
  uint32_t dcPinMask;  ///< Bitmask for data/command
  uint32_t csPinMask;  ///< Bitmask for chip select

  spi_t *_spi;
  uint8_t _bitOrder;
  uint8_t data_buf[64] = {0};
  uint32_t *data_buf32 = (uint32_t *)&data_buf;
  uint16_t data_buf_bit_idx = 0;
};

#endif // _ARDUINO_ESP32SPI_H_
#endif // #ifdef ESP32
