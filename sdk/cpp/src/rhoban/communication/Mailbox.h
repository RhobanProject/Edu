#ifndef MAILBOX_H___
#define MAILBOX_H___

namespace Rhoban {
  class Connection;
  class Mailbox;
}

#include <cstdlib>
#include <cstdio>
#include <map>
#include <threading/Condition.h>
#include <threading/Mutex.h>
#include <threading/Thread.h>
#include "Connection.h"

using namespace std;

namespace Rhoban
{
  typedef void sendCallback(Message *);
  
  class Mailbox : public Thread
  {
  public :
    Mailbox(Connection* connection);
    void execute();
    map<ui32, Message *> response;
    map<ui32, Condition *> waiting;
    map<ui32, sendCallback *> callback;
            
  protected:
    Connection *connection;
  };
}

#endif // MAILBOX_H___
