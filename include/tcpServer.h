#ifndef CUTILS_TCP_SERVER_H
#define CUTILS_TCP_SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "bytestream.h"
#include "dynArray.h"
#include "tcpServerClient.h"

typedef struct cutilsTcpServer{
	int sockfd;
	cutilsTcpServerClientArr clients;

	int backlog;
} cutilsTcpServer;

int cutilsTcpServerInit(cutilsTcpServer *server, const char *service, int backlog);
cutilsTcpServer* cutilsTcpServerNew(const char *service, int backlog);
void cutilsTcpServerDeinit(cutilsTcpServer *server);
void cutilsTcpServerFree(cutilsTcpServer *server);

#endif
