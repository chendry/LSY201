
#include <Arduino.h>
#include "LSY201.h"
#include "NullStream.h"

const uint8_t TX_RESET[] = { 0x56, 0x00, 0x26, 0x00 };
const uint8_t RX_RESET[] = { 0x76, 0x00, 0x26, 0x00 };

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

void LSY201::discard_all_input()
{
  while (_stream->available())
    _stream->read();
}

void LSY201::tx(const uint8_t *bytes, uint8_t length)
{
  while (length --)
    _stream->write(*bytes++);
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
