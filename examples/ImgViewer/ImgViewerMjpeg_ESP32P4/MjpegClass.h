/*******************************************************************************
 * ESP32_JPEG Wrapper Class
 *
 * Dependent libraries:
 * ESP32_JPEG: https://github.com/esp-arduino-libs/ESP32_JPEG.git
 ******************************************************************************/
#pragma once

#if defined(ESP32)

#define READ_BUFFER_SIZE 1024

#include <driver/jpeg_decode.h>

class MjpegClass
{
public:
  bool setup(const char *path)
  {
    _input = fopen(path, "r");
    _inputindex = 0;

    if (!_read_buf)
    {
      _read_buf = (uint8_t *)malloc(READ_BUFFER_SIZE);
    }

    if (!_read_buf)
    {
      return false;
    }

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
    if (_inputindex == 0)
    {
      _buf_read = fread(_read_buf, 1, READ_BUFFER_SIZE, _input);
      _inputindex += _buf_read;
    }
    _mjpeg_buf_offset = 0;
    int i = 0;
    bool found_FFD8 = false;
    while ((_buf_read > 0) && (!found_FFD8))
    {
      i = 0;
      while ((i < _buf_read) && (!found_FFD8))
      {
        if ((_read_buf[i] == 0xFF) && (_read_buf[i + 1] == 0xD8)) // JPEG header
        {
          // Serial.printf("Found FFD8 at: %d.\n", i);
          found_FFD8 = true;
        }
        ++i;
      }
      if (found_FFD8)
      {
        --i;
      }
      else
      {
        _buf_read = fread(_read_buf, 1, READ_BUFFER_SIZE, _input);
      }
    }
    uint8_t *_p = _read_buf + i;
    _buf_read -= i;
    bool found_FFD9 = false;
    if (_buf_read > 0)
    {
      i = 3;
      while ((_buf_read > 0) && (!found_FFD9))
      {
        if ((_mjpeg_buf_offset > 0) && (_mjpeg_buf[_mjpeg_buf_offset - 1] == 0xFF) && (_p[0] == 0xD9)) // JPEG trailer
        {
          // Serial.printf("Found FFD9 at: %d.\n", i);
          found_FFD9 = true;
        }
        else
        {
          while ((i < _buf_read) && (!found_FFD9))
          {
            if ((_p[i] == 0xFF) && (_p[i + 1] == 0xD9)) // JPEG trailer
            {
              found_FFD9 = true;
              ++i;
            }
            ++i;
          }
        }

        // Serial.printf("i: %d\n", i);
        memcpy(_mjpeg_buf + _mjpeg_buf_offset, _p, i);
        _mjpeg_buf_offset += i;
        size_t o = _buf_read - i;
        if (o > 0)
        {
          // Serial.printf("o: %d\n", o);
          memcpy(_read_buf, _p + i, o);
          _buf_read = fread(_read_buf + o, 1, READ_BUFFER_SIZE - o, _input);
          _p = _read_buf;
          _inputindex += _buf_read;
          _buf_read += o;
          // Serial.printf("_buf_read: %d\n", _buf_read);
        }
        else
        {
          _buf_read = fread(_read_buf, 1, READ_BUFFER_SIZE, _input);
          _p = _read_buf;
          _inputindex += _buf_read;
        }
        i = 0;
      }
      if (found_FFD9)
      {
        return true;
      }
    }

    return false;
  }

  bool decodeJpg()
  {
    _remain = _mjpeg_buf_offset;

    jpeg_decode_picture_info_t header_info;
    ESP_ERROR_CHECK(jpeg_decoder_get_info(_mjpeg_buf, _remain, &header_info));
    _w = header_info.width;
    _h = header_info.height;
    uint32_t out_size;
    jpeg_decode_cfg_t decode_cfg_rgb = {
        .output_format = JPEG_DECODE_OUT_FORMAT_RGB565,
        .rgb_order = JPEG_DEC_RGB_ELEMENT_ORDER_BGR,
    };
    ESP_ERROR_CHECK(jpeg_decoder_process(_decoder_engine, &decode_cfg_rgb, (const uint8_t *)_mjpeg_buf, _remain, (uint8_t *)_output_buf, MJPEG_OUTPUT_SIZE, &out_size));

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

  uint8_t *_read_buf;
  int32_t _mjpeg_buf_offset = 0;

  jpeg_decoder_handle_t _decoder_engine;

  int16_t _w = 0, _h = 0;

  int32_t _inputindex = 0;
  int32_t _buf_read;
  int32_t _remain = 0;
};

#endif // defined(ESP32)
