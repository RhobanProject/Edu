#ifndef __CONNECTION_H_
#define __CONNECTION_H_

namespace Rhoban{
  class Connection;
  class Mailbox;
}

#include <cstdlib>
#include <cstdio>
#include <sockets/TCPClient.h>
#include "Message.h"
#include "Mailbox.h"

using namespace std;

namespace Rhoban
{  
  typedef void sendCallback(Message *);

  class Connection : public TCPClient
  {
  public:
    Connection();
    void sendMessage(Message *message);
    Message *sendMessageRecieve(Message *message, int timeout=100);
    void sendMessageCallback(Message *message, sendCallback *callback);
    void startMailbox();
    Message *getMessage();
    Message *getMessage(Message *message);
  protected:
    Mailbox mailbox;
  };
}

#endif // __CONNECTION_H_
