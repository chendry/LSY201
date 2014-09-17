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

Most of the methods of LSY201 send some bytes to the camera the request) and
read some bytes back (the response.)  Setting a debug serial object causes
those request and responses to be logged to that serial object.

For example, the following debug output shows the bytes transmitted and
received while changing the image resolution to 640x480, resetting the camera,
waiting for "Init end," and finally taking a picture:

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

`takePicture` takes a picture and stores it as a JPEG file on the camera
which can later be read back.

    camera.takePicture();

**IMPORTANT**: You must perform a reset before taking another picture.  If you
do not, the second `takePicture` call will be ignored and you will read back
the JPEG data from the first image.

### Reading the Image

Call `readJpegFileContent` repeatedly to read the JPEG image data stored on the
camera.  This method expects three parameters:

1. the 0-based offset of where in the JPEG file you want to start reading.  (Must
   be a multiple of 8.)
2. a buffer to hold the data, and
3. the size of that buffer.

The method returns `true` when it has put new data into the buffer, or `false`
if it has not because the end-of-file has been reached.

For example, the following reads the entire contents of the JPEG and writes
each byte to Serial:

    uint16_t offset = 0;
    uint8_t buf[32];

    while (camera.readJpegFileContent(buf, offset, sizeof(buf)))
    {
      for (int i = 0; i < sizeof(buf); i ++)
        Serial.println(buf[i], HEX);

       offset += sizeof(buf);
    }

### Changing the Baud Rate

Call `setBaudRate` with one of the supported baud rates: 9600, 19200, 38400,
57600, or 115200.  The camera will be using that baud when the method returns,
so you'll need to reconfigure your serial object accordingly.

**IMPORTANT**: Using anything but the default baud will slightly complicate the
camera reset process as detailied below.

### Reset, Baud Rates, and Init End

There are two calls which need to be made to reset the camera:

1. `reset` sends a reset request to the camera.
2. `waitForInitEnd` waits until the camera has output "Init end\r\n".

If you're using the default 38400 baud, a reset is simple:

    camera.reset();
    camera.waitForInitEnd();

However, if you're using another baud, perhaps 9600, you must understand that
calling `reset` returns the baud rate back to the default 38400 prior to
outputting "Init end".  Thus, to perform a reset, you will need to do something
like this:

    camera.reset();

    /* camera is now using 38400 baud, so we must configure the serial object
     * accordingly before we wait for "Init end\r\n" */
    camera_serial.begin(38400);
    camera.waitForInitEnd();

    /* return the baud to 9600 */
    camera.setBaudRate(9600);
    camera_serial.begin(9600);

### Setting the Image Compression and Size

Call `setImageSize` with one of the following values to set the image size used
for the next picture taken:

* `LS101::Small` (160x120)
* `LS101::Medium` (320x240)
* `LS101::Large` (640x480)

You must reset the camera before the image size change takes effect.

To change the compression ratio, call `setCompressionRatio` with any value
between between 0 (low compression, high quality) and 0xFF (high compression,
low quality.)

Resetting the camera returns the compression ratio back to 0x36, so changing
both size and compression should look something like this:

    camera.setImageSize(LSY201::Large);
    camera.reset();
    camera.waitForInitEnd();
    camear.setCompressionRatio(0xFF)

### Miscellaneous

* `readJpegFileSize` returns the size, in bytes, of the most recently taken
  picture.

It's not clear from the documentation what the following commands actually do,
but I have implemented them and have verified that the camera is sending back
the correct, documented response:

* `enterPowerSaving`
* `exitPowerSaving`
* `stopTakingPictures`
