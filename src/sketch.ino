
#include <LSY201.h>
#include <SoftwareSerial.h>

SoftwareSerial camera_serial(2, 3);
LSY201 camera(camera_serial);
uint8_t buf[32];

void setup()
{
  Serial.begin(38400);
  camera_serial.begin(38400);
  camera.set_debug_stream(Serial);
}

void loop()
{
  camera.reset();
  delay(3000);

  camera.set_baud_rate(LSY201::Baud19200);
  camera_serial.begin(19200);
  delay(2000);

  camera.take_picture();
  
  while (1)
  {
    camera.read_jpeg_file_content(buf, 0, sizeof(buf));
  }
}
