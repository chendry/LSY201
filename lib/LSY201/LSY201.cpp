
#include <Arduino.h>
#include "LSY201.h"
#include "NullStream.h"

const uint8_t TX_RESET[] = { 0x56, 0x00, 0x26, 0x00 };
const uint8_t RX_RESET[] = { 0x76, 0x00, 0x26, 0x00 };

const uint8_t TX_TAKE_PICTURE[] = { 0x56, 0x00, 0x36, 0x01, 0x00 };
const uint8_t RX_TAKE_PICTURE[] = { 0x76, 0x00, 0x36, 0x00, 0x00 };

const uint8_t TX_READ_JPEG_FILE_SIZE[] = { 0x56, 0x00, 0x34, 0x01, 0x00 };
const uint8_t RX_READ_JPEG_FILE_SIZE[] = { 0x76, 0x00, 0x34, 0x00, 0x04, 0x00, 0x00 };

LSY201::LSY201(Stream &stream) : _stream(&stream), _debug(&NullStream()) { } 

void LSY201::setDebugStream(Stream &stream)
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

  _debug->println("reset complete");
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
    delay(1);

  return _stream->read();
}
