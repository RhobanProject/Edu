#ifndef HEADER_H_
#define HEADER_H_

#include <string>
#include <vector>

#include "types.h"
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


#endif // HEADER_H_
