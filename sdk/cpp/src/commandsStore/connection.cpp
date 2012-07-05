#include <cstdlib>
#include <cstdio>
#include "msg.h"
#include "connection.h"


using namespace std;

namespace Rhoban{
  void Connection::sendMessage(Msg *msg)
  {
    transmit(msg->getRaw(), msg->buffer_size);
  }

  Msg* Connection::getMessage(){
    Msg* msg=new Msg;
    
    receive(msg->buffer, 4);
    

    return msg;
  }  
}
