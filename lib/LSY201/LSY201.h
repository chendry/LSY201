
#ifndef LSY201_H
#define LSY201_H

#include <Stream.h>

class LSY201
{
  Stream *_stream;
  Stream *_debug;

public:

  LSY201(Stream &stream);
  void setDebugStream(Stream &stream);
  void reset();

private:

  void discard_all_input();
  void tx(const uint8_t *bytes, uint8_t length);
  void rx(const uint8_t *bytes, uint8_t length);
  uint8_t read_byte();
};

#endif
