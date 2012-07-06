/**
 * /!\ Attention /!\
 *
 * Ce fichier est généré automatiquement à l'aide du command store
 *
 * Toute modification pourra être perdue
 */
#include <rhoban/types.h>
#include "MessageBuilder.h"
#include "Message.h"

namespace Rhoban
{
  MessageBuilder::MessageBuilder()
  {
    uid = 0;
  }

  ui32 MessageBuilder::getUid()
  {
    return uid++;
  }

  <METHODS>
}
