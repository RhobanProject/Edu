#include <cstdlib>
#include <cstdio>
#include <threading/Condition.h>
#include <threading/Mutex.h>
#include <threading/Thread.h>
#include "Connection.h"
#include "Mailbox.h"

using namespace std;

namespace Rhoban
{
  Mailbox::Mailbox(Connection* connection) 
  {
    this->connection = connection;
  }

  void Mailbox::execute()
  {
    Message *message;
    while(connection->isConnected())
      {
	message = connection->getMessage();
	if(waiting.count(message->uid))	  
	  {
	    response[message->uid]=message;
	    waiting[message->uid]->broadcast();
	    waiting.erase(message->uid);
	  }
	else if(callback.count(message->uid))
	  {
	    callback[message->uid](message);
	    callback.erase(message->uid);
	  }
      }
  }

}
