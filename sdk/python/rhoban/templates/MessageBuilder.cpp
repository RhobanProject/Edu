/**
 * /!\ Attention /!\
 *
 * Ce fichier est généré automatiquement à l'aide du command store
 *
 * Toute modification pourra être perdue
 */
#include <commandsStore/MessageBuilder.h>
#include <commandsStore/commandsStore.h>

namespace Rhoban
{
    MessageBuilder::MessageBuilder(CommandsStore *store_)
    {
        store = store_;
        uid = 0;
    }

    MessageBuilder::getUid()
    {
        return uid++;
    }

    <METHODS>
}
