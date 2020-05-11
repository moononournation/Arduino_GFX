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
#include "esp_jpg_decode.h"

static uint32_t _jpg_write(JDEC *decoder, void *bitmap, JRECT *rect)
{
    uint16_t x = rect->left;
    uint16_t y = rect->top;
    uint16_t w = rect->right + 1 - x;
    uint16_t h = rect->bottom + 1 - y;
    uint8_t *data = (uint8_t *)bitmap;

    esp_jpg_decoder_t *jpeg = (esp_jpg_decoder_t *)decoder->device;

    if (jpeg->writer)
    {
        return jpeg->writer(jpeg->arg, x, y, w, h, data);
    }
    return 0;
}

static uint32_t _jpg_read(JDEC *decoder, uint8_t *buf, uint32_t len)
{
    esp_jpg_decoder_t *jpeg = (esp_jpg_decoder_t *)decoder->device;
    if (jpeg->len && len > (jpeg->len - jpeg->index))
    {
        len = jpeg->len - jpeg->index;
    }
    if (len)
    {
        len = jpeg->reader(jpeg->arg, jpeg->index, buf, len);
        if (!len)
        {
            //            loge("Read Fail at %u/%u", jpeg->index, jpeg->len);
        }
        jpeg->index += len;
    }
    return len;
}

void esp_jpg_decode(size_t len, jpg_scale_t scale, jpg_reader_cb reader, jpg_writer_cb writer, void *arg)
{
    static uint8_t work[3100];
    JDEC decoder;
    esp_jpg_decoder_t jpeg;

    jpeg.len = len;
    jpeg.reader = reader;
    jpeg.writer = writer;
    jpeg.arg = arg;
    jpeg.scale = scale;
    jpeg.index = 0;

    JRESULT jres = jd_prepare(&decoder, _jpg_read, work, 3100, &jpeg);
    if (jres != JDR_OK)
    {
        return;
    }

    uint16_t output_width = decoder.width / (1 << (uint8_t)(jpeg.scale));
    uint16_t output_height = decoder.height / (1 << (uint8_t)(jpeg.scale));

    //output start
    writer(arg, 0, 0, output_width, output_height, NULL);
    //output write
    jres = jd_decomp(&decoder, _jpg_write, (uint8_t)jpeg.scale);
    //output end
    writer(arg, output_width, output_height, output_width, output_height, NULL);

    //check if all data has been consumed.
    if (len && jpeg.index < len)
    {
        _jpg_read(&decoder, NULL, len - jpeg.index);
    }
}

// #endif // ESP8266
