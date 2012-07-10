#include <cstdlib>
#include <cstdio>
#include <string>
#include "Message.h"
#include "Connection.h"

using namespace std;

namespace Rhoban
{
  Connection::Connection() : mailbox((Connection *)this)
  {
  }

  void Connection::sendMessage(Message *message)
  {
    transmit(message->getRaw(), message->size);
  }
  
  Message* Connection::getMessage() 
  {
    Message* message = new Message;

    try {
      getMessage(message);
    } catch (string e) {
        delete message;
        throw e;
    }

    return message;
  }  

  Message *Connection::getMessage(Message *message)
  {
    message->clear();

    receiveAll(message->buffer, MSG_HEADER_SIZE);
    message->read_header(message->buffer);

    message->alloc(message->length + MSG_HEADER_SIZE);
    message->size = message->length + MSG_HEADER_SIZE;
    receiveAll(message->buffer + MSG_HEADER_SIZE, message->length);

    return message;
  }
  
  Message *Connection::sendMessangeRecieve(Message *message, int timeout)
  {
    mailbox.waiting[message->uid]=new Condition;
    sendMessage(message);
    mailbox.waiting[message->uid]->wait(new Mutex,timeout);
    if(mailbox.response.count(message->uid))
      return(mailbox.response[message->uid]);
    else
      {
	mailbox.waiting.erase(message->uid);
	return NULL;
      }
  }
  
  void Connection::sendMessageCallback(Message *message, sendCallback *callback)
  {
    mailbox.callback[message->uid]=callback;
    sendMessage(message);
  }
}
