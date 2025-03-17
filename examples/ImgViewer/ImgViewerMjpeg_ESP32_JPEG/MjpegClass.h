/*******************************************************************************
 * ESP32_JPEG Wrapper Class
 *
 * Dependent libraries:
 * ESP32_JPEG: https://github.com/esp-arduino-libs/ESP32_JPEG.git
 ******************************************************************************/
#pragma once

#if defined(ESP32)
#include <ESP32_JPEG_Library.h>

#define READ_BATCH_SIZE 1024

class MjpegClass
{
public:
  bool setup(
      const char *path, uint8_t *mjpeg_buf,
      uint16_t *output_buf, size_t output_buf_size, bool useBigEndian)
  {
    _input = fopen(path, "r");
    _mjpeg_buf = mjpeg_buf;
    _output_buf = (uint8_t *)output_buf;
    _output_buf_size = output_buf_size;
    _useBigEndian = useBigEndian;
    _read = 0;

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
    // Generate default configuration
    jpeg_dec_config_t config = {
        .output_type = JPEG_RAW_TYPE_RGB565_BE,
        .rotate = JPEG_ROTATE_0D,
    };
    // Create jpeg_dec
    _jpeg_dec = jpeg_dec_open(&config);

    // Create io_callback handle
    _jpeg_io = (jpeg_dec_io_t *)calloc(1, sizeof(jpeg_dec_io_t));

    // Create out_info handle
    _out_info = (jpeg_dec_header_info_t *)calloc(1, sizeof(jpeg_dec_header_info_t));

    // Set input buffer and buffer len to io_callback
    _jpeg_io->inbuf = _mjpeg_buf;
    _jpeg_io->inbuf_len = _p - _mjpeg_buf;

    jpeg_dec_parse_header(_jpeg_dec, _jpeg_io, _out_info);

    _w = _out_info->width;
    _h = _out_info->height;

    if ((_w * _h * 2) > _output_buf_size)
    {
      return false;
    }
    _jpeg_io->outbuf = _output_buf;

    jpeg_dec_process(_jpeg_dec, _jpeg_io);
    jpeg_dec_close(_jpeg_dec);

    free(_jpeg_io);
    free(_out_info);

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

  void close()
  {
    fclose(_input);
  }

private:
  FILE *_input;
  uint8_t *_mjpeg_buf;
  uint8_t *_output_buf;
  size_t _output_buf_size;
  bool _useBigEndian;

  jpeg_dec_handle_t *_jpeg_dec;
  jpeg_dec_io_t *_jpeg_io;
  jpeg_dec_header_info_t *_out_info;
  int16_t _w = 0, _h = 0;

  uint8_t *_p;
  int32_t _read;
};

#endif // defined(ESP32)
