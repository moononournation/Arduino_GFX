/*******************************************************************************
 * JPEGDEC Wrapper Class
 *
 * Dependent libraries:
 * JPEGDEC: https://github.com/bitbank2/JPEGDEC.git
 ******************************************************************************/
#pragma once

#include <JPEGDEC.h>

#define READ_BATCH_SIZE 1024

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#elif defined(ESP32) || defined(ESP8266)
#include <FS.h>
#else
#include <SD.h>
#endif

class MjpegClass
{
public:
  bool setup(
      Stream *input, uint8_t *mjpeg_buf, JPEG_DRAW_CALLBACK *pfnDraw, bool useBigEndian,
      int x, int y, int widthLimit, int heightLimit)
  {
    _input = input;
    _mjpeg_buf = mjpeg_buf;
    _pfnDraw = pfnDraw;
    _useBigEndian = useBigEndian;
    _x = x;
    _y = y;
    _widthLimit = widthLimit;
    _heightLimit = heightLimit;
    _read = 0;

    return true;
  }

  bool readMjpegBuf()
  {
    if (_read == 0)
    {
      // _mjpeg_buf empty
    _read = _input->readBytes(_mjpeg_buf, READ_BATCH_SIZE);
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
          _read = _input->readBytes(_mjpeg_buf + 1, READ_BATCH_SIZE) + 1;
        }
        else
        {
          _read = _input->readBytes(_mjpeg_buf, READ_BATCH_SIZE);
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
      _read = _input->readBytes(_p, READ_BATCH_SIZE);
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
        _read += _input->readBytes(_p + _read, READ_BATCH_SIZE);
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

  bool drawJpg()
  {
    _jpeg.openRAM(_mjpeg_buf, _p - _mjpeg_buf, _pfnDraw);
    if (_scale == -1)
    {
      // scale to fit height
      int iMaxMCUs;
      int w = _jpeg.getWidth();
      int h = _jpeg.getHeight();
      float ratio = (float)h / _heightLimit;
      if (ratio <= 1)
      {
        _scale = 0;
        iMaxMCUs = _widthLimit / 16;
      }
      else if (ratio <= 2)
      {
        _scale = JPEG_SCALE_HALF;
        iMaxMCUs = _widthLimit / 8;
        w /= 2;
        h /= 2;
      }
      else if (ratio <= 4)
      {
        _scale = JPEG_SCALE_QUARTER;
        iMaxMCUs = _widthLimit / 4;
        w /= 4;
        h /= 4;
      }
      else
      {
        _scale = JPEG_SCALE_EIGHTH;
        iMaxMCUs = _widthLimit / 2;
        w /= 8;
        h /= 8;
      }
      _jpeg.setMaxOutputSize(iMaxMCUs);
      _x = (w > _widthLimit) ? 0 : ((_widthLimit - w) / 2);
      _y = (_heightLimit - h) / 2;
    }
    if (_useBigEndian)
    {
      _jpeg.setPixelType(RGB565_BIG_ENDIAN);
    }
    _jpeg.decode(_x, _y, _scale);
    _jpeg.close();

    return true;
  }

private:
  Stream *_input;
  uint8_t *_mjpeg_buf;
  JPEG_DRAW_CALLBACK *_pfnDraw;
  bool _useBigEndian;
  int _x;
  int _y;
  int _widthLimit;
  int _heightLimit;
  int _scale = -1;

  JPEGDEC _jpeg;

  uint8_t *_p;
  int32_t _read;
};
