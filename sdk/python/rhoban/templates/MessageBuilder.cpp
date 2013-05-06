/*************************************************
* Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/**
 * /!\ Attention /!\
 *
 * Ce fichier est généré automatiquement à l'aide du command store
 *
 * Toute modification pourra être perdue
 */
#include "MessageBuilder.h"

namespace Rhoban
{
  MessageBuilder::MessageBuilder()
  {
    uid = 0;
  }

  ui32 MessageBuilder::getUid()
  {
      ui32 tmp; 

      mutex.lock();
      tmp = uid++;
      mutex.unlock();

      return tmp;
  }

  <METHODS>
}
