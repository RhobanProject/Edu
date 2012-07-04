#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

#include "msg.h"
#include "header.h"
#include "Buffer.h"
 
using namespace std;

namespace Rhoban{
  Msg::Msg()
  {
    alloc(MSG_MIN_SIZE);
    clear();
  }
  //deallocation is done by the buffer destructor
  Msg::~Msg()
  {

  }
  Msg::Msg(ui32 t, ui32 size, char *buffer)
  {
    type = t;
    subtype = 0;
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
  void Msg::append(const string &value)
  {
    append((uint) value.size());
    append_to_buffer(value.c_str(),value.size());
  }



  /**
     @brief write a string in the internal buffer with an offset (internal cursor)
  */
  void Msg::append(vector<ui8> & values)
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
  void Msg::append(vector<string> & words)
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
  void Msg::append(vector<ui32> & values)
  {
    int n = values.size();
    append((ui32) n);
    for(int i=0;i<n;i++)
      {
	append(values[i]);
      }
  }
  void Msg::append(vector<int> & values)
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
  void Msg::append(vector<double> & values)
  {
    int n = values.size();
    //cout << "Appending float array length "<<n<<endl;
    append((ui32) n);
    for(int i=0;i<n;i++)
      append(values[i]);
  }

  void Msg::append(vector<float> & values)
  {
    int n = values.size();
    //cout << "Appending float array length "<<n<<endl;
    append((ui32) n);
    for(int i=0;i<n;i++)
      append(values[i]);
  }

  void Msg::append(vector<vector<double> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Msg::append(vector<vector<float> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Msg::append(vector<vector<uint> > & values)
  {
    append((int) values.size());
    for(uint i=0;i<values.size();i++)
      append(values[i]);
  }

  void Msg::append(ui8 value)
  {
    alloc(size+sizeof(ui8));
    buffer[size]=value;
    size+=sizeof(ui8);
  }

  void Msg::append(ui32 value)
  {
    alloc(size+sizeof(ui32));
    encodings::encode_uint(value,buffer+size);
    size+=sizeof(ui32);
  }

  void Msg::append(int value)
  {
    alloc(size+sizeof(ui32));
    encodings::encode_int(value,buffer+size);
    size+=sizeof(ui32);
  }
  void Msg::append(float value)
  {
    alloc(size+sizeof(ui32));
    encodings::encode_float(value,buffer+size);
    size+=sizeof(ui32);
  }
  void Msg::append(double value)
  {
    alloc(size+sizeof(ui32));
    encodings::encode_double(value,buffer+size);
    size+=sizeof(ui32);
  }

  void Msg::append(const char * data, ui32 siz)
  {
    append(siz);
    append_to_buffer(data,siz);
  }

  Msg & operator<< (Msg & msg , ui8 & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , ui32 & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , int & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , float & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , double & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , string & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , vector<double> & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , vector<float> & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , vector<int> & val ){msg.append(val); return msg;}
  Msg & operator<< (Msg & msg , vector<uint> & val ){msg.append(val); return msg;}


  ui32 Msg::read_uint(void)
  {
    cursor += sizeof(ui32);
    return Buffer::read_uint(cursor-sizeof(ui32));
  }

  int Msg::read_int(void)
  {
    cursor += sizeof(ui32);
    return Buffer::read_int(cursor-sizeof(ui32));
  }

  float Msg::read_float(void)
  {
    cursor += sizeof(float);
    return Buffer::read_float(cursor-sizeof(float));
  }

  double Msg::read_double(void)
  {
    return (double) read_float();
  }

  /**
     @brief read a string from the intenal buffer with an offset (internal cursor)
  */
  string Msg::read_string(void)
  {
    ui32 length=read_uint();
    cursor += length;
    return Buffer::read_string(length,cursor-length);
  }

  vector<ui8> Msg::read_array(void)
  {
    ui32 length=read_uint();
    cursor += length;
    return Buffer::read_array(length, cursor- length);
  }

  /**
     @brief read a string array from the internal buffer based on internal cursor
  */
  vector<string> Msg::read_string_array(void)
  {
    vector<string> strs;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      strs.push_back(read_string());
    return strs;
  }

  vector<ui32> Msg::read_uint_array(void)
  {
    vector<ui32> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      {
	values.push_back(read_uint());
      }
    return values;
  }
  vector<int> Msg::read_int_array(void)
  {
    vector<int> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      values.push_back(read_int());
    return values;
  }
  vector<float> Msg::read_float_array(void)
  {
    vector<float> values;
    ui32 length = read_uint();
    for(unsigned int i=0;i<length;i++)
      values.push_back(read_float());
    return values;
  }
  vector<double> Msg::read_double_array(void)
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
  void Msg::clear()
  {
    Header::clear();
    Buffer::clear();

    //buffer overhead used to write header
    size = MSG_HEADER_SIZE;
    cursor=MSG_HEADER_SIZE;
  }




  void Msg::print(void)
  {
    printf(" { ");
    printf("Type : %d, SubType : %d, TimeStamp : %d, Length : %d }\n",
	   type, subtype, timestamp, length);
  }

  void Msg::rawprint(void)
  {
    for(ui32 i=0;i<size/4;i++)
      {
	printf("%d ", encodings::decode_uint(buffer+4*i));
      }
    printf("\n");
  }
}


  
