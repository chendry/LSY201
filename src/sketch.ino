
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
  camera.set_image_size(LSY201::S320x240);

  output_size_for_picture_at_compression_ratio(0xFF);
  output_size_for_picture_at_compression_ratio(0x80);
  output_size_for_picture_at_compression_ratio(0x00);

  camera.enter_power_saving();
  camera.exit_power_saving();

  delay(30000);

  int offset = 0;
  while (camera.read_jpeg_file_content(buf, offset, sizeof(buf)))
    offset += sizeof(buf);

  delay(5000);
}

void output_size_for_picture_at_compression_ratio(int value)
{
  camera.reset();
  delay(4000);

  camera.set_compression_ratio(value);
  camera.take_picture();

  uint16_t size = camera.read_jpeg_file_size();

  Serial.print("size at ");
  Serial.print(value);
  Serial.print(": ");
  Serial.println(size);
}
