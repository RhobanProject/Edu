#ifndef MAILBOX_ENTRY_H
#define MAILBOX_ENTRY_H

#include <cstdlib>
#include <cstdio>
#include <map>
#include <ctime>
#include <rhoban/types.h>
#include <threading/Condition.h>
#include "Message.h"

using namespace std;

namespace Rhoban
{
  typedef void sendCallback(Message *);

  class MailboxEntry
  {
  public:
    MailboxEntry(ui32 uid, Condition *condition);
    MailboxEntry(ui32 uid, sendCallback *callback);
    ~MailboxEntry();
    void wait(int timeout);
    int isWaiting();
    int isCallback();
    void executeCallback(Message *message);
    ui32 getUid();
    time_t getCreationDate();
    Message * getResponse();
    void setResponse(Message * message);
    void broadcast();
  protected:
    Message * response;
    Condition * waiting;
    sendCallback* callback;
    ui32 uid;
    time_t creationDate;
  };
}

#endif // MAILBOX_ENTRY_H
