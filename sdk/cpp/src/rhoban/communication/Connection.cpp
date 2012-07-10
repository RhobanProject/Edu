#include <cstdlib>
#include <cstdio>
#include <string>
#include "Message.h"
#include "Connection.h"
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
  Connection::Connection():mailbox(this){}

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
    Message * retval;
    mailbox.addEntry(new MailboxEntry(message->uid, new Condition));
    sendMessage(message);
    mailbox.wait(message->uid, timeout);
    retval = mailbox.getResponse(message->uid);
    return retval;
  }
  
  void Connection::sendMessageCallback(Message *message, sendCallback *callback)
  {
    mailbox.addEntry(new MailboxEntry(message->uid, callback));
    sendMessage(message);
  }
}
