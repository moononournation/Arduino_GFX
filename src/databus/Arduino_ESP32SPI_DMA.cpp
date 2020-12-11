/*
 * start rewrite from:
 * https://github.com/espressif/arduino-esp32.git
 */
#ifdef ESP32
#include "Arduino_DataBus.h"
#include "Arduino_ESP32SPI_DMA.h"

Arduino_ESP32SPI_DMA::Arduino_ESP32SPI_DMA(int8_t dc /* = -1 */, int8_t cs /* = -1 */, int8_t sck /* = -1 */, int8_t mosi /* = -1 */, int8_t miso /* = -1 */, uint8_t spi_num /* = VSPI */, bool is_shared_interface /* = true */)
    : _dc(dc), _spi_num(spi_num), _is_shared_interface(is_shared_interface)
{
  if (sck == -1 && miso == -1 && mosi == -1 && cs == -1)
  {
    _sck = (_spi_num == VSPI) ? SCK : 14;
    _miso = (_spi_num == VSPI) ? MISO : 12;
    _mosi = (_spi_num == VSPI) ? MOSI : 13;
    _cs = (_spi_num == VSPI) ? SS : 15;
  }
  else
  {
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
    _cs = cs;
  }
}

void Arduino_ESP32SPI_DMA::begin(int32_t speed, int8_t dataMode)
{
  _speed = speed ? speed : SPI_DEFAULT_FREQ;
  _dataMode = dataMode;

  if (!_div)
  {
    _div = spiFrequencyToClockDiv(_speed);
  }

  if (_dc >= 0)
  {
    pinMode(_dc, OUTPUT);
    digitalWrite(_dc, HIGH); // Data mode
  }
  if (_dc >= 32)
  {
    dcPinMask = digitalPinToBitMask(_dc);
    dcPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    dcPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else if (_dc >= 0)
  {
    dcPinMask = digitalPinToBitMask(_dc);
    dcPortSet = (PORTreg_t)&GPIO.out_w1ts;
    dcPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }

  if (_cs >= 0)
  {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH); // disable chip select
  }
  if (_cs >= 32)
  {
    csPinMask = digitalPinToBitMask(_cs);
    csPortSet = (PORTreg_t)&GPIO.out1_w1ts.val;
    csPortClr = (PORTreg_t)&GPIO.out1_w1tc.val;
  }
  else if (_cs >= 0)
  {
    csPinMask = digitalPinToBitMask(_cs);
    csPortSet = (PORTreg_t)&GPIO.out_w1ts;
    csPortClr = (PORTreg_t)&GPIO.out_w1tc;
  }
  else
  {
    csPinMask = 0;
    csPortSet = dcPortSet;
    csPortClr = dcPortClr;
  }

  spi_bus_config_t buscfg = {
      .mosi_io_num = _mosi,
      .miso_io_num = _miso,
      .sclk_io_num = _sck,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = MAX_TRANSFER_SZ,
  };
  spi_device_interface_config_t devcfg = {
      .command_bits = 0,
      .address_bits = 0,
      .dummy_bits = 0,
      .mode = (uint8_t)_dataMode,
      .duty_cycle_pos = 128,
      .cs_ena_pretrans = 0,
      .cs_ena_posttrans = 0,
      .clock_speed_hz = _speed,
      .input_delay_ns = 0,
      .spics_io_num = -1, // avoid use system CS control
      .flags = (_miso < 0) ? (uint32_t)SPI_DEVICE_NO_DUMMY : 0,
      .queue_size = 1,
  };
  esp_err_t ret;

  ret = spi_bus_initialize((spi_host_device_t)(_spi_num - 1), &buscfg, DMA_CHANNEL);
  if (ret != ESP_OK)
  {
    log_e("Arduino_ESP32SPI_DMA::begin() spi_bus_initialize error: %d", ret);
  }

  ret = spi_bus_add_device((spi_host_device_t)(_spi_num - 1), &devcfg, &_handle);
  if (ret != ESP_OK)
  {
    log_e("Arduino_ESP32SPI_DMA::begin() spi_bus_add_device error: %d", ret);
  }

  data_buf = (char *)heap_caps_malloc((MAX_TRANSFER_SZ / 8), MALLOC_CAP_DMA);
  if (!data_buf)
  {
    log_e("Allocation failed!");
  }
  else
  {
    data_buf16 = (uint16_t *)data_buf;
    data_buf32 = (uint32_t *)data_buf;
  }

  if (!_is_shared_interface)
  {
    spi_device_acquire_bus(_handle, portMAX_DELAY);
  }
}

void Arduino_ESP32SPI_DMA::beginWrite()
{
  data_buf_bit_idx = 0;
  data_buf[0] = 0;

  if (_is_shared_interface)
  {
    spi_device_acquire_bus(_handle, portMAX_DELAY);
  }

  if (_dc >= 0)
  {
    DC_HIGH();
  }
  CS_LOW();
}

void Arduino_ESP32SPI_DMA::writeCommand(uint8_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(c);
  }
  else
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    DC_LOW();

    spi_transaction_t ct;
    memset(&ct, 0, sizeof(ct));
    ct.length = 8;
    ct.tx_data[0] = c;
    ct.flags = SPI_TRANS_USE_TXDATA;

    esp_err_t ret = spi_device_polling_transmit(_handle, &ct);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }

    DC_HIGH();
  }
}

void Arduino_ESP32SPI_DMA::writeCommand16(uint16_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(c >> 8);
    WRITE9BIT(c & 0xff);
  }
  else
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    DC_LOW();

    spi_transaction_t ct;
    memset(&ct, 0, sizeof(ct));
    ct.length = 16;
    MSB_16_SET(ct.tx_data[0], c);
    ct.flags = SPI_TRANS_USE_TXDATA;

    esp_err_t ret = spi_device_polling_transmit(_handle, &ct);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }

    DC_HIGH();
  }
}

void Arduino_ESP32SPI_DMA::writeCommand32(uint32_t c)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(c >> 24);
    WRITE9BIT((c >> 16) & 0xff);
    WRITE9BIT((c >> 8) & 0xff);
    WRITE9BIT(c & 0xff);
  }
  else
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    DC_LOW();

    spi_transaction_t ct;
    memset(&ct, 0, sizeof(ct));
    ct.length = 32;
    MSB_32_SET(ct.tx_data[0], c);
    ct.flags = SPI_TRANS_USE_TXDATA;

    esp_err_t ret = spi_device_polling_transmit(_handle, &ct);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }

    DC_HIGH();
  }
}

void Arduino_ESP32SPI_DMA::write(uint8_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(0x100 | d);
  }
  else
  {
    WRITE8BIT(d);
  }
}

void Arduino_ESP32SPI_DMA::write16(uint16_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(0x100 | (d >> 8));
    WRITE9BIT(0x100 | (d & 0xff));
  }
  else
  {
    WRITE8BIT(d >> 8);
    WRITE8BIT(d);
  }
}

void Arduino_ESP32SPI_DMA::write32(uint32_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(0x100 | (d >> 24));
    WRITE9BIT(0x100 | ((d >> 16) & 0xff));
    WRITE9BIT(0x100 | ((d >> 8) & 0xff));
    WRITE9BIT(0x100 | (d & 0xff));
  }
  else
  {
    WRITE8BIT(d >> 24);
    WRITE8BIT(d >> 16);
    WRITE8BIT(d >> 8);
    WRITE8BIT(d);
  }
}

void Arduino_ESP32SPI_DMA::writeC8D8(uint8_t c, uint8_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(c);
    WRITE9BIT(0x100 | d);
  }
  else
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    DC_LOW();

    spi_transaction_t ct;
    memset(&ct, 0, sizeof(ct));
    ct.length = 8;
    ct.tx_data[0] = c;
    ct.flags = SPI_TRANS_USE_TXDATA;

    esp_err_t ret = spi_device_polling_transmit(_handle, &ct);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }

    DC_HIGH();

    spi_transaction_t dt;
    memset(&dt, 0, sizeof(dt));
    dt.length = 8;
    dt.tx_data[0] = d;
    dt.flags = SPI_TRANS_USE_TXDATA;

    ret = spi_device_polling_transmit(_handle, &dt);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }
  }
}

void Arduino_ESP32SPI_DMA::writeC8D16(uint8_t c, uint16_t d)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(c);
    WRITE9BIT(0x100 | (d >> 8));
    WRITE9BIT(0x100 | (d & 0xff));
  }
  else
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    DC_LOW();

    spi_transaction_t ct;
    memset(&ct, 0, sizeof(ct));
    ct.length = 8;
    ct.tx_data[0] = c;
    ct.flags = SPI_TRANS_USE_TXDATA;

    esp_err_t ret = spi_device_polling_transmit(_handle, &ct);
    if (ret != ESP_OK)
    {
      log_e("Arduino_ESP32SPI_DMA::write() spi_device_polling_transmit error: %d", ret);
    }

    DC_HIGH();

    spi_transaction_t dt;
    memset(&dt, 0, sizeof(dt));
    dt.length = 16;
    dt.tx_data[0] = (d >> 8);
    dt.tx_data[1] = (d & 0xff);
    dt.flags = SPI_TRANS_USE_TXDATA;

    ret = spi_device_polling_transmit(_handle, &dt);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }
  }
}

void Arduino_ESP32SPI_DMA::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  if (_dc < 0) // 9-bit SPI
  {
    WRITE9BIT(c);
    WRITE9BIT(0x100 | (d1 >> 8));
    WRITE9BIT(0x100 | (d1 & 0xff));
    WRITE9BIT(0x100 | (d2 >> 8));
    WRITE9BIT(0x100 | (d2 & 0xff));
  }
  else
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    DC_LOW();

    spi_transaction_t ct;
    memset(&ct, 0, sizeof(ct));
    ct.length = 8;
    ct.tx_data[0] = c;
    ct.flags = SPI_TRANS_USE_TXDATA;

    esp_err_t ret = spi_device_polling_transmit(_handle, &ct);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }

    DC_HIGH();

    spi_transaction_t dt;
    memset(&dt, 0, sizeof(dt));
    dt.length = 32;
    dt.tx_data[0] = (d1 >> 8);
    dt.tx_data[1] = (d1 & 0xff);
    dt.tx_data[2] = (d2 >> 8);
    dt.tx_data[3] = (d2 & 0xff);
    dt.flags = SPI_TRANS_USE_TXDATA;

    ret = spi_device_polling_transmit(_handle, &dt);
    if (ret != ESP_OK)
    {
      log_e("spi_device_polling_transmit error: %d", ret);
    }
  }
}

void Arduino_ESP32SPI_DMA::endWrite()
{
  if (data_buf_bit_idx > 0)
  {
    flush_data_buf();
  }

  if (_is_shared_interface)
  {
    spi_device_release_bus(_handle);
  }

  CS_HIGH();
}

void Arduino_ESP32SPI_DMA::sendCommand(uint8_t c)
{
  beginWrite();

  writeCommand(c);

  endWrite();
}

void Arduino_ESP32SPI_DMA::sendCommand16(uint16_t c)
{
  beginWrite();

  writeCommand16(c);

  endWrite();
}

void Arduino_ESP32SPI_DMA::sendCommand32(uint32_t c)
{
  beginWrite();

  writeCommand32(c);

  endWrite();
}

void Arduino_ESP32SPI_DMA::sendData(uint8_t d)
{
  beginWrite();

  write(d);

  endWrite();
}

void Arduino_ESP32SPI_DMA::sendData16(uint16_t d)
{
  beginWrite();

  write16(d);

  endWrite();
}

void Arduino_ESP32SPI_DMA::sendData32(uint32_t d)
{
  beginWrite();

  write32(d);

  endWrite();
}

void Arduino_ESP32SPI_DMA::writeRepeat(uint16_t p, uint32_t len)
{
  if (data_buf_bit_idx > 0)
  {
    flush_data_buf();
  }

  if (_dc < 0) // 9-bit SPI
  {
    uint32_t hi = 0x100 | (p >> 8);
    uint32_t lo = 0x100 | (p & 0xff);
    uint32_t idx, shift;
    uint32_t bufLen = (len <= 28) ? len : 28;
    uint32_t xferLen;
    for (uint32_t t = 0; t < bufLen; t++)
    {
      idx = data_buf_bit_idx >> 3;
      shift = (data_buf_bit_idx % 8);
      if (shift)
      {
        data_buf[idx++] |= hi >> (shift + 1);
        data_buf[idx] = hi << (7 - shift);
      }
      else
      {
        data_buf[idx++] = hi >> 1;
        data_buf[idx] = hi << 7;
      }
      data_buf_bit_idx += 9;

      idx = data_buf_bit_idx >> 3;
      shift = (data_buf_bit_idx % 8);
      if (shift)
      {
        data_buf[idx++] |= lo >> (shift + 1);
        data_buf[idx] = lo << (7 - shift);
      }
      else
      {
        data_buf[idx++] = lo >> 1;
        data_buf[idx] = lo << 7;
      }
      data_buf_bit_idx += 9;
    }

    // Issue pixels in blocks from temp buffer
    while (len)
    {                                          // While pixels remain
      xferLen = (bufLen < len) ? bufLen : len; // How many this pass?

      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = xferLen * 18;
      t.tx_buffer = data_buf;

      esp_err_t ret = spi_device_polling_transmit(_handle, &t);
      if (ret != ESP_OK)
      {
        log_e("spi_device_queue_trans error: %d", ret);
      }

      len -= xferLen;
    }
  }
  else // 8-bit SPI
  {
    uint32_t bufLen = (len < (MAX_TRANSFER_SZ / 16)) ? len : (MAX_TRANSFER_SZ / 16);
    uint32_t xferLen;
    uint32_t c32;
    MSB_32_16_16_SET(c32, p, p);

    uint32_t l = (bufLen + 1) / 2;
    for (uint32_t i = 0; i < l; i++)
    {
      data_buf32[i] = c32;
    }

    // Issue pixels in blocks from temp buffer
    while (len) // While pixels remain
    {
      xferLen = (bufLen <= len) ? bufLen : len; // How many this pass?

      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = xferLen * 16;
      t.tx_buffer = data_buf;

      esp_err_t ret = spi_device_polling_transmit(_handle, &t);
      if (ret != ESP_OK)
      {
        log_e("spi_device_queue_trans error: %d", ret);
      }

      len -= xferLen;
    }
  }
}

void Arduino_ESP32SPI_DMA::writeBytes(uint8_t *data, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
    while (len--)
    {
      write(*data++);
    }
  }
  else // 8-bit SPI
  {
    if (esp_ptr_dma_capable(data))
    {
      if (data_buf_bit_idx > 0)
      {
        flush_data_buf();
      }
      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = len << 3;
      t.tx_buffer = data;
      esp_err_t ret = spi_device_polling_transmit(_handle, &t);
      if (ret != ESP_OK)
      {
        log_e("spi_device_queue_trans error: %d", ret);
      }
    }
    else
    {
      uint32_t *p = (uint32_t *)data;
      uint32_t bufLen = (len < (MAX_TRANSFER_SZ / 8)) ? len : (MAX_TRANSFER_SZ / 8);

      if (len >= bufLen)
      {
        if (data_buf_bit_idx > 0)
        {
          flush_data_buf();
        }

        spi_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length = bufLen << 3;
        t.tx_buffer = data_buf;
        uint32_t l = bufLen >> 2;
        while (len >= bufLen)
        {
          for (uint32_t i = 0; i < l; i++)
          {
            data_buf32[i] = *p++;
          }
          esp_err_t ret = spi_device_polling_transmit(_handle, &t);
          if (ret != ESP_OK)
          {
            log_e("spi_device_queue_trans error: %d", ret);
          }

          len -= bufLen;
        }
      }

      if ((len > 0) && ((len % 4) == 0))
      {
        if (data_buf_bit_idx > 0)
        {
          flush_data_buf();
        }

        spi_transaction_t t;
        memset(&t, 0, sizeof(t));
        t.length = len << 3;
        t.tx_buffer = data_buf;
        uint32_t l = len >> 2;
        for (uint32_t i = 0; i < l; i++)
        {
          data_buf32[i] = *p++;
        }

        esp_err_t ret = spi_device_polling_transmit(_handle, &t);
        if (ret != ESP_OK)
        {
          log_e("spi_device_queue_trans error: %d", ret);
        }
      }
      else
      {
        while (len--)
        {
          write(*data++);
        }
      }
    }
  }
}

void Arduino_ESP32SPI_DMA::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
    while (len--)
    {
      write16(idx[*(data++)]);
    }
  }
  else // 8-bit SPI
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    uint32_t bufLen = (len < (MAX_TRANSFER_SZ / 16)) ? len : (MAX_TRANSFER_SZ / 16);
    uint32_t xferLen, p;

    while (len) // While pixels remain
    {
      xferLen = (bufLen <= len) ? bufLen : len; // How many this pass?

      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = xferLen << 4;
      t.tx_buffer = data_buf;
      for (uint32_t i = 0; i < xferLen; i++)
      {
        p = idx[*(data++)];
        MSB_16_SET(data_buf16[i], p);
      }

      esp_err_t ret = spi_device_polling_transmit(_handle, &t);
      if (ret != ESP_OK)
      {
        log_e("spi_device_queue_trans error: %d", ret);
      }

      len -= xferLen;
    }
  }
}

void Arduino_ESP32SPI_DMA::writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
    uint32_t l = len;
    uint8_t *d = data;
    uint16_t p;
    uint8_t hi, lo;
    while (l--)
    {
      p = idx[*(d++)];
      hi = p >> 8;
      lo = p;
      write(hi);
      write(lo);
      write(hi);
      write(lo);
    }
  }
  else // 8-bit SPI
  {
    if (data_buf_bit_idx > 0)
    {
      flush_data_buf();
    }

    uint32_t bufLen = (len < (MAX_TRANSFER_SZ / 16 / 2)) ? len : (MAX_TRANSFER_SZ / 16 / 2);
    uint32_t xferLen;
    uint16_t p;

    while (len) // While pixels remain
    {
      xferLen = (bufLen <= len) ? bufLen : len; // How many this pass?

      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = xferLen << 5;
      t.tx_buffer = data_buf;
      for (uint32_t i = 0; i < xferLen; i++)
      {
        p = idx[*(data++)];
        MSB_16_SET(p, p);
        data_buf16[(i * 2)] = p;
        data_buf16[(i * 2) + 1] = p;
      }

      esp_err_t ret = spi_device_polling_transmit(_handle, &t);
      if (ret != ESP_OK)
      {
        log_e("spi_device_queue_trans error: %d", ret);
      }

      len -= xferLen;
    }
  }
}

void Arduino_ESP32SPI_DMA::writePixels(uint16_t *data, uint32_t len)
{
  if (_dc < 0) // 9-bit SPI
  {
    while (len--)
    {
      write16(*data++);
    }
  }
  else // 8-bit SPI
  {
    uint32_t bufLen = (len < (MAX_TRANSFER_SZ / 16)) ? len : (MAX_TRANSFER_SZ / 16);
    uint16_t p;

    if (len >= bufLen)
    {
      if (data_buf_bit_idx > 0)
      {
        flush_data_buf();
      }

      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = bufLen << 4;
      t.tx_buffer = data_buf;
      while (len >= bufLen)
      {
        for (uint32_t i = 0; i < bufLen; i++)
        {
          p = *data++;
          MSB_16_SET(data_buf16[i], p);
        }
        esp_err_t ret = spi_device_polling_transmit(_handle, &t);
        if (ret != ESP_OK)
        {
          log_e("spi_device_queue_trans error: %d", ret);
        }

        len -= bufLen;
      }
    }

    if ((len > 0) && ((len % 2) == 0))
    {
      if (data_buf_bit_idx > 0)
      {
        flush_data_buf();
      }

      spi_transaction_t t;
      memset(&t, 0, sizeof(t));
      t.length = len << 4;
      t.tx_buffer = data_buf;
      for (uint32_t i = 0; i < len; i++)
      {
        p = *data++;
        MSB_16_SET(data_buf16[i], p);
      }

      esp_err_t ret = spi_device_polling_transmit(_handle, &t);
      if (ret != ESP_OK)
      {
        log_e("spi_device_queue_trans error: %d", ret);
      }
    }
    else
    {
      while (len--)
      {
        write16(*data++);
      }
    }
  }
}

/**
 * @param data uint8_t *
 * @param len uint8_t
 * @param repeat uint32_t
 */
void Arduino_ESP32SPI_DMA::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  while (repeat--)
  {
    writeBytes(data, len);
  }
}

void Arduino_ESP32SPI_DMA::flush_data_buf()
{
  spi_transaction_t t;
  memset(&t, 0, sizeof(t));
  t.length = data_buf_bit_idx;
  t.tx_buffer = data_buf;

  esp_err_t ret = spi_device_polling_transmit(_handle, &t);
  if (ret != ESP_OK)
  {
    log_e("spi_device_polling_transmit error: %d", ret);
  }

  data_buf_bit_idx = 0;
}

INLINE void Arduino_ESP32SPI_DMA::WRITE8BIT(uint8_t d)
{
  uint16_t idx = data_buf_bit_idx >> 3;
  data_buf[idx] = d;
  data_buf_bit_idx += 8;
  if (data_buf_bit_idx >= MAX_TRANSFER_SZ)
  {
    flush_data_buf();
  }
}

INLINE void Arduino_ESP32SPI_DMA::WRITE9BIT(uint32_t d)
{
  uint16_t idx = data_buf_bit_idx >> 3;
  uint8_t shift = (data_buf_bit_idx % 8);
  if (shift)
  {
    data_buf[idx++] |= d >> (shift + 1);
    data_buf[idx] = d << (7 - shift);
  }
  else
  {
    data_buf[idx++] = d >> 1;
    data_buf[idx] = d << 7;
  }
  data_buf_bit_idx += 9;
  if (data_buf_bit_idx >= 504) // 56 bytes * 9 bits
  {
    flush_data_buf();
  }
}

/******** low level bit twiddling **********/

INLINE void Arduino_ESP32SPI_DMA::DC_HIGH(void)
{
  *dcPortSet = dcPinMask;
}

INLINE void Arduino_ESP32SPI_DMA::DC_LOW(void)
{
  *dcPortClr = dcPinMask;
}

INLINE void Arduino_ESP32SPI_DMA::CS_HIGH(void)
{
  *csPortSet = csPinMask;
}

INLINE void Arduino_ESP32SPI_DMA::CS_LOW(void)
{
  *csPortClr = csPinMask;
}

#endif
