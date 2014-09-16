
#ifndef LSY201_H
#define LSY201_H

#include <Stream.h>

class LSY201
{
  Stream *_stream;
  Stream *_debug;
  bool _foundJpegEOF;

public:

  enum Size
  {
    Size_320x240 = 0x11,
    Size_640x480 = 0x00,
    Size_160x120 = 0x22
  };

  enum Baud
  {
    Baud_9600 = 0xAEC8,
    Baud_19200 = 0x56E4,
    Baud_38400 = 0x2AF2,
    Baud_57600 = 0x1C4C,
    Baud_115200 = 0x0DA6
  };

  LSY201();
  void setSerial(Stream &stream);
  void setDebugSerial(Stream &stream);
  void reset();
  void takePicture();
  uint16_t readJpegFileSize();
  bool readJpegFileContent(uint16_t offset, uint8_t *buf, uint16_t size);
  void stopTakingPictures();
  void setCompressionRatio(uint8_t value);
  void setImageSize(Size size);
  void enterPowerSaving();
  void exitPowerSaving();
  void setBaudRate(Baud baud);

private:

  void tx(const uint8_t *bytes, uint8_t length);
  void rx(const uint8_t *bytes, uint8_t length);
  uint8_t readByte();
};

#endif
