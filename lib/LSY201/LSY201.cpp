
#include <Arduino.h>
#include "LSY201.h"
#include "NullStream.h"

const uint8_t TX_RESET[] = { 0x56, 0x00, 0x26, 0x00 };
const uint8_t RX_RESET[] = { 0x76, 0x00, 0x26, 0x00 };

const uint8_t TX_TAKE_PICTURE[] = { 0x56, 0x00, 0x36, 0x01, 0x00 };
const uint8_t RX_TAKE_PICTURE[] = { 0x76, 0x00, 0x36, 0x00, 0x00 };

const uint8_t TX_READ_JPEG_FILE_SIZE[] = { 0x56, 0x00, 0x34, 0x01, 0x00 };
const uint8_t RX_READ_JPEG_FILE_SIZE[] = { 0x76, 0x00, 0x34, 0x00, 0x04, 0x00, 0x00 };

const uint8_t TX_READ_JPEG_FILE_CONTENT[] = { 0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00 };
const uint8_t RX_READ_JPEG_FILE_CONTENT[] = { 0x76, 0x00, 0x32, 0x00, 0x00 };

const uint8_t TX_STOP_TAKING_PICTURES[] = { 0x56, 0x00, 0x36, 0x01, 0x03 };
const uint8_t RX_STOP_TAKING_PICTURES[] = { 0x76, 0x00, 0x36, 0x00, 0x00 };

const uint8_t TX_SET_COMPRESSION_RATIO[] = { 0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04 };
const uint8_t RX_SET_COMPRESSION_RATIO[] = { 0x76, 0x00, 0x31, 0x00, 0x00 };

const uint8_t TX_SET_IMAGE_SIZE[] = { 0x56, 0x00, 0x31, 0x05, 0x04, 0x01, 0x00, 0x19 };
const uint8_t RX_SET_IMAGE_SIZE[] = { 0x76, 0x00, 0x31, 0x00, 0x00 };

const uint8_t TX_ENTER_POWER_SAVING[] = { 0x56, 0x00, 0x3E, 0x03, 0x00, 0x01, 0x01 };
const uint8_t RX_ENTER_POWER_SAVING[] = { 0x76, 0x00, 0x3E, 0x00, 0x00 };

const uint8_t TX_EXIT_POWER_SAVING[] = { 0x56, 0x00, 0x3E, 0x03, 0x00, 0x01, 0x00 };
const uint8_t RX_EXIT_POWER_SAVING[] = { 0x76, 0x00, 0x3E, 0x00, 0x00 };

const uint8_t TX_CHANGE_BAUD_RATE[] = { 0x56, 0x00, 0x24, 0x03, 0x01 };
const uint8_t RX_CHANGE_BAUD_RATE[] = { 0x76, 0x00, 0x24, 0x00, 0x00 };

LSY201::LSY201(Stream &stream) : _stream(&stream), _debug(&NullStream()) { } 

void LSY201::set_debug_stream(Stream &stream)
{
  _debug = &stream;
}

void LSY201::reset()
{
  discard_all_input();

  tx(TX_RESET, sizeof(TX_RESET));
  rx(RX_RESET, sizeof(RX_RESET));

  char buf[25] = { 0 }, *p = buf;

  while (1)
  {
    *p = read_byte();

    if (*p == '\n')
    {
      _debug->print(buf);

      if (strcmp(buf, "Init end\r\n") == 0)
        break;

      memset(buf, 0, sizeof(buf));
      p = buf;
    }
    else
      p ++;
  }
}

void LSY201::take_picture()
{
  tx(TX_TAKE_PICTURE, sizeof(TX_TAKE_PICTURE));
  rx(RX_TAKE_PICTURE, sizeof(RX_TAKE_PICTURE));
}

uint16_t LSY201::read_jpeg_file_size()
{
  tx(TX_READ_JPEG_FILE_SIZE, sizeof(TX_READ_JPEG_FILE_SIZE));
  rx(RX_READ_JPEG_FILE_SIZE, sizeof(RX_READ_JPEG_FILE_SIZE));

  return (((uint16_t) read_byte()) << 8) | read_byte();
}

bool LSY201::read_jpeg_file_content(uint8_t *buf, uint16_t offset, uint16_t size)
{
  static uint8_t last = 0x00;

  tx(TX_READ_JPEG_FILE_CONTENT, sizeof(TX_READ_JPEG_FILE_CONTENT));

  uint8_t params[] = {
    (offset & 0xFF00) >> 8,
    (offset & 0x00FF),
    0x00,
    0x00,
    (size & 0xFF00) >> 8,
    (size & 0x00FF),
    0x00,
    0x0A
  };

  tx(params, sizeof(params));

  rx(RX_READ_JPEG_FILE_CONTENT, sizeof(RX_READ_JPEG_FILE_CONTENT));

  while (size --)
  {
    *buf++ = read_byte();

    if (last == 0xFF && buf[-1] == 0xD9)
      return false;

    last = buf[-1];
  }

  rx(RX_READ_JPEG_FILE_CONTENT, sizeof(RX_READ_JPEG_FILE_CONTENT));

  return true;
}

void LSY201::stop_taking_pictures()
{
  tx(TX_STOP_TAKING_PICTURES, sizeof(TX_STOP_TAKING_PICTURES));
  rx(RX_STOP_TAKING_PICTURES, sizeof(RX_STOP_TAKING_PICTURES));
}

void LSY201::set_compression_ratio(uint8_t value)
{
  tx(TX_SET_COMPRESSION_RATIO, sizeof(TX_SET_COMPRESSION_RATIO));
  _stream->write(value);

  rx(RX_SET_COMPRESSION_RATIO, sizeof(RX_SET_COMPRESSION_RATIO));
}

void LSY201::set_image_size(Size size)
{
  tx(TX_SET_IMAGE_SIZE, sizeof(TX_SET_IMAGE_SIZE));
  _stream->write((uint8_t) size);
  rx(RX_SET_IMAGE_SIZE, sizeof(RX_SET_IMAGE_SIZE));
}

void LSY201::enter_power_saving()
{
  tx(TX_ENTER_POWER_SAVING, sizeof(TX_ENTER_POWER_SAVING));
  rx(RX_ENTER_POWER_SAVING, sizeof(RX_ENTER_POWER_SAVING));
}

void LSY201::exit_power_saving()
{
  tx(TX_EXIT_POWER_SAVING, sizeof(TX_EXIT_POWER_SAVING));
  rx(RX_EXIT_POWER_SAVING, sizeof(RX_EXIT_POWER_SAVING));
}

void LSY201::set_baud_rate(Baud baud)
{
  _debug->println("resetting");

  tx(TX_CHANGE_BAUD_RATE, sizeof(TX_CHANGE_BAUD_RATE));

  uint8_t params[] = {
    (baud & 0xFF00) >> 8,
    baud & 0x00FF
  };

  tx(params, sizeof(params));

  rx(RX_CHANGE_BAUD_RATE, sizeof(RX_CHANGE_BAUD_RATE));
}

void LSY201::discard_all_input()
{
  while (_stream->available())
    _stream->read();
}

void LSY201::tx(const uint8_t *bytes, uint8_t length)
{
  _debug->print("sending bytes:");

  while (length --)
  {
    _debug->print(" ");
    _debug->print(*bytes, HEX);
    _stream->write(*bytes);

    bytes ++;
  }

  _debug->println("");
}

void LSY201::rx(const uint8_t *bytes, uint8_t length)
{
  _debug->println("expecting bytes:");

  while (length --)
  {
    uint8_t byte = read_byte();

    _debug->print(byte, HEX);
    _debug->print(" ");

    if (byte == *bytes)
      _debug->println("ok");
    else
    {
      _debug->print("expected ");
      _debug->println(*bytes, HEX);
    }

    bytes ++;
  }
}

uint8_t LSY201::read_byte()
{
  while (!_stream->available())
    ;

  return _stream->read();
}
