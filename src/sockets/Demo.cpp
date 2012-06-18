#include <cstdlib>
#include <iostream>

#include <sockets/TCPServer.h>
#include <sockets/TCPClient.h>

#include <sockets/DemoServerClient.h>
#include <main/Command.h>

using namespace std;
using namespace Rhoban;

COMMAND_DEFINE(server, "Socket demo server")
{
    TCPServer<DemoServerClient> *server = new TCPServer<DemoServerClient>;

    cout << "*** Creating DemoServer" << endl;
    server->run(1234);
}

COMMAND_DEFINE(client, "Socket demo client")
{
    TCPClient *client = new TCPClient;

    client->connectTo("localhost", 1234);
    client->transmit("Hello!", 6);
    client->stop();
}
