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
    store=new MessageBuilder;
  }

  CommandsStore::~CommandsStore()
  {
    delete(store);
  }
}
