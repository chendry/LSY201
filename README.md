LSY201
======

LSY201 is an Arduino library to interface with the LinkSprite JPEG TTL Camera.
You can purchase the camera from SparkFun here:

[https://www.sparkfun.com/products/11610](https://www.sparkfun.com/products/11610)

I've tried to mirror the 
[LSY201 documentation](http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/LightImaging/LinkSprite%20JPEG%20Color%20Camera%20Manual.pdf)
closely and have not added any additional behavior on top of what is documented
there.

## Usage

Create a LSY201 instance and call `setSerial` to give a it stream object
configured for the camera.  In the following example, it is assumed that the TX
and RX pins of the camera are attached to pins 2 and 3 respectively on the
Arduino:

    SoftwareSerial camera_serial(2, 3);
    camera_serial.begin(38400);  /* LSY201's default baud */
    LSY201 camera;
    camera.setSerial(camera_serial);

### Debugging Information

Call `setDebugSerial` to have additional debug information written to a serial
object:

    camera.setDebugSerial(Serial);

Most of the methods exposed by the LSY201 object send some bytes to the camera
the request) and read some bytes back (the response.)  Setting a debug serial
object causes those request and response bytes to be logged to that serial
object.

For example, the following debug output shows the bytes transmitted and
received while changing the image resolution to 640x480, resetting the camera,
and then taking a picture:

    sending bytes: 56 0 31 5 4 1 0 19
    expecting bytes:
    76 ok
    0 ok
    31 ok
    0 ok
    0 ok
    sending bytes: 56 0 26 0
    expecting bytes:
    76 ok
    0 ok
    26 ok
    0 ok
    Ctrl infr exist
    User-defined sensor
    625
    Init end
    sending bytes: 56 0 36 1 0
    expecting bytes:
    76 ok
    0 ok
    36 ok
    0 ok
    0 ok

### Taking the Picture

Call `takePicture` to instruct the camera to take a picture.  This instructs
the camera to take a picture and store it as a JPEG file which can later be
retrieved.

    camera.takePicture();

### Reading the Image

Retrieve the image data by making multiple calls to the `readJpegFileContent`.
This method expects three parameters:

1. a 0-based offset of where in the JPEG file you want to start reading.  (Must
   be a multiple of 8.)
2. a buffer to which the data is to be written, and
3. the size of that buffer.

The method returns `true` when it has read data, or `false` if there is no more
data to be read because the end-of-file has been reached.

Here's an example of how to read the entire contents of the JPEG and write each
byte to the serial port:

    uint16_t offset = 0;
    uint8_t buf[32];

    while (camera.readJpegFileContent(buf, offset, sizeof(buf)))
    {
      for (int i = 0; i < sizeof(buf); i ++)
        Serial.println(buf[i], HEX);

       offset += sizeof(buf);
    }

### Setting the Image Compression and Size

Call `setImageSize` with one of the following values to set the image size for
the next picture taken:

* `LS101::Size_320x240`
* `LS101::Size_640x480`
* `LS101::Size_160x120`

You must call `reset` before the size change takes effect.

To change the compression ratio, call `setCompressionRatio` with any value
between between 0 (low compression, high quality) and 255 (high compression,
low quality.)

The `reset` method changes the compression ratio back to 0x36, so
`setCompressionRatio` must be called afterwords:

    camera.setImageSize(LS101::Size_320x240);
    camera.reset();
    camera.setCompressionRatio(0x80);

### Changing the Baud Rate

Call `setBaudRate` with one of the following values:

* `LS101::Baud_9600`
* `LS101::Baud_19200`
* `LS101::Baud_38400`
* `LS101::Baud_57600`
* `LS101::Baud_115200`


### Miscellaneous

* `readJpegFileSize` returns the size, in bytes, of the most recently taken
  picture.
* `enterPowerSaving` and `exitPowerSaving` enter and exit power saving mode.
* I'm not sure what `stopTakingPictures` actually does, but it's listed in the
  documentation.
