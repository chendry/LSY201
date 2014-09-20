
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

  camera.setImageSize(LSY201::Small);
  camera.reset();
}

void loop()
{
  camera.takePicture();

  uint16_t offset = 0;
  uint16_t sum = 0;
  while (camera.readJpegFileContent(offset, buf, sizeof(buf)))
  {
    offset += sizeof(buf);
    for (int i = 0; i < sizeof(buf); i ++)
      sum += buf[i];
  }

  Serial.println(sum);
  camera.stopTakingPictures();
  delay(25);
}
