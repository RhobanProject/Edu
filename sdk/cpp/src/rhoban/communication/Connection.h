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
  class Connection : public TCPClient
  {
  public:
    Connection();
    void sendMessage(Message *message);
    Message *sendMessangeRecieve(Message *message, int timeout=100);
    void sendMessageCallback(Message *message, sendCallback *callback);
    Message *getMessage();
    Message *getMessage(Message *message);
  protected:
    Mailbox mailbox;
  };
}

#endif // __CONNECTION_H_
