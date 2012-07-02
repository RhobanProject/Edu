#include <iostream>

#include <sockets/demo/DemoServerClient.h>

using namespace std;

namespace Rhoban
{
    DemoServerClient::DemoServerClient(int socket) : TCPServerClient(socket)
    {
    }

    void DemoServerClient::loop()
    {
        int n;
        char buffer[128];

        cout << "Entering the loop" << endl;

        while (1) {
            n = receive(buffer, sizeof(buffer));
            transmit(buffer, n);
        }
    }
}
