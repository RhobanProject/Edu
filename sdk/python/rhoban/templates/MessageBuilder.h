/**
 * /!\ Attention /!\
 *
 * Ce fichier est généré automatiquement à l'aide du command store
 *
 * Toute modification pourra être perdue
 */
#ifndef _MESSAGE_BUILDER_H
#define _MESSAGE_BUILDER_H

#include <string>
#include <vector>
#include <rhoban/types.h>
#include "CommandsStore.h"
#include "Message.h"


namespace Rhoban
{
  class MessageBuilder
  {
  public:
    MessageBuilder();

<METHODS_PROTOTYPES>

    protected:
    CommandsStore *store;
    ui32 uid;

    ui32 getUid();
  };
}

#endif // _MESSAGE_BUILDER_H
