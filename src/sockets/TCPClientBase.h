#ifndef _TCPCLIENTBASE_H
#define _TCPCLIENTBASE_H

#include <sockets/common.h>

namespace Rhoban
{
    class TCPClientBase
    {
        public:
            /**
             * Sends a buffer
             */
            int transmit(const char *buffer, int size);

            /**
             * Receive data
             */
            int receive(char *buffer, int size);

            /**
             * Change to block mode / non block mode
             */
            void setBlocking(int blocking);

        protected:
            /**
             * Internal socket
             */
            SOCKET clientSocket;
    };
}

#endif // _TCPCLIENTBASE_H
