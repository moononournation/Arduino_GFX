/*
 * start rewrite from:
 * https://github.com/adafruit/Adafruit-GFX-Library.git
 */
#include "Arduino_DataBus.h"

Arduino_DataBus::Arduino_DataBus() {}

void Arduino_DataBus::writeC8D8(uint8_t c, uint8_t d)
{
  writeCommand(c);
  write(d);
}

void Arduino_DataBus::writeC8D16(uint8_t c, uint16_t d)
{
  writeCommand(c);
  write16(d);
}

void Arduino_DataBus::writeC16D16(uint16_t c, uint16_t d)
{
  writeCommand16(c);
  write16(d);
}

void Arduino_DataBus::writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2)
{
  writeCommand(c);
  write16(d1);
  write16(d2);
}

void Arduino_DataBus::writeC8D16D16Split(uint8_t c, uint16_t d1, uint16_t d2)
{
  writeCommand(c);
  _data16.value = d1;
  write(_data16.msb);
  write(_data16.lsb);
  _data16.value = d2;
  write(_data16.msb);
  write(_data16.lsb);
}

void Arduino_DataBus::sendCommand(uint8_t c)
{
  beginWrite();
  writeCommand(c);
  endWrite();
}

void Arduino_DataBus::sendCommand16(uint16_t c)
{
  beginWrite();
  writeCommand16(c);
  endWrite();
}

void Arduino_DataBus::sendData(uint8_t d)
{
  beginWrite();
  write(d);
  endWrite();
}

void Arduino_DataBus::sendData16(uint16_t d)
{
  beginWrite();
  write16(d);
  endWrite();
}

void Arduino_DataBus::batchOperation(const uint8_t *operations, size_t len)
{
  for (size_t i = 0; i < len; ++i)
  {
    uint8_t l = 0;
    switch (operations[i])
    {
    case BEGIN_WRITE:
      beginWrite();
      break;
    case WRITE_C8_D16:
      l++;
      /* fall through */
    case WRITE_C8_D8:
      l++;
      /* fall through */
    case WRITE_COMMAND_8:
      writeCommand(operations[++i]);
      break;
    case WRITE_C16_D16:
      l = 2;
      /* fall through */
    case WRITE_COMMAND_16:
      _data16.msb = operations[++i];
      _data16.lsb = operations[++i];
      writeCommand16(_data16.value);
      break;
    case WRITE_COMMAND_BYTES:
      l = operations[++i];
      writeCommandBytes((uint8_t *)(operations + i + 1), l);
      i += l;
      l = 0;
      break;
    case WRITE_DATA_8:
      l = 1;
      break;
    case WRITE_DATA_16:
      l = 2;
      break;
    case WRITE_BYTES:
      l = operations[++i];
      break;
    case WRITE_C8_BYTES:
      writeCommand(operations[++i]);
      l = operations[++i];
      break;
    case END_WRITE:
      endWrite();
      break;
    case DELAY:
      delay(operations[++i]);
      break;
    default:
      printf("Unknown operation id at %d: %d\n", i, operations[i]);
      break;
    }
    while (l--)
    {
      write(operations[++i]);
    }
  }
}

#if !defined(LITTLE_FOOT_PRINT)
void Arduino_DataBus::write16bitBeRGBBitmapR1(uint16_t *bitmap, int16_t w, int16_t h)
{
  uint16_t *p;
  for (int16_t i = 0; i < w; i++)
  {
    p = bitmap + ((h - 1) * w) + i;
    for (int16_t j = 0; j < h; j++)
    {
      _data16.value = *p;
      write(_data16.lsb);
      write(_data16.msb);
      p -= w;
    }
  }
}

void Arduino_DataBus::writePattern(uint8_t *data, uint8_t len, uint32_t repeat)
{
  while (repeat--)
  {
    writeBytes(data, len);
  }
}

void Arduino_DataBus::writeIndexedPixels(uint8_t *data, uint16_t *idx, uint32_t len)
{
  while (len--)
  {
    write16(idx[*(data++)]);
  }
}

void Arduino_DataBus::writeIndexedPixelsDouble(uint8_t *data, uint16_t *idx, uint32_t len)
{
  uint8_t *d = data;
  while (len--)
  {
    _data16.value = idx[*(d++)];
    write(_data16.msb);
    write(_data16.lsb);
    write(_data16.msb);
    write(_data16.lsb);
  }
}

void Arduino_DataBus::writeYCbCrPixels(uint8_t *yData, uint8_t *cbData, uint8_t *crData, uint16_t w, uint16_t h)
{
  int cols = w >> 1;

  uint8_t pxCb, pxCr;
  int16_t pxR, pxG, pxB, pxY;

  for (int row = 0; row < h;)
  {
    for (int col = 0; col < cols; ++col)
    {
      pxCb = *cbData++;
      pxCr = *crData++;
      pxR = CR2R16[pxCr];
      pxG = -CB2G16[pxCb] - CR2G16[pxCr];
      pxB = CB2B16[pxCb];

      pxY = Y2I16[*yData++];
      _data16.value = CLIPRBE[pxY + pxR] | CLIPGBE[pxY + pxG] | CLIPBBE[pxY + pxB];
      write(_data16.lsb);
      write(_data16.msb);
      pxY = Y2I16[*yData++];
      _data16.value = CLIPRBE[pxY + pxR] | CLIPGBE[pxY + pxG] | CLIPBBE[pxY + pxB];
      write(_data16.lsb);
      write(_data16.msb);
    }

    if (++row & 1)
    {
      // rollback CbCr data
      cbData -= cols;
      crData -= cols;
    }
  }
}

#endif // !defined(LITTLE_FOOT_PRINT)
