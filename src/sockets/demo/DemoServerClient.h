#ifndef _DEMOSERVERCLIENT_H
#define _DEMOSERVERCLIENT_H

#include <sockets/TCPServerClient.h>

namespace Rhoban
{
    class DemoServerClient : public TCPServerClient
    {
        public:
            DemoServerClient(int socket);

            void loop();
    };
}

#endif //  _DEMOSERVERCLIENT_H
