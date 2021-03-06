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
#include "Connection.h"

namespace Rhoban
{
	Connection::Connection(CommandsStore *commandsStore_) 
            : BaseConnection(), commandsStore(commandsStore_), ownCommandStore(false)
	{
            if (commandsStore == NULL) {
                commandsStore = new CommandsStore;
                ownCommandStore = true;
            }
	}

        Connection::~Connection()
        {
            if (ownCommandStore) {
                delete commandsStore;
                commandsStore = NULL;
            }
        }

    <METHODS>
}
