#include <cstdlib>
#include <cstdio>
#include <threading/Condition.h>
#include <threading/Mutex.h>
#include <threading/Thread.h>
#include "Connection.h"
#include "Mailbox.h"
#include "MailboxEntry.h"

#define GARBAGETIMER 300
#define GARBAGECHECKRATE 100

using namespace std;

namespace Rhoban
{
  Mailbox::Mailbox(Connection* connection) 
  {
    this->connection = connection;
    garbageCounter = 0;
  }

  void Mailbox::execute()
  {
    Message *message;
    while(connection->isConnected())
      {
	message = connection->getMessage();
	if(entries.count(message->uid))
	  {
	    if(entries[message->uid]->isWaiting())
	      {
		setResponse(message->uid, message);
		broadcastCondition(message->uid);
	      }
	    else if(entries[message->uid]->isCallback())
	      {
		entries[message->uid]->executeCallback(message);
		deleteEntry(message->uid);
	      }
	  }
      }
  }

  void Mailbox::deleteEntry(ui32 uid)
  {
    entries.erase(uid);
  }

  void Mailbox::wait(ui32 uid, int timeout)
  {
    entries[uid]->wait(timeout);
  }

  void Mailbox::setResponse(ui32 uid, Message * message)
  {
    entries[uid]->setResponse(message);
  }

  Message * Mailbox::getResponse(ui32 uid)
  {
    return(entries[uid]->getResponse());
  }

  void Mailbox::broadcastCondition(ui32 uid)
  {
    entries[uid]->broadcast();
  }
  
  //mailbox.addEntry(new MailboxEntry(message->uid, new Condition));
  //mailbox.entries[message->uid]=new MailboxEntry(message->uid, new Condition);

  void Mailbox::addEntry(MailboxEntry *entry)
  {
    entries[entry->getUid()]=entry;
    garbageCounter++;
    if(garbageCounter >= GARBAGECHECKRATE)
      {
	garbageCounter = 0;
	garbageCollector();
      }
  }
  
  void Mailbox::garbageCollector()
  {
    map<ui32, MailboxEntry *>::iterator it;
    for(it=entries.begin(); it!=entries.end(); ++it)
      {
	if(time(NULL) - it->second->getCreationDate() > GARBAGETIMER)
	  deleteEntry(it->second->getUid());
      }
  }
  
}
