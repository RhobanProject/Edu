/*
 * msg.h
 *
 *  Created on: 1 oct. 2010
 *      Author: hugo
 */

#ifndef MSG_H_
#define MSG_H_


#include <string>
#include <vector>

#include "types.h"

#include "rhobanProtocol.h"
#include "Buffer.h"

using namespace std;

#define MSG_TYPE_MAX_NB 8
#define MSG_HEADER_SIZE 4*sizeof(ui32)
#define MSG_MIN_SIZE MSG_HEADER_SIZE + 32
#define MSG_MAX_SIZE 2000000


/*!
*\brief the Msg struct encapsulates messages
* there is no constraint on the length a priori,
* this may be used to send pictures as well as short commands
*
* the type variable is used to dispatch messages to ressources like
* module, low_level, mid_level, peripherals etc
*/

class Header
{
public:
	ui32 timestamp; // timestamp to uniquely identify message
	ui32 type; // type of message
	ui32 subtype; // subtype of message
	ui32 length; // content length of the message


	Header();
	void clear();

	/*!
	* reads header from a char * of size MSG_HEADER_SIZE
	*/
	void read_header(const char * raw_header);
	/*!
	* writes header to a char of size MSG_HEADER_SIZE
	*/
	void write_header(char * raw_header);


	static const ui32 timestamp_offset = 0;
	static const ui32 type_offset = 4;
	static const ui32 subtype_offset = 8;
	static const ui32 length_offset = 12;

protected:
	static ui32 _timestamp; // internal counter of timestamp

};



class Msg : public Header, public Buffer
{
private:

	int cursor; // internal cursor (to read data)

public:

	Msg();
	virtual ~Msg();
	/**
	@brief build a new message
	@param t type of the message (MSG_TYPE_?)
	@param size size of the buffer
	@param buffer pointer of the buffer to use
	*/
	Msg(ui32 type, ui32 size, char * buffer );


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
	friend Msg & operator<< (Msg & msg , ui8 & val );
	friend Msg & operator<< (Msg & msg , ui32 & val );
	friend Msg & operator<< (Msg & msg , int & val );
	friend Msg & operator<< (Msg & msg , float & val );
	friend Msg & operator<< (Msg & msg , double & val );
	friend Msg & operator<< (Msg & msg , string & val );
	friend Msg & operator<< (Msg & msg , vector<double> & val );
	friend Msg & operator<< (Msg & msg , vector<float> & val );
	friend Msg & operator<< (Msg & msg , vector<int> & val );
	friend Msg & operator<< (Msg & msg , vector<uint> & val );

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

};

#endif /* MSG_H_ */



