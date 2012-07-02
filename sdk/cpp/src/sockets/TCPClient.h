#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include <sockets/common.h>
#include <sockets/TCPClientBase.h>

namespace Rhoban
{
    class TCPClient : public TCPClientBase
    {
        public:
            TCPClient();
            ~TCPClient();

            /**
             * Try to connect to address:port
             */
            void connectTo(const char *address, int port);

            /**
             * Closes the connection
             */
            void stop();

            /**
             * Is the connection established ?
             */
            bool isConnected();

        protected:
            /**
             * State
             */
            bool connected;
    };
}

#endif //  _TCPCLIENT_H
