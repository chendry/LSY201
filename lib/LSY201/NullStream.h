
#ifndef NULLSTREAM_H
#define NULLSTREAM_H

struct NullStream : public Stream
{
  NullStream() { return; }
  int available() { return 0; }
  void flush() { return; }
  int peek() { return -1; }
  int read() { return -1; }
  size_t write(uint8_t d) { return 1; }
};

#endif

