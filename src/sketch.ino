
#include <LSY201.h>

LSY201 camera(Serial);

void setup()
{
  Serial.begin(9600);
  camera.reset();
}

void loop()
{
}
