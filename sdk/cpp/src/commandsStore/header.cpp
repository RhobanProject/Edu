#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <string>
#include <unistd.h>
#include <iostream>

#include "header.h"
#include "Buffer.h"
 
using namespace std;


namespace Rhoban{
  
  ui32 Header::_timestamp = 0;
  
  Header::Header()
  {
    clear();
    uid = _timestamp++;
  }
  
  void Header::read_header(const char * raw_header)
  {
    uid = encodings::decode_uint(raw_header + uid_offset); // timestamp to uniquely identify message
    destination = encodings::decode_uint(raw_header + destination_offset);; // type of message
    command = encodings::decode_uint(raw_header + command_offset); // subtype of message
    length = encodings::decode_uint(raw_header + length_offset); // content length of the message
  }

  void Header::write_header(char * raw_header)
  {
    encodings::encode_uint(uid, (char*) raw_header + uid_offset);
    encodings::encode_uint(destination, (char*) raw_header + destination_offset);
    encodings::encode_uint(command, (char*) raw_header + command_offset);
    encodings::encode_uint(length, (char*) raw_header + length_offset);
  }

  void Header::clear()
  {
    uid=0;
    destination=0;
    command=0;
    length=0;
  }
}
