#include <cstdlib>
#include <cstdio>
#include "Message.h"
#include "Connection.h"

using namespace std;

namespace Rhoban{
  void Connection::sendMessage(Message *message)
  {
    transmit(msg->getRaw(), msg->size);
  }

  Message* Connection::getMessage() {
    Message* message = new Message;

    try {
        getMessage(message)
    } catch (e) {
        delete message;
        throw e;
    }

    return message;
  }  

  void Connection::getMessage(Message *message) {
    message->clear();

    receiveAll(message->buffer, MSG_HEADER_SIZE);
    message->read_header(message->buffer);

    message->alloc(message->length + MSG_HEADER_SIZE);
    receiveAll(message->buffer + MSG_HEADER_SIZE, message->length);
  }
}
