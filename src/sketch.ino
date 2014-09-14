
#include <LSY201.h>
#include <SoftwareSerial.h>

SoftwareSerial camera_serial(2, 3);
LSY201 camera(camera_serial);
uint8_t buf[32];

void setup()
{
  Serial.begin(38400);
  camera_serial.begin(38400);

  Serial.println("setup finished");
}

void loop()
{
  camera.reset();
  camera.set_image_size(LSY201::Size160x120);
  camera.take_picture();

  uint16_t offset = 0;
  while (camera.read_jpeg_file_content(buf, offset, sizeof(buf)))
  {
    for (int i = 0; i < sizeof(buf); i ++)
      Serial.println(buf[i], HEX);

    offset += sizeof(buf);
  }

  Serial.println("DONE");
  delay(30000);
}
