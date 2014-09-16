
#include <LSY201.h>
#include <SoftwareSerial.h>

SoftwareSerial camera_serial(2, 3);
LSY201 camera;
uint8_t buf[32];

void setup()
{
  Serial.begin(38400);

  camera_serial.begin(38400);
  camera.setSerial(camera_serial);
  camera.setImageSize(LSY201::Size_160x120);
}

void loop()
{
  camera.reset();
  camera.takePicture();

  uint16_t offset = 0;
  while (camera.readJpegFileContent(offset, buf, sizeof(buf)))
  {
    offset += sizeof(buf);
    for (int i = 0; i < sizeof(buf); i ++)
      Serial.write(buf[i]);
  }
}
