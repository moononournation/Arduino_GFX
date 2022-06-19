#ifndef _VNC_GFX_H_
#define _VNC_GFX_H_

#include "Arduino_GFX_Library.h"
#include "VNC.h"
#include "VNC_config.h"

class VNC_GFX : public VNCdisplay
{
public:
    VNC_GFX(Arduino_TFT *gfx)
    {
        _gfx = gfx;
    }

    bool hasCopyRect(void)
    {
        return false;
    }

    uint32_t getHeight(void)
    {
        return _gfx->height();
    }
    uint32_t getWidth(void)
    {
        return _gfx->width();
    }

    void draw_area(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *data)
    {
        _gfx->draw16bitRGBBitmap(x, y, (uint16_t *)data, w, h);
    }

    void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color)
    {
        _gfx->fillRect(x, y, w, h, color);
    }

    void copy_rect(uint32_t src_x, uint32_t src_y, uint32_t dest_x, uint32_t dest_y, uint32_t w, uint32_t h)
    {
    }

    void area_update_start(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
    {
        _gfx->startWrite();
    }

    void area_update_data(char *data, uint32_t pixel)
    {
        _gfx->writePixels((uint16_t *)data, pixel);
    }

    void area_update_end(void)
    {
        _gfx->endWrite();
    }

private:
    uint32_t area_x, area_y, area_w, area_h;
    Arduino_TFT *_gfx;
};

#endif /* _VNC_GFX_H_ */
