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
        // DEBUG_VNC("draw_area(%d, %d, %d, %d, data)\n", x, y, w, h);
#ifdef RTL8722DM
        _gfx->draw16bitBeRGBBitmap(x, y, (uint16_t *)data, w, h);
#else
        _gfx->draw16bitRGBBitmap(x, y, (uint16_t *)data, w, h);
#endif
    }

    void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t color)
    {
        // DEBUG_VNC("draw_rect(%d, %d, %d, %d, color)\n", x, y, w, h);
        _gfx->fillRect(x, y, w, h, color);
    }

    void copy_rect(uint32_t src_x, uint32_t src_y, uint32_t dest_x, uint32_t dest_y, uint32_t w, uint32_t h)
    {
        // DEBUG_VNC("copy_rect(%d, %d, %d, %d, %d, %d)\n", src_x, src_y, dest_x, dest_y, w, h);
    }

    void area_update_start(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
    {
        // DEBUG_VNC("area_update_start(%d, %d, %d, %d)\n", x, y, w, h);
        _gfx->startWrite();
        _gfx->writeAddrWindow(x, y, w, h);
    }

    void area_update_data(char *data, uint32_t pixel)
    {
        // DEBUG_VNC("area_update_data(data, %d)\n", pixel);
#ifdef RTL8722DM
        _gfx->writeBytes((uint8_t *)data, pixel * 2);
#else
        _gfx->writePixels((uint16_t *)data, pixel);
#endif
    }

    void area_update_end(void)
    {
        // DEBUG_VNC("area_update_end()\n");
        _gfx->endWrite();
    }

private:
    Arduino_TFT *_gfx;
};

#endif /* _VNC_GFX_H_ */
