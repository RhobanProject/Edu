#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <cstdlib>
#include <cstdio>
#include <sockets/TCPClient.h>
#include "Message.h"

using namespace std;

namespace Rhoban
{
  class Connection : public TCPClient
  {
  public:
    void sendMessage(Message *message);
    Message *getMessage();
    Message *getMessage(Message *message);
  };
}

#endif
