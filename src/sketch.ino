
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
  camera.setDebugSerial(Serial);

  Serial.println("setup finished");

  pinMode(13, OUTPUT);
}

void loop()
{
  while (true)
  {
    camera.setImageSize(LSY201::Size_640x480);
    camera.reset();
    camera.setImageSize(LSY201::Size_160x120);
    camera.setCompressionRatio(0x10);
    delay(3000);
    camera.takePicture();

    Serial.println(camera.readJpegFileSize());
    delay(5000);

    for (int i = 0; i < 10; i ++)
    {
      camera.enterPowerSaving();
      delay(1000);
      camera.exitPowerSaving();
      delay(1000);
    }
  }


  delay(30000);

  uint16_t offset = 0;
  while (camera.readJpegFileContent(offset, buf, sizeof(buf)))
  {
    for (int i = 0; i < sizeof(buf); i ++)
      Serial.println(buf[i], HEX);

    offset += sizeof(buf);
  }

  Serial.println("DONE");
  delay(30000);
}
