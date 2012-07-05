#ifndef _COMMANDSSTORE_H
#define _COMMANDSSTORE_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include "commandsStore/commandSpecification.h"

using namespace std;

namespace Rhoban
{
  class CommandsStore
  {
  public:
    CommandsStore(string filepath);
    ~CommandsStore();
    CommandSpecification* get(string name);

  protected:
    map<string, CommandSpecification*> store;
  };
}

#endif //_COMMANDSTORE_H
