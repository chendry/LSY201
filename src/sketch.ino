
#include <SoftwareSerial.h>
#include <LSY201.h>

/* assuming the TX and RX pins on the camera are attached to pins 2 and 3 of
 * the arduino. */
SoftwareSerial camera_serial(2, 3);

LSY201 camera;
uint8_t buf[32];
String hexbuf;

void setup()
{
  Serial.begin(115200);
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

    {
      if (buf[i] < 0x10) {
        hexbuf = "0" + String(buf[i], HEX);
      }
      else {
        hexbuf = String(buf[i], HEX);
      }
      //Serial.write(buf[i]); // Uncomment this for write character to Serial
      Serial.print(hexbuf); // Uncomment this for print HEX value of character to Serial
      if ((buf[i - 1] == 0xFF) && (buf[i] == 0xD9))
      {
        break;
      }
    }
    offset += sizeof(buf);
  }

  Serial.println();
  Serial.println("Done.");

  camera.stopTakingPictures();
  delay(10000);
}

