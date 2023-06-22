#include "Arduino_ESP32QSPI.h"

#if defined(ESP32)

Arduino_ESP32QSPI::Arduino_ESP32QSPI(
    int8_t cs, int8_t sck, int8_t mosi, int8_t miso, int8_t quadwp, int8_t quadhd)
    : _cs(cs), _sck(sck), _mosi(mosi), _miso(miso), _quadwp(quadwp), _quadhd(quadhd)
{
}

bool Arduino_ESP32QSPI::begin(int32_t speed, int8_t dataMode)
{
  // set SPI parameters
  _speed = (speed == GFX_NOT_DEFINED) ? QSPI_FREQUENCY : speed;
  _dataMode = (dataMode == GFX_NOT_DEFINED) ? QSPI_SPI_MODE : dataMode;

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH); // disable chip select
#if (CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3)
  if (_cs >= 32)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    _csPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else
#endif
      if (_cs != GFX_NOT_DEFINED)
  {
    _csPinMask = digitalPinToBitMask(_cs);
    _csPortSet = (PORTreg_t)&GPIO.out_w1ts;
    _csPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }

  spi_bus_config_t buscfg = {
      .mosi_io_num = _mosi,
      .miso_io_num = _miso,
      .sclk_io_num = _sck,
      .quadwp_io_num = _quadwp,
      .quadhd_io_num = _quadhd,
      .max_transfer_sz = (SEND_BUF_SIZE * 16) + 8,
      .flags = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_GPIO_PINS};
  esp_err_t ret = spi_bus_initialize(QSPI_SPI_HOST, &buscfg, QSPI_DMA_CHANNEL);
  if (ret != ESP_OK)
  {
    ESP_ERROR_CHECK(ret);
    return false;
  }

  spi_device_interface_config_t devcfg = {
      .command_bits = 8,
      .address_bits = 24,
      .mode = QSPI_SPI_MODE,
      .clock_speed_hz = QSPI_FREQUENCY,
      .spics_io_num = -1,
      .flags = SPI_DEVICE_HALFDUPLEX,
      .queue_size = 17};
  ret = spi_bus_add_device(QSPI_SPI_HOST, &devcfg, &_handle);
  if (ret != ESP_OK)
  {
    ESP_ERROR_CHECK(ret);
    return false;
  }

  _send_buf = (uint16_t *)heap_caps_malloc(SEND_BUF_SIZE, MALLOC_CAP_INTERNAL);
  if (!_send_buf)
  {
    log_e("_send_buf malloc failed!");
    return false;
  }

  memset(&_spi_tran_ext, 0, sizeof(_spi_tran_ext));
  _spi_tran = (spi_transaction_t *)&_spi_tran_ext;

  return true;
}

void Arduino_ESP32QSPI::beginWrite()
{
  spi_device_acquire_bus(_handle, portMAX_DELAY);
}

void Arduino_ESP32QSPI::endWrite()
{
  spi_device_release_bus(_handle);
}

void Arduino_ESP32QSPI::writeCommand(uint8_t c)
{
  CS_LOW();
  _spi_tran_ext.base.flags = SPI_TRANS_MULTILINE_CMD | SPI_TRANS_MULTILINE_ADDR;
  _spi_tran_ext.base.cmd = 0x02;
  _spi_tran_ext.base.addr = ((uint32_t)c) << 8;
  _spi_tran_ext.base.tx_buffer = NULL;
  _spi_tran_ext.base.length = 0;
  spi_device_polling_transmit(_handle, _spi_tran);
  CS_HIGH();
}

void Arduino_ESP32QSPI::writeCommand16(uint16_t c)
{
  CS_LOW();
  _spi_tran_ext.base.flags = SPI_TRANS_MULTILINE_CMD | SPI_TRANS_MULTILINE_ADDR;
  _spi_tran_ext.base.cmd = 0x02;
  _spi_tran_ext.base.addr = c;
  _spi_tran_ext.base.tx_buffer = NULL;
  _spi_tran_ext.base.length = 0;
  spi_device_polling_transmit(_handle, _spi_tran);
  CS_HIGH();
}

void Arduino_ESP32QSPI::write(uint8_t d)
{
  CS_LOW();
  _spi_tran_ext.base.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_MODE_QIO;
  _spi_tran_ext.base.cmd = 0x32;
  _spi_tran_ext.base.addr = 0x003C00;
  _spi_tran_ext.base.tx_data[0] = d;
  _spi_tran_ext.base.length = 8;
  spi_device_polling_transmit(_handle, _spi_tran);
  CS_HIGH();
}

void Arduino_ESP32QSPI::write16(uint16_t d)
{
  CS_LOW();
  _spi_tran_ext.base.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_MODE_QIO;
  _spi_tran_ext.base.cmd = 0x32;
  _spi_tran_ext.base.addr = 0x003C00;
  _spi_tran_ext.base.tx_data[0] = d >> 8;
  _spi_tran_ext.base.tx_data[1] = d;
  _spi_tran_ext.base.length = 16;
  spi_device_polling_transmit(_handle, _spi_tran);
  CS_HIGH();
}

void Arduino_ESP32QSPI::writeC8D8(uint8_t c, uint8_t d)
{
  CS_LOW();
  _spi_tran_ext.base.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_MULTILINE_CMD | SPI_TRANS_MULTILINE_ADDR;
  _spi_tran_ext.base.cmd = 0x02;
  _spi_tran_ext.base.addr = ((uint32_t)c) << 8;
  _spi_tran_ext.base.tx_data[0] = d;
  _spi_tran_ext.base.length = 8;
  spi_device_polling_transmit(_handle, _spi_tran);
  CS_HIGH();
}

void Arduino_ESP32QSPI::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  CS_LOW();
  _spi_tran_ext.base.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_MULTILINE_CMD | SPI_TRANS_MULTILINE_ADDR;
  _spi_tran_ext.base.cmd = 0x02;
  _spi_tran_ext.base.addr = ((uint32_t)c) << 8;
  _spi_tran_ext.base.tx_data[0] = d1 >> 8;
  _spi_tran_ext.base.tx_data[1] = d1;
  _spi_tran_ext.base.tx_data[2] = d2 >> 8;
  _spi_tran_ext.base.tx_data[3] = d2;
  _spi_tran_ext.base.length = 32;
  spi_device_polling_transmit(_handle, _spi_tran);
  CS_HIGH();
}

void Arduino_ESP32QSPI::writeRepeat(uint16_t p, uint32_t len)
{
  MSB_16_SET(p, p);
  bool first_send = 1;
  uint32_t l = (len > (SEND_BUF_SIZE / 2)) ? (SEND_BUF_SIZE / 2) : len;
  for (int i = 0; i < l; ++i)
  {
    _send_buf[i] = p;
  }

  CS_LOW();
  while (len)
  {
    if (len < l)
    {
      l = len;
    }

    if (first_send)
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO;
      _spi_tran_ext.base.cmd = 0x32;
      _spi_tran_ext.base.addr = 0x003C00;
      first_send = 0;
    }
    else
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD |
                                 SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY;
    }
    _spi_tran_ext.base.tx_buffer = _send_buf;
    _spi_tran_ext.base.length = l * 16;

    spi_device_polling_transmit(_handle, _spi_tran);
    len -= l;
  }
  CS_HIGH();
}

void Arduino_ESP32QSPI::writePixels(uint16_t *data, uint32_t len)
{
  // log_i("Arduino_ESP32QSPI::writePixels(%d)", len);

  bool first_send = 1;
  uint32_t l = (len > (SEND_BUF_SIZE / 2)) ? (SEND_BUF_SIZE / 2) : len;

  CS_LOW();
  uint16_t p;
  while (len)
  {
    if (len < l)
    {
      l = len;
    }

    if (first_send)
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO;
      _spi_tran_ext.base.cmd = 0x32;
      _spi_tran_ext.base.addr = 0x003C00;
      first_send = 0;
    }
    else
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD |
                                 SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY;
    }
    for (uint32_t i = 0; i < l; ++i)
    {
      p = *data++;
      MSB_16_SET(_send_buf[i], p);
    }

    _spi_tran_ext.base.tx_buffer = _send_buf;
    _spi_tran_ext.base.length = l * 16;

    spi_device_polling_transmit(_handle, _spi_tran);
    len -= l;
  }
  CS_HIGH();
}

void Arduino_ESP32QSPI::writeBytes(uint8_t *data, uint32_t len)
{
  bool first_send = 1;
  uint32_t l = (len > (SEND_BUF_SIZE)) ? (SEND_BUF_SIZE) : len;

  CS_LOW();
  while (len)
  {
    if (len < l)
    {
      l = len;
    }

    if (first_send)
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO;
      _spi_tran_ext.base.cmd = 0x32;
      _spi_tran_ext.base.addr = 0x003C00;
      first_send = 0;
    }
    else
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD |
                                 SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY;
    }

    _spi_tran_ext.base.tx_buffer = data;
    _spi_tran_ext.base.length = l * 8;

    spi_device_polling_transmit(_handle, _spi_tran);
    len -= l;
    data += l;
  }
  CS_HIGH();
}

void Arduino_ESP32QSPI::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  while (repeat--)
  {
    writeBytes(data, len);
  }
}

void Arduino_ESP32QSPI::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  // log_i("Arduino_ESP32QSPI::writeIndexedPixels(%d)", len);

  bool first_send = 1;
  uint32_t l = (len > (SEND_BUF_SIZE / 2)) ? (SEND_BUF_SIZE / 2) : len;

  CS_LOW();
  uint16_t p;
  while (len)
  {
    if (len < l)
    {
      l = len;
    }

    if (first_send)
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO;
      _spi_tran_ext.base.cmd = 0x32;
      _spi_tran_ext.base.addr = 0x003C00;
      first_send = 0;
    }
    else
    {
      _spi_tran_ext.base.flags = SPI_TRANS_MODE_QIO | SPI_TRANS_VARIABLE_CMD |
                                 SPI_TRANS_VARIABLE_ADDR | SPI_TRANS_VARIABLE_DUMMY;
    }
    for (uint32_t i = 0; i < l; ++i)
    {
      p = idx[*data++];
      MSB_16_SET(_send_buf[i], p);
    }

    _spi_tran_ext.base.tx_buffer = _send_buf;
    _spi_tran_ext.base.length = l * 16;

    spi_device_polling_transmit(_handle, _spi_tran);
    len -= l;
  }
  CS_HIGH();
}

/******** low level bit twiddling **********/

INLINE void Arduino_ESP32QSPI::CS_HIGH(void)
{
  *_csPortSet = _csPinMask;
}

INLINE void Arduino_ESP32QSPI::CS_LOW(void)
{
  *_csPortClr = _csPinMask;
}

#endif // #if defined(ESP32)
