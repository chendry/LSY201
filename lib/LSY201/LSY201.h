
#ifndef LSY201_H
#define LSY201_H

#include <Stream.h>

class LSY201
{
  Stream *_camera;
  Stream *_debug;
  bool _eof;

public:

  enum Size
  {
    Small = 0x22,
    Medium = 0x11,
    Large = 0x00
  };

  LSY201();
  void setSerial(Stream &stream);
  void setDebugSerial(Stream &stream);
  void reset();
  void waitForInitEnd();
  void takePicture();
  uint16_t readJpegFileSize();
  bool readJpegFileContent(uint16_t offset, uint8_t *buf, uint16_t size);
  void setCompressionRatio(uint8_t value);
  void setImageSize(Size size);
  void setBaudRate(unsigned long baud);

private:

  void tx(const uint8_t *bytes, uint8_t length);
  void rx(const uint8_t *bytes, uint8_t length);
  uint8_t readByte();
};

#endif
