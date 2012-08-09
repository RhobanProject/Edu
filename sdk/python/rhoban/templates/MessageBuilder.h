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
#ifndef _MESSAGE_BUILDER_H
#define _MESSAGE_BUILDER_H

#include <string>
#include <vector>

#include <communication/types.h>
#include <communication/Message.h>

namespace Rhoban
{
  class MessageBuilder
  {
  public:
    MessageBuilder();

<METHODS_PROTOTYPES>

    protected:
    ui32 uid;

    ui32 getUid();
  };
}

#endif // _MESSAGE_BUILDER_H
