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

#if defined(INCLUDE_READ_OPERATIONS)

uint8_t Arduino_DataBus::receive(uint8_t commandByte, uint8_t index)
{
  return 0;
}

uint16_t Arduino_DataBus::receive16(uint16_t addr)
{
  return 0;
}

#endif // defined(INCLUDE_READ_OPERATIONS)

#if !defined(LITTLE_FOOT_PRINT)
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
  w >>= 1;
  for (int i = 0; i < h;)
  {
    for (int j = 0; j < w; ++j)
    {
      uint8_t cb = *cbData++;
      uint8_t cr = *crData++;
      int16_t r = CR2R16[cr];
      int16_t g = -CB2G16[cb] - CR2G16[cr];
      int16_t b = CB2B16[cb];
      int16_t y = Y2I16[*yData++];
      _data16.value = CLIPRBE[y + r] | CLIPGBE[y + g] | CLIPBBE[y + b];
      write(_data16.lsb);
      write(_data16.msb);
      y = Y2I16[*yData++];
      _data16.value = CLIPRBE[y + r] | CLIPGBE[y + g] | CLIPBBE[y + b];
      write(_data16.lsb);
      write(_data16.msb);
    }

    if (++i & 1)
    {
      // rollback CbCr data
      cbData -= w;
      crData -= w;
    }
  }
}

#endif // !defined(LITTLE_FOOT_PRINT)
