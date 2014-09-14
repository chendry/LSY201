
#ifndef LSY201_H
#define LSY201_H

#include <Stream.h>

class LSY201
{
  Stream *_stream;
  Stream *_debug;

public:

  enum Size
  {
    S320x240 = 0x11,
    S640x480 = 0x00,
    S160x120 = 0x22
  };

  LSY201(Stream &stream);
  void setDebugStream(Stream &stream);
  void reset();
  void take_picture();
  uint16_t read_jpeg_file_size();
  uint8_t *read_jpeg_file_content(uint16_t offset, uint16_t size);
  bool read_jpeg_file_content(uint8_t *buf, uint16_t offset, uint16_t size);
  void stop_taking_pictures();
  void set_compression_ratio(uint8_t value);
  void set_image_size(Size size);

private:

  void discard_all_input();
  void tx(const uint8_t *bytes, uint8_t length);
  void rx(const uint8_t *bytes, uint8_t length);
  uint8_t read_byte();
};

#endif
