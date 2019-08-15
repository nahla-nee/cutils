#ifndef CUTILS_TCP_SERVER_H
#define CUTILS_TCP_SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "str.h"
#include "bytestream.h"
#include "dynArray.h"


typedef struct cutilsTcpServerClient{
	int sockfd;

	cutilsString address;
	cutilsByteStream buffer;
} cutilsTcpServerClient;

CUTILS_DEF_DYNARRAY_H(cutilsTcpServerClient, cutilsTcpServerClientArr);

typedef struct cutilsTcpServer{
	int sockfd;
	cutilsTcpServerClientArr clients;

	int backlog;
} cutilsTcpServer;

int cutilsTcpServerInit(cutilsTcpServer *server, const char *service, int backlog);
void cutilsTcpServerFree(cutilsTcpServer *server);

#endif
