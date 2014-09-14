
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
  camera.reset();
}

void loop()
{
  camera.take_picture();
  delay(1000);
}
