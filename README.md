LSY201
======

LSY201 is an Arduino library to interface with the LinkSprite JPEG TTL Camera.
You can purchase the camera from SparkFun here:

[https://www.sparkfun.com/products/retired/10061](https://www.sparkfun.com/products/retired/10061)

I've attempted to mirror the language in the
[LSY201 documentation](http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/LightImaging/LinkSprite%20JPEG%20Color%20Camera%20Manual.pdf)
and have tried to avoid anything on top of documented calls.

## Basic Usage

Create a LSY201 instance and pass it a stream object configured for the camera.
In this exaple, the RX and TX pins of the camera are attached to pins 2 and 3,
respectively, of the Arduino:

    SoftwareSerial camera_serial(2, 3);
    camera_serial.begin(38400);  /* LSY201's default baud */
    LSY201 camera(camera_serial);

### Debugging Information

Call `setDebugStream` to see logs of the bytes being sent to and received from
the camera:

    camera.setDebugStream(Serial);

Most methods you invoke on the LSY201 object result in some request being sent
to the camera followed by some response being read from the camera.  Setting
the debug stream, as done above, logs these requests and responses so that you
can see what's going on in the serial monitor.


### Reset

You'll need to reset the camera prior to doing anything.  Also, as per the
documentation, you'll need to wait 2-3 seconds prior to taking a picture:

    camera.reset();
    delay(3000);

### Taking the Picture

Call `takePicture` to instruct the camera to take a picture.  This simply
causes the camera to take and store the picture as a JPEG file:

    camera.takePicture();


### Reading the Image

Retrieve the image by making multiple calls to the `readJpegFileContent`.  This
method expects a 0-based offset of where in the JPEG file you want to start
reading, a buffer to write the data, and the size of that buffer.  The method
returns `true` when it has read data, or `false` if there is no more data to be
read because the end of file has been reached.

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
