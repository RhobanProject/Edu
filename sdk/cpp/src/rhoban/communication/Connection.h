#ifndef _CONNECTION_H_
#define _CONNECTION_H_

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
    void sendMessage(Message *message);
    Message *sendMessangeAndRecieve(Message *message, int timeout=100);
    void sendMessageCallback(Message *message, sendCallback *callback);
    Message *getMessage();
    Message *getMessage(Message *message);
  protected:
    Mailbox *mailbox;
  };
}

#endif // _CONNECTION_H_
