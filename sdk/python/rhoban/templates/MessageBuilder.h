/**
 * /!\ Attention /!\
 *
 * Ce fichier est généré automatiquement à l'aide du command store
 *
 * Toute modification pourra être perdue
 */
#ifndef _MESSAGE_BUILDER_H
#define _MESSAGE_BUILDER_H

#include <commandsStore/commandsStore.h>

namespace Rhoban
{
    class MessageBuilder
    {
        public:
            MessageBuilder(CommandStore *store_);

<METHODS_PROTOTYPES>

        protected:
            CommandStore *store;
            unsigned int uid;

            unsigned int getUid();
    };
}

#endif // _MESSAGE_BUILDER_H
