
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
    Size320x240 = 0x11,
    Size640x480 = 0x00,
    Size160x120 = 0x22
  };

  enum Baud
  {
    Baud9600 = 0xAEC8,
    Baud19200 = 0x56E4,
    Baud38400 = 0x2AF2,
    Baud57600 = 0x1C4C,
    Baud115200 = 0x0DA6
  };

  LSY201(Stream &stream);
  void set_debug_stream(Stream &stream);
  void reset();
  void take_picture();
  uint16_t read_jpeg_file_size();
  uint8_t *read_jpeg_file_content(uint16_t offset, uint16_t size);
  bool read_jpeg_file_content(uint8_t *buf, uint16_t offset, uint16_t size);
  void stop_taking_pictures();
  void set_compression_ratio(uint8_t value);
  void set_image_size(Size size);
  void enter_power_saving();
  void exit_power_saving();
  void set_baud_rate(Baud baud);

private:

  void discard_all_input();
  void tx(const uint8_t *bytes, uint8_t length);
  void rx(const uint8_t *bytes, uint8_t length);
  uint8_t read_byte();
};

#endif
