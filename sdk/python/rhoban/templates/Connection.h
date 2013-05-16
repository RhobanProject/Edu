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
#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <string>
#include <vector>

#include <communication/CommandsStore.h>
#include <communication/BaseConnection.h>

using namespace Rhoban;

namespace Rhoban
{
    class Connection : public BaseConnection
    {
        public:
            Connection(CommandsStore *commandsStore_ = NULL);
            virtual ~Connection();

            CommandsStore *commandsStore;

            <METHODS_PROTOTYPES>

        protected:
                bool ownCommandStore;
    };
}

#endif // _CONNECTION_H
