#ifndef _ARDUINO_UNOPAR8_H_
#define _ARDUINO_UNOPAR8_H_

#include "Arduino_DataBus.h"

// for MCUFriend UNO kind of shields. -jz-
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_UNOR4_WIFI)

class Arduino_UNOPAR8 : public Arduino_DataBus
{
public:
  Arduino_UNOPAR8(); // Constructor

  bool begin(int32_t speed = GFX_NOT_DEFINED, int8_t dataMode = GFX_NOT_DEFINED) override;
  void beginWrite() override;
  void endWrite() override;
  void writeCommand(uint8_t) override;
  void writeCommand16(uint16_t) override;
  void write(uint8_t) override;
  void write16(uint16_t) override;
  void writeRepeat(uint16_t p, uint32_t len) override;
  void writePixels(uint16_t *data, uint32_t len) override;

  void writeC8D8(uint8_t c, uint8_t d) override;
  void writeC8D16(uint8_t c, uint16_t d) override;
  void writeC8D16D16(uint8_t c, uint16_t d1, uint16_t d2) override;
#if !defined(LITTLE_FOOT_PRINT)
  void writeBytes(uint8_t *data, uint32_t len) override;
#endif
protected:
private:
  INLINE void WRITE(uint8_t d);
  INLINE void DC_HIGH(void);
  INLINE void DC_LOW(void);
  INLINE void CS_HIGH(void);
  INLINE void CS_LOW(void);

  static const int16_t _dc = A2;
  static const int16_t _cs = A3;
  static const int16_t _wr = A1;
  static const int16_t _rd = A0;

#if defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_UNOR4_WIFI)
  static const int16_t _p1_map[];
#endif

};

#endif // _ARDUINO_UNOPAR8_H_

#endif // #if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_UNOR4_WIFI)
