#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

#include "header.h"
#include "Buffer.h"
 
using namespace std;

ui32 Header::_timestamp = 0;


Header::Header()
{
  clear();
  timestamp = _timestamp++;
}

void Header::read_header(const char * raw_header)
{
  timestamp = decode_uint(raw_header + timestamp_offset); // timestamp to uniquely identify message
  type = decode_uint(raw_header + type_offset);; // type of message
  subtype = decode_uint(raw_header + subtype_offset); // subtype of message
  length = decode_uint(raw_header + length_offset); // content length of the message
}

void Header::write_header(char * raw_header)
{
  encode_uint(timestamp, (char*) raw_header);
  encode_uint(type, (char*) raw_header + type_offset);
  encode_uint(subtype, (char*) raw_header + subtype_offset);
  encode_uint(length, (char*) raw_header + length_offset);
}

void Header::clear()
{
  timestamp =0;
  type=0;
  subtype=0;
  length = 0;
}
