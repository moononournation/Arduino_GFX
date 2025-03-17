/*******************************************************************************
 * ESP32_JPEG Wrapper Class
 *
 * Dependent libraries:
 * ESP32_JPEG: https://github.com/esp-arduino-libs/ESP32_JPEG.git
 ******************************************************************************/
#pragma once

#if defined(ESP32)
#include <driver/jpeg_decode.h>

#define READ_BATCH_SIZE 1024

class MjpegClass
{
public:
  bool setup(const char *path)
  {
    _input = fopen(path, "r");
    _read = 0;

    jpeg_decode_engine_cfg_t decode_eng_cfg = {
        .intr_priority = 0,
        .timeout_ms = 40,
    };
    ESP_ERROR_CHECK(jpeg_new_decoder_engine(&decode_eng_cfg, &_decoder_engine));

    jpeg_decode_cfg_t decode_cfg_rgb = {
        .output_format = JPEG_DECODE_OUT_FORMAT_RGB565,
        .rgb_order = JPEG_DEC_RGB_ELEMENT_ORDER_BGR,
    };

    size_t tx_buffer_size;
    size_t rx_buffer_size;

    jpeg_decode_memory_alloc_cfg_t rx_mem_cfg = {
        .buffer_direction = JPEG_DEC_ALLOC_OUTPUT_BUFFER,
    };

    jpeg_decode_memory_alloc_cfg_t tx_mem_cfg = {
        .buffer_direction = JPEG_DEC_ALLOC_INPUT_BUFFER,
    };

    uint32_t out_buf_size = MJPEG_OUTPUT_SIZE;
    uint32_t bit_stream_size = MJPEG_BUFFER_SIZE;
    _mjpeg_buf = (uint8_t *)jpeg_alloc_decoder_mem(bit_stream_size, &tx_mem_cfg, &tx_buffer_size);
    _output_buf = (uint16_t *)jpeg_alloc_decoder_mem(out_buf_size, &rx_mem_cfg, &rx_buffer_size);

    return true;
  }

  bool readMjpegBuf()
  {
    if (_read == 0)
    {
      // _mjpeg_buf empty
      _read = fread(_mjpeg_buf, 1, READ_BATCH_SIZE, _input);
    }
    else
    {
      // pad previous remain data to the start of _mjpeg_buf
      memcpy(_mjpeg_buf, _p, _read);
    }

    bool found_FFD8 = false;
    _p = _mjpeg_buf;
    while ((_read > 0) && (!found_FFD8))
    {
      while ((_read > 1) && (!found_FFD8))
      {
        --_read;
        if ((*_p++ == 0xFF) && (*_p == 0xD8)) // JPEG header
        {
          // Serial.printf("Found FFD8 at: %d.\n", i);
          found_FFD8 = true;
        }
      }
      if (!found_FFD8)
      {
        if (*_p == 0xFF)
        {
          _mjpeg_buf[0] = 0xFF;
          _read = fread(_mjpeg_buf + 1, 1, READ_BATCH_SIZE, _input) + 1;
        }
        else
        {
          _read = fread(_mjpeg_buf, 1, READ_BATCH_SIZE, _input);
        }
        _p = _mjpeg_buf;
      }
    }

    if (!found_FFD8)
    {
      return false;
    }

    // rewind 1 byte
    --_p;
    ++_read;

    // pad JPEG header to the start of _mjpeg_buf
    if (_p > _mjpeg_buf)
    {
      Serial.println("(_p > _mjpeg_buf)");
      memcpy(_mjpeg_buf, _p, _read);
    }

    // skip JPEG header
    _p += 2;
    _read -= 2;

    if (_read == 0)
    {
      _read = fread(_p, 1, READ_BATCH_SIZE, _input);
    }

    bool found_FFD9 = false;
    while ((_read > 0) && (!found_FFD9))
    {
      while ((_read > 1) && (!found_FFD9))
      {
        --_read;
        if ((*_p++ == 0xFF) && (*_p == 0xD9)) // JPEG trailer
        {
          // Serial.printf("Found FFD9 at: %d.\n", i);
          found_FFD9 = true;
        }
      }

      if (!found_FFD9)
      {
        _read += fread(_p + _read, 1, READ_BATCH_SIZE, _input);
        // Serial.printf("_read: %d\n", _read - 1);
      }
    }

    if (found_FFD9)
    {
      ++_p;
      --_read;
      return true;
    }

    return false;
  }

  bool decodeJpg()
  {
    jpeg_decode_picture_info_t header_info;
    ESP_ERROR_CHECK(jpeg_decoder_get_info(_mjpeg_buf, _p - _mjpeg_buf, &header_info));
    _w = header_info.width;
    _h = header_info.height;
    uint32_t out_size;
    jpeg_decode_cfg_t decode_cfg_rgb = {
        .output_format = JPEG_DECODE_OUT_FORMAT_RGB565,
        .rgb_order = JPEG_DEC_RGB_ELEMENT_ORDER_BGR,
    };
    ESP_ERROR_CHECK(jpeg_decoder_process(_decoder_engine, &decode_cfg_rgb, (const uint8_t *)_mjpeg_buf, _p - _mjpeg_buf, (uint8_t *)_output_buf, MJPEG_OUTPUT_SIZE, &out_size));

    return true;
  }

  int16_t getWidth()
  {
    return _w;
  }

  int16_t getHeight()
  {
    return _h;
  }

  uint16_t *getOutBuf()
  {
    return _output_buf;
  }

  void close()
  {
    fclose(_input);
  }

private:
  FILE *_input;
  uint8_t *_mjpeg_buf;
  uint16_t *_output_buf;

  jpeg_decoder_handle_t _decoder_engine;
  int16_t _w = 0, _h = 0;

  uint8_t *_p;
  int32_t _read;
};

#endif // defined(ESP32)
