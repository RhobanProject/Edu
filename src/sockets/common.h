#ifndef _NET_H
#define _NET_H

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#define SOCKET int
#endif

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif // _NET_H
