LSY201
======

LSY201 is an Arduino library to interface with the LinkSprite JPEG TTL Camera.
You can purchase the camera from SparkFun here:

[https://www.sparkfun.com/products/11610](https://www.sparkfun.com/products/11610)

You can download the library here:
[LSY201.zip](https://github.com/chendry/LSY201/releases/download/v1.0.0/LSY201.zip).

## A Minimal Example

The following example assumes that the TX and RX pins of the camera are
attached to pins 2 and 3 respectively of the Arduino.  This example continuously
takes pictures and outputs the raw bytes of the JPEG-encoded images to Serial:

    #include <SoftwareSerial.h>
    #include <LSY201.h>

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
      camera.reset();
      camera.takePicture();

      uint16_t offset = 0;
      while (camera.readJpegFileContent(offset, buf, sizeof(buf)))
      {
        for (int i = 0; i < sizeof(buf); i ++)
          Serial.write(buf[i]);

        offset += sizeof(buf);
      }
    }

## Usage

Create a LSY201 instance and call `setSerial` to give a it stream object
configured for the camera.  This example assumes that the TX and RX pins of the
camera are attached to pins 2 and 3 respectively of the Arduino:

    SoftwareSerial camera_serial(2, 3);
    camera_serial.begin(38400);  /* LSY201's default baud */
    LSY201 camera;
    camera.setSerial(camera_serial);

### Debugging Information

Call `setDebugSerial` to have debug information written to a serial object:

    camera.setDebugSerial(Serial);

Most of the methods of LSY201 send some bytes to the camera and read some bytes
back.  These requests and responses are logged to the debug serial object when
present.

For example, the following debug output shows the bytes transmitted and
received while changing the image size to 640x480, resetting the camera,
waiting for "Init end," and finally taking a picture:

    sending bytes: 56 0 31 5 4 1 0 19
    sending bytes: 0
    expecting bytes: 76 0 31 0 0
    76 ok
    0 ok
    31 ok
    0 ok
    0 ok
    sending bytes: 56 0 26 0
    expecting bytes: 76 0 26 0
    76 ok
    0 ok
    26 ok
    0 ok
    Ctrl infr exist
    User-defined sensor
    625
    Init end
    sending bytes: 56 0 36 1 0
    expecting bytes: 76 0 36 0 0
    76 ok
    0 ok
    36 ok
    0 ok
    0 ok

### Taking the Picture

`takePicture` takes a picture and stores it in the camera's internal buffer to
be read back later.

    camera.takePicture();

### Reading the JPEG

Call `readJpegFileContent` repeatedly to read the JPEG image data stored on the
camera.  This method expects three parameters:

1. the 0-based byte offset at which to start reading,
2. a buffer to store the data, and
3. the number of bytes to read.

The method returns `true` if it wrote new data to the buffer, or `false` if it
did not because the end-of-file had previously been reached.

For example, the following reads the entire contents of the JPEG in 32-byte
chunks and writes the raw bytes to Serial:

    uint16_t offset = 0;
    uint8_t buf[32];

    while (camera.readJpegFileContent(buf, offset, sizeof(buf)))
    {
      for (int i = 0; i < sizeof(buf); i ++)
        Serial.write(buf[i]);

       offset += sizeof(buf);
    }

### Taking Multiple Pictures

The camera has an internal buffer that must be cleared prior to taking another
picture; otherwise the next call to `takePicture` will effectively be ignored
and the buffer will not change.

Call `stopTakingPictures` to clear the camera's buffer.  You'll need to wait a
short while afterwords for this to work.  I've found that a 25ms delay is
consistently sufficient:

    camera.takePicture();
    camera.stopTakingPictures();
    delay(25);
    camera.takePicture();

You can alternatively use `reset` to clear the buffer, but that has some
additional side effects described below.

### Setting the Baud Rate

Call `setBaudRate` with one of the supported baud rates: 9600, 19200, 38400,
57600, or 115200.  The camera will be using that baud when the method returns
so you'll need to reconfigure your serial object accordingly:

    camera.setBaudRate(9600);
    camera_serial.begin(9600);

Using a baud rate other than the default 38400 can make development on the
Arduino a bit tricky.  Suppose that your sketch's `setup` looks like this:

    LSY201 camera;
    SoftwareSerial camera_serial(2, 3);

    void setup()
    {
      camera.setSerial(camera_serial);
      camera_serial.begin(38400);

      camera.setBaudRate(9600);
      camera_serial.begin(9600);
    }

This will work the first time.  However, after you reset the Arduino or
redeploy your sketch, the call to `setBaudRate` will never return.  This is
because the camera is still operating at 9600 baud.  To get around this you can
power cycle the camera so that it returns to it's default 38400 baud assumed by
`setup`.

### Resetting the Camera

Resetting the camera clears the internal buffer and returns the compression
ratio and baud rate to their default values.

If you are using the default 38400 baud rate, simply call `reset`:

    camera.reset();

Things are a little more complicated if you're not using 38400 baud.  This is
because a reset actually consists of two steps: sending the reset command, and
waiting for the camera to output "Init end\r\n" thus signaling that it is okay
to resume operations.

The issue is that the camera returns to the default 38400 baud prior to
outputting "Init end", so you must reconfigure your serial object accordingly:

    camera.resetWithoutWaitingForInitEnd();
    camera_serial.begin(38400);
    camera.waitForInitEnd();

### Setting the Image Size

Call `setImageSize` with one of the following values to change the image size
used to encode pictures:

* `LS101::Small` (160x120)
* `LS101::Medium` (320x240)
* `LS101::Large` (640x480)

You must reset the camera for the image size change to take effect:

    camera.setImageSize(LS101::Small);
    camera.reset();

### Setting the Compression Ratio

Call `setCompressionRatio` with a value between between 0 (low compression,
high quality) and 0xFF (high compression, low quality.)

Note that resetting the camera returns the compression ratio to its default
value, so if you need to change both the image size and compression ratio, you
must change the compression ratio after performing the reset:

    camera.setImageSize(LS101::Large);
    camera.reset();
    camera.setCompressionRatio(0x20);

### Reading the JPEG File Size

`readJpegFileSize` returns size, in bytes, of the most recently taken picture:

    camera.takePicture();
    uint16_t size = camera.readJpegFileSize();
