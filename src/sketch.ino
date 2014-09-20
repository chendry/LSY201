
#include <SoftwareSerial.h>
#include <LSY201.h>

/* assuming the TX and RX pins on the camera are attached to pins 2 and 3 of
 * the arduino. */
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
  Serial.println("Taking picture...");
  camera.takePicture();

  Serial.println("Bytes:");

  uint16_t offset = 0;
  while (camera.readJpegFileContent(offset, buf, sizeof(buf)))
  {
    for (int i = 0; i < sizeof(buf); i ++)
      Serial.println(buf[i], HEX);

    offset += sizeof(buf);
  }

  Serial.println("Done.");

  camera.stopTakingPictures();
  delay(10000);
}

