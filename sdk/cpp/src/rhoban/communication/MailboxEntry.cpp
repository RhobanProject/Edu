#include <cstdlib>
#include <cstdio>
#include <map>
#include <ctime>
#include "MailboxEntry.h"

using namespace std;

namespace Rhoban
{
  MailboxEntry::MailboxEntry(ui32 uid, Condition *condition)
  {
    this->uid = uid;
    waiting = condition;
    response = NULL;
    callback = NULL;
    creationDate = time(NULL);    
  }

  MailboxEntry::MailboxEntry(ui32 uid, sendCallback *callback)
  {
    this->uid = uid;
    waiting = NULL;
    response = NULL;
    this->callback = callback;
    creationDate = time(NULL);    
  }

  MailboxEntry::~MailboxEntry()
  {
    delete response;
    delete waiting;
  }

  void MailboxEntry::wait(int timeout)
  {
    waiting->wait(new Mutex, timeout);
  }

  int MailboxEntry::isWaiting()
  {
    return(waiting != NULL);
  }

  int MailboxEntry::isCallback()
  {
    return(callback != NULL);
  }
  
  void MailboxEntry::executeCallback(Message *message)
  {
    callback(message);
  }

  ui32 MailboxEntry::getUid()
  {
    return uid;
  }
  
  time_t MailboxEntry::getCreationDate()
  {
    return creationDate;
  }

  Message * MailboxEntry::getResponse()
  {
    return response;
  }

  void MailboxEntry::setResponse(Message * message)
  {
    response=message;
  }

  void MailboxEntry::broadcast()
  {
    waiting->broadcast();
  }
}
