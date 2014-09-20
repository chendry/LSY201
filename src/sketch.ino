
#include <SoftwareSerial.h>
#include <LSY201.h>

SoftwareSerial camera_serial(2, 3);
LSY201 camera;
uint8_t buf[32];

void setup()
{
  camera.setSerial(camera_serial);
  camera_serial.begin(38400);
}

void loop()
{
  camera.reset();
  camera.takePicture();

  uint16_t offset = 0;
  while (camera.readJpegFileContent(offset, buf, sizeof(buf)))
  {
    for (int i = 0; i < sizeof(buf); i ++)
      Serial.write(buf[i]);

    offset += sizeof(buf);
  }
}

