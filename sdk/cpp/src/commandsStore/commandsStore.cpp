#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include "commandsStore/commandSpecification.h"
#include "commandsStore/commandsStore.h"

using namespace std;

namespace Rhoban
{
  CommandsStore::CommandsStore(string filepath)
  {




  }

  CommandsStore::~CommandsStore()
  {
    map<string, CommandSpecification*>::iterator it;
    for(it=store.begin(); it!=store.end(); ++it)
      	delete (*it).second;
    store.clear();
  }

  CommandSpecification* CommandsStore::get(string name)
  {
    if(!store.count(name))
      throw string("Unknow key.");

    return store[name];
  }
}
