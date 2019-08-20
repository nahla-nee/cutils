#ifndef CUTILS_TCP_CLIENT_H
#define CUTILS_TCP_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "str.h"
#include "bytestream.h"

typedef struct cutilsTcpClient{
	int sockfd;

	cutilsString server;
	cutilsByteStream buffer;
} cutilsTcpClient;

int cutilsTcpClientInit(cutilsTcpClient *client, size_t bufferSize);
cutilsTcpClient* cutilsTcpClientNew(size_t bufferSize);
void cutilsTcpClientFree(cutilsTcpClient *client);

int cutilsTcPClientConnect(cutilsTcpClient *client, const char *node, const char *service);
void cutilsTcpClientDisconnect(cutilsTcpClient *client);

#endif
