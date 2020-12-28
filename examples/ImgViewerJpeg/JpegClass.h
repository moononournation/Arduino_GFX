/*******************************************************************************
 * JPEGDEC Wrapper Class
 * 
 * Dependent libraries:
 * JPEGDEC: https://github.com/bitbank2/JPEGDEC.git
 ******************************************************************************/
#ifndef _JPEGCLASS_H_
#define _JPEGCLASS_H_

#include <JPEGDEC.h>

/* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
#include <Seeed_FS.h>
#else
#include <FS.h>
#endif

class JpegClass
{
public:
    void draw(
        FS *fs, char *filename, JPEG_DRAW_CALLBACK *jpegDrawCallback, bool useBigEndian,
        int x, int y, int widthLimit, int heightLimit)
    {
        File f = fs->open(filename);
        _jpeg.open(f, jpegDrawCallback);

        // scale to fit height
        int _scale;
        int iMaxMCUs;
        float ratio = (float)_jpeg.getHeight() / heightLimit;
        if (ratio <= 1)
        {
            _scale = 0;
            iMaxMCUs = widthLimit / 16;
        }
        else if (ratio <= 2)
        {
            _scale = JPEG_SCALE_HALF;
            iMaxMCUs = widthLimit / 8;
        }
        else if (ratio <= 4)
        {
            _scale = JPEG_SCALE_QUARTER;
            iMaxMCUs = widthLimit / 4;
        }
        else
        {
            _scale = JPEG_SCALE_EIGHTH;
            iMaxMCUs = widthLimit / 2;
        }
        _jpeg.setMaxOutputSize(iMaxMCUs);
        if (useBigEndian)
        {
            _jpeg.setPixelType(RGB565_BIG_ENDIAN);
        }
        _jpeg.decode(x, y, _scale);
        _jpeg.close();
    }

private:
    JPEGDEC _jpeg;
};

#endif // _JPEGCLASS_H_