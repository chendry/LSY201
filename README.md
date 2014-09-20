LSY201
======

LSY201 is an Arduino library to interface with the LinkSprite JPEG TTL Camera.
You can purchase the camera from SparkFun here:

[https://www.sparkfun.com/products/11610](https://www.sparkfun.com/products/11610)

## Usage

Create a LSY201 instance and call `setSerial` to give a it stream object
configured for the camera.  This example assumes that the TX and RX pins of the
camera are attached to pins 2 and 3 respectively of the Arduino:

    SoftwareSerial camera_serial(2, 3);
    camera_serial.begin(38400);  /* LSY201's default baud */
    LSY201 camera;
    camera.setSerial(camera_serial);

### Debugging Information

Call `setDebugSerial` to have additional debug information written to a serial
object:

    camera.setDebugSerial(Serial);

Most of the methods of LSY201 send some bytes to the camera and read some bytes
back.  Those requests and responses are logged to the debug serial object when
set.

For example, the following debug output shows the bytes transmitted and
received while changing the image resolution to 640x480, resetting the camera,
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
did not because the end-of-file had been reached.

For example, the following reads the entire contents of the JPEG in 32-byte
chunks and writes each byte to Serial:

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

You can also use `reset` to clear the buffer, but that has the additional side
effects.

### Changing the Baud Rate

Call `setBaudRate` with one of the supported baud rates: 9600, 19200, 38400,
57600, or 115200.  The camera will be using that baud when the method returns,
so you'll need to reconfigure your serial object accordingly:

    camera.setBaudRate(9600);
    camera_serial.begin(9600);

### Resetting the Camera

Resetting the camera clears the internal buffer and returns the compression
ratio and baud rate to their befault values.

If you are using the default 38400 baud rate, you can simply call `reset`:

    camera.reset();

If you are not using the default 38400 baud rate, you will need to make two calls:

    /* send the reset command which has the side-effect of changing the
     * camera's baud to 38400. */
    camera.resetWithoutWaitingForInitEnd();

    /* change the serial object's baud accordingly. */
    camera_serial.begin(38400);

    /* with the correct baud on the serial object, wait for the camera to
     * output "Init end\r\n" */
    camera.waitForInitEnd();

### Setting the Image Size

Call `setImageSize` with one of the following values to set the image size used
for the next picture taken:

* `LS101::Small` (160x120)
* `LS101::Medium` (320x240)
* `LS101::Large` (640x480)

You must reset the camera for the image size change to take effect.

### Setting the Compression Ratio

Call `setCompressionRatio` with any value between between 0 (low compression,
high quality) and 0xFF (high compression, low quality.)

### Reading the JPEG File Size

`readJpegFileSize` returns size, in bytes, of the most recently taken picture.

### Power Saving Mode


