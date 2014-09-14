
#include <LSY201.h>
#include <SoftwareSerial.h>

SoftwareSerial camera_serial(2, 3);
LSY201 camera(camera_serial);
uint8_t buf[32];

void setup()
{
  Serial.begin(38400);
  Serial.println("*** STARTING ***");

  camera_serial.begin(38400);
  camera.setDebugStream(Serial);
}

void loop()
{
  camera.reset();
  delay(2000);

  camera.take_picture();

  Serial.println("size:");
  Serial.println(camera.read_jpeg_file_size());

  int offset = 0;
  while (camera.read_jpeg_file_content(buf, offset, sizeof(buf)))
    offset += sizeof(buf);

  delay(5000);
}
