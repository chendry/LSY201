
#ifndef LSY201_H
#define LSY201_H

#include <Stream.h>

class LSY201
{
public:

  LSY201(Stream &stream) : _stream(&stream)
  {
  }

  void reset();

private:

  Stream *_stream;
};

#endif
