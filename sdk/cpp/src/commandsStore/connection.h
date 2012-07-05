#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <cstdlib>
#include <cstdio>
#include "msg.h"
#include "sockets/TCPClient.h"

using namespace std;

namespace Rhoban
{
  class Connection : public TCPClient
  {
  public:
    void sendMessage(Msg *msg);
    Msg* getMessage();
  };
}

#endif
