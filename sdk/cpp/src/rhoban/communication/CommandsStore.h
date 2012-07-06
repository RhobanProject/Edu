#ifndef _COMMANDSSTORE_H
#define _COMMANDSSTORE_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include "MessageBuilder.h"

using namespace std;

namespace Rhoban
{
  class CommandsStore
  {
  public:
    CommandsStore();
    ~CommandsStore();
    MessageBuilder *builder;
  };
}

#endif //_COMMANDSTORE_H
