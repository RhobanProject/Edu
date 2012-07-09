#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <string>
#include <unistd.h>
#include <iostream>
#include "Message.h"
#include "Header.h"
#include "Buffer.h"
 
using namespace std;

namespace Rhoban{
  Message::Message()
  {
    alloc(MSG_MIN_SIZE);
    clear();
  }
  //deallocation is done by the buffer destructor
  Message::~Message()
  {

  }
  Message::Message(ui32 dest, ui32 size, char *buffer)
  {
    destination = dest;
    command = 0;
    length = 0;
    cursor = 0;
    if(buffer)
      assign(buffer, 0,size);
    else
      alloc(size);
  }



  /**
     @brief write a string in the internal buffer with an offset (internal cursor)
  */
  void Message::append(const string &value)
  {
    append((uint) value.size());
    append_to_buffer(value.c_str(),value.size());
  }



  /**
     @brief write a string in the internal buffer with an offset (Internal cursor)
  */
  void Message::append(vector<ui8> & values)
  {
    int n = values.size();
    append((ui32) n);
    for(int i=0;i<n;i++)
      {
	append(values[i]);
      }
  }

  /**
     @brief appends an array of strings to a message
     first, a ui32 is written which represents the number of strings
     then, each string is written using append_string
  */
  void Message::append(vector<string> & words)
  {
    int n = words.size();
    append(n);
    for(int i=0;i<n;i++)
      {
	append(words[i]);
      }
  }


  /**
     @brief appends an array of ints to a message
     first, a ui32 is written which represents the number of strings
     then, each string is written using append_string
  */
  void Message::append(vector<ui32> & values)
  {
    int n = values.size();
    append((ui32) n);
    for(int i=0;i<n;i++)
      {
	append(values[i]);
      }
  }
  void Message::append(vector<int> & values)
  {
    int n = values.size();
    append((ui32) n);
    for(int i=0;i<n;i++)
      {
	append(values[i]);
      }
  }
  /**
     @brief appends an array of floats to a message
     first, a ui32 is written which represents the number of strings
     then, each string is written using append_string
  */
  void Message::append(vector<double> & values)
  {
    int n = values.size();
    //cout << "Appending float array length "<<n<<endl;
    append((ui32) n);
    for(int i=0;i<n;i++)
      append(values[i]);
  }

  void Message::append(vector<float> & values)
  {
    int n = values.size();
    //cout << "Appending float array length "<<n<<endl;
    append((ui32) n);
    for(int i=0;i<n;i++)
      append(values[i]);
  }

  void Message::append(vector<vector<double> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Message::append(vector<vector<float> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Message::append(vector<vector<uint> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Message::append(vector<vector<string> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Message::append(ui8 value)
  {
    alloc(size+sizeof(ui8));
    buffer[size]=value;
    size+=sizeof(ui8);
  }

  void Message::append(ui32 value)
  {
    alloc(size+sizeof(ui32));
    Encodings::encode_uint(value,buffer+size);
    size+=sizeof(ui32);
  }

  void Message::append(int value)
  {
    alloc(size+sizeof(ui32));
    Encodings::encode_int(value,buffer+size);
    size+=sizeof(ui32);
  }
  void Message::append(float value)
  {
    alloc(size+sizeof(ui32));
    Encodings::encode_float(value,buffer+size);
    size+=sizeof(ui32);
  }
  void Message::append(double value)
  {
    alloc(size+sizeof(ui32));
    Encodings::encode_double(value,buffer+size);
    size+=sizeof(ui32);
  }

  void Message::append(const char * data, ui32 siz)
  {
    append(siz);
    append_to_buffer(data,siz);
  }

  Message & operator<< (Message & msg , ui8 & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , ui32 & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , int & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , float & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , double & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , string & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , vector<double> & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , vector<float> & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , vector<int> & val ){msg.append(val); return msg;}
  Message & operator<< (Message & msg , vector<uint> & val ){msg.append(val); return msg;}


  ui32 Message::read_uint(void)
  {
    cursor += sizeof(ui32);
    return Buffer::read_uint(cursor-sizeof(ui32));
  }

  int Message::read_int(void)
  {
    cursor += sizeof(ui32);
    return Buffer::read_int(cursor-sizeof(ui32));
  }

  float Message::read_float(void)
  {
    cursor += sizeof(float);
    return Buffer::read_float(cursor-sizeof(float));
  }

  double Message::read_double(void)
  {
    return (double) read_float();
  }

  /**
     @brief read a string from the intenal buffer with an offset (internal cursor)
  */
  string Message::read_string(void)
  {
    ui32 length=read_uint();
    cursor += length;
    return Buffer::read_string(length,cursor-length);
  }

  vector<ui8> Message::read_array(void)
  {
    ui32 length=read_uint();
    cursor += length;
    return Buffer::read_array(length, cursor- length);
  }

  /**
     @brief read a string array from the internal buffer based on internal cursor
  */
  vector<string> Message::read_string_array(void)
  {
    vector<string> strs;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      strs.push_back(read_string());
    return strs;
  }

  vector<ui32> Message::read_uint_array(void)
  {
    vector<ui32> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      {
	values.push_back(read_uint());
      }
    return values;
  }
  vector<int> Message::read_int_array(void)
  {
    vector<int> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      values.push_back(read_int());
    return values;
  }
  vector<float> Message::read_float_array(void)
  {
    vector<float> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      values.push_back(read_float());
    return values;
  }
  vector<double> Message::read_double_array(void)
  {
    vector<double> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      values.push_back(read_double());
    return values;
  }


  /*
   * clears the message
   */
  void Message::clear()
  {
    Header::clear();
    Buffer::clear();

    //buffer overhead used to write header
    size = MSG_HEADER_SIZE;
    cursor = MSG_HEADER_SIZE;
  }

  char* Message::getRaw()
  {
      length = size - MSG_HEADER_SIZE;
    write_header(buffer);
    return buffer;
  }

  void Message::print(void)
  {
    printf(" { ");
    printf("Destination : %d, Command : %d, UID : %d, Length : %d }\n",
	   destination, command, uid, length);
  }

  void Message::rawprint(void)
  {
    for(ui32 i=0;i<size/4;i++)
      {
	printf("%d ", Encodings::decode_uint(buffer+4*i));
      }
    printf("\n");
  }
}
  
