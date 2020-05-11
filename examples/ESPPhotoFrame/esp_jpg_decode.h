// #if defined(ESP8266)
// extract from esp32 source code for filling ESP8266 missing code
// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef _ESP_JPG_DECODE
#define _ESP_JPG_DECODE

#include <stddef.h>
#include <stdbool.h>
#include "tjpgd.h"

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
{
#endif

    typedef __SIZE_TYPE__ size_t;

    /* These types must be 16-bit, 32-bit or larger integer */
    typedef int INT;
    typedef unsigned int UINT;

    /* These types must be 8-bit integer */
    typedef char CHAR;
    typedef unsigned char UCHAR;
    typedef unsigned char BYTE;

    /* These types must be 16-bit integer */
    typedef short SHORT;
    typedef unsigned short USHORT;
    typedef unsigned short WORD;
    typedef unsigned short WCHAR;

    /* These types must be 32-bit integer */
    typedef long LONG;
    typedef unsigned long ULONG;
    typedef unsigned long DWORD;

    typedef enum
    {
        JPG_SCALE_NONE,
        JPG_SCALE_2X,
        JPG_SCALE_4X,
        JPG_SCALE_8X,
        JPG_SCALE_MAX = JPG_SCALE_8X
    } jpg_scale_t;

    typedef size_t (*jpg_reader_cb)(void *arg, size_t index, uint8_t *buf, size_t len);
    typedef bool (*jpg_writer_cb)(void *arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);

    typedef struct
    {
        jpg_scale_t scale;
        jpg_reader_cb reader;
        jpg_writer_cb writer;
        void *arg;
        size_t len;
        size_t index;
    } esp_jpg_decoder_t;

    void esp_jpg_decode(size_t len, jpg_scale_t scale, jpg_reader_cb reader, jpg_writer_cb writer, void *arg);

#ifdef __cplusplus
}
#endif

#endif /* _TJPGDEC */
// #endif // ESP8266
