#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include "CommandsStore.h"
#include "MessageBuilder.h"

using namespace std;

namespace Rhoban
{
  CommandsStore::CommandsStore()
  {
    builder=new MessageBuilder;
  }

  CommandsStore::~CommandsStore()
  {
    delete(builder);
  }

  MessageBuilder *CommandsStore::getBuilder()
  {
    return builder;
  }
}
