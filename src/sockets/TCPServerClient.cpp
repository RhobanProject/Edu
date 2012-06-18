#include <iostream>

#include <sockets/TCPServerClient.h>

using namespace std;

namespace Rhoban
{
    TCPServerClient::TCPServerClient(int socket)
    {
        clientSocket = socket;
        dead = false;
    }

    TCPServerClient::~TCPServerClient()
    {
        pthread_cancel(thread);
        pthread_join(thread, NULL);
    }

    bool TCPServerClient::isDead()
    {
        return dead;
    }

    void TCPServerClient::stop()
    {
        close(clientSocket);
        dead = true;
    }

    void TCPServerClient::run()
    {
        pthread_create(&thread, NULL, TCPServerClient::clientThread, (void*)this);
    }

    void *TCPServerClient::clientThread(void *clientPtr)
    {
        TCPServerClient *client = (TCPServerClient *)clientPtr;

        try {
            client->loop();
        } catch (string exception) {
            cout << "Closing client (" << exception << ")" << endl;
        }

        client->stop();

        return NULL;
    }
}
