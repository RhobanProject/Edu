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
    delete(message);
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
  
  Message * Connection::sendMessageRecieve(Message *message, int timeout)
  {
    Message * retval;
    ui32 uid = message->uid;
    
    Condition condition;
    
    mailbox.addEntry(new MailboxEntry(uid, &condition));

    mailbox.lock();
    sendMessage(message);
    mailbox.wait(uid, timeout);
    retval = mailbox.getResponse(uid);
    return retval;
  }
  
  void Connection::sendMessageCallback(Message *message, sendCallback *callback)
  {
    MailboxEntry *entry = new MailboxEntry(message->uid, callback);
    mailbox.addEntry(entry);
    sendMessage(message);
    delete(entry);
  }

  void Connection::startMailbox()
  {
    mailbox.start(NULL);
  }
}
