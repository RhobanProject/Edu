#include <cstdlib>
#include <cstdio>
#include <string>
#include "Message.h"
#include "Connection.h"

using namespace std;

namespace Rhoban{
  void Connection::sendMessage(Message *message)
  {
    transmit(message->getRaw(), message->size);
  }

  Message* Connection::getMessage() {
    Message* message = new Message;

    try {
      getMessage(message);
    } catch (string e) {
        delete message;
        throw e;
    }

    return message;
  }  

  Message *Connection::getMessage(Message *message) {
    message->clear();

    receiveAll(message->buffer, MSG_HEADER_SIZE);
    message->read_header(message->buffer);

    message->alloc(message->length + MSG_HEADER_SIZE);
    message->size = message->length + MSG_HEADER_SIZE;
    receiveAll(message->buffer + MSG_HEADER_SIZE, message->length);

    return message;
  }
  
  Message *sendMessangeAndRecieve(Message *message, int timeout=100){
    return(new Message);
  }
  
  void sendMessageCallback(Message *message, sendCallback *callback){
    
  }
}
