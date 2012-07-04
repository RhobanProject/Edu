#ifndef _TCPCLIENTBASE_H
#define _TCPCLIENTBASE_H

#include <string>

#include <sockets/common.h>

using namespace std;

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
             * Sends a string
             */
            void transmitString(string str, bool lineTerminates = false);

            /**
             * Transmits a formatted string
             */
            void transmitFormat(const char *format, ...);

            /**
             * Receive data
             */
            int receive(char *buffer, int size);

            /**
             * Receives a string
             */
            string receiveString(bool lineTerminates = false);

            /**
             * Receives a formatted string
             */
            void receiveFormat(const char *format, ...);

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
