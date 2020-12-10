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

static FS *_fs;

class JpegClass
{
public:
    void draw(
        FS *fs, char *filename, JPEG_DRAW_CALLBACK *jpegDrawCallback, bool useBigEndian,
        int x, int y, int widthLimit, int heightLimit)
    {
        _fs = fs;
        _jpeg.open(filename, JPGOpenFile, JPGCloseFile, JPGReadFile, JPGSeekFile, jpegDrawCallback);

        // scale to fit height
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
    static void *JPGOpenFile(const char *szFilename, int32_t *pFileSize)
    {
        // Serial.println("JPGOpenFile");
        static File f = _fs->open(szFilename, "r");
        *pFileSize = f.size();
        return &f;
    }

    static void JPGCloseFile(void *pHandle)
    {
        // Serial.println("JPGCloseFile");
        File *f = static_cast<File *>(pHandle);
        f->close();
    }

    static int32_t JPGReadFile(JPEGFILE *pFile, uint8_t *pBuf, int32_t iLen)
    {
        // Serial.printf("JPGReadFile, iLen: %d\n", iLen);
        File *f = static_cast<File *>(pFile->fHandle);
        size_t r = f->read(pBuf, iLen);
        return r;
    }

    static int32_t JPGSeekFile(JPEGFILE *pFile, int32_t iPosition)
    {
        // Serial.printf("JPGSeekFile, pFile->iPos: %d, iPosition: %d\n", pFile->iPos, iPosition);
        File *f = static_cast<File *>(pFile->fHandle);
        f->seek(iPosition);
        return iPosition;
    }

    JPEGDEC _jpeg;
    int _scale = -1;
};

#endif // _JPEGCLASS_H_