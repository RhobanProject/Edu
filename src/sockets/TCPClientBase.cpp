#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <fcntl.h>
#endif
#include <errno.h>
#include <string>

#include <sockets/TCPClientBase.h>

using namespace std;

namespace Rhoban
{
    int TCPClientBase::receive(char *buffer, int size)
    {
        int n = recv(clientSocket, buffer, size, 0);

        if (n == 0) {
            throw string("Connection closed");
        }

#ifdef WIN32
		if (n < 0 && errno != EAGAIN && errno != WSAEWOULDBLOCK ) {
#else 
        if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
#endif
            throw string("Error while receiving data");
        }

        return n;
    }

    int TCPClientBase::transmit(const char *buffer, int size)
    {
        int n = send(clientSocket, buffer, size, 0);

#ifdef WIN32
		if (n < 0 && errno != EAGAIN && errno != WSAEWOULDBLOCK ) {
#else 
        if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
#endif
            throw string("Error while sending data");
        }

        return n;
    }

    void TCPClientBase::setBlocking(int blocking)
    {
#ifdef WIN32
        unsigned long arg;

        arg = blocking ? 1 : 0;

        ioctlsocket(clientSocket, FIONBIO, &arg); 
#else
        int flags;

        flags = fcntl(clientSocket, F_GETFL);

        if (blocking) {
            fcntl(clientSocket, F_SETFL, flags & ~O_NONBLOCK);   
        } else {
            fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK); 
        }
#endif
    }
}
