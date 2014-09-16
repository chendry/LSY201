#!/usr/bin/env ruby

require 'serialport'

serial = SerialPort.new("/dev/tty.usbmodem3a21", 38400)
buffer = []

while true
  buffer << serial.readbyte
  buffer = buffer.last(2)

  if buffer == [ 0xFF, 0xD8 ]
    while buffer.last(2) != [ 0xFF, 0xD9 ]
      buffer << serial.readbyte
    end

    File.open("test.jpg", "w") do |f|
      f.write buffer.pack("C*")
    end

    puts "wrote #{buffer.length} bytes"
    buffer = []
  end
end

