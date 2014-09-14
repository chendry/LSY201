
#include <LSY201.h>
#include <SoftwareSerial.h>

SoftwareSerial camera_serial(2, 3);
LSY201 camera(camera_serial);

void setup()
{
  Serial.begin(9600);
  Serial.println("*** STARTING ***");

  camera_serial.begin(38400);
  camera.setDebugStream(Serial);
}

void loop()
{
  camera.reset();
  delay(3000);
  camera.take_picture();

  Serial.println("size:");
  Serial.println(camera.read_jpeg_file_size());
  delay(10000);
}
