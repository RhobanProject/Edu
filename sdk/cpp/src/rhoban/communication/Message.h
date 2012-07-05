#ifndef MSG_H_
#define MSG_H_

#include <string>
#include <vector>
#include <rhoban/types.h>
#include "Buffer.h"
#include "Header.h"

using namespace std;

namespace Rhoban{
  class Message : public Header, public Buffer
  {
  private:

    int cursor; // internal cursor (to read data)

  public:

    Message();
    virtual ~Message();
    /**
       @brief build a new message
       @param t type of the message (MSG_TYPE_?)
       @param size size of the buffer
       @param buffer pointer of the buffer to use
    */
    Message(ui32 dest, ui32 size, char * buffer );


    /*
     * clears the message
     */
    void clear();

    /*!
     * *************************************************
     * READING DATA
     * *************************************************
     */

    /**
       @brief read int from content base on an internal cursor
    */
    ui32 read_uint(void);
    int read_int(void);

    /**
       @brief read a float from the intenal buffer based on internal cursor
    */
    float read_float(void);
    double read_double(void);

    /**
       @brief read a string from the internal buffer based on internal cursor
    */
    string read_string(void);

    /**
       @brief read a char array from the internal buffer based on internal cursor
    */
    vector<ui8> read_array(void);


    /**
       @brief read a string array from the internal buffer based on internal cursor
    */
    vector<string> read_string_array(void);

    /**
       @brief read an int array from the internal buffer based on internal cursor
    */
    vector<ui32> read_uint_array(void);
    vector<int> read_int_array(void);

    /**
       @brief read a float array from the internal buffer based on internal cursor
    */
    vector<float> read_float_array(void);
    vector<double> read_double_array(void);

    /*!
     * *************************************************
     * WRITING DATA
     * *************************************************
     */


    /*!
     * append data at the end of the buffer
     */
    void append(ui8 value);

    void append(ui32 value);

    void append(int value);

    void append(float value);

    void append(double value);

    /*! append data using stream operator */
    friend Message & operator<< (Message & msg , ui8 & val );
    friend Message & operator<< (Message & msg , ui32 & val );
    friend Message & operator<< (Message & msg , int & val );
    friend Message & operator<< (Message & msg , float & val );
    friend Message & operator<< (Message & msg , double & val );
    friend Message & operator<< (Message & msg , string & val );
    friend Message & operator<< (Message & msg , vector<double> & val );
    friend Message & operator<< (Message & msg , vector<float> & val );
    friend Message & operator<< (Message & msg , vector<int> & val );
    friend Message & operator<< (Message & msg , vector<uint> & val );

    /*!
     * append data to the message
     * a copy is performed
     */
    void append(const char * data, ui32 siz);

    /**
       @brief appends a string to a message
       first, a ui32 is written which represents the length of the array
       then, the array itself is writtent in the message
    */
    void append(const string &value);

    /** same but less efficient
	first, a ui32 is written which represents the number of strings
	then, each char is written using append
    */
    void append(vector<ui8> & values);

    /**
       @brief appends an array of strings to a message
       first, a ui32 is written which represents the number of strings
       then, each string is written using append_string
    */
    void append(vector<string> & words);

    void append(vector<ui32> & values);
    void append(vector<int> & values);

    void append(vector<double> & values);
    void append(vector<float> & values);

    void append(vector<vector<double> > & values);
    void append(vector<vector<float> > & values);
    void append(vector<vector<uint> > & values);

    /**
       @brief print the message (type, subtype, timestamp and length)
    */
    void print(void);

    /**
       @brief print the message byte per byte with the %d format
    */
    void rawprint(void);

    char* getRaw();
  };
}

#endif // MSG_H_
