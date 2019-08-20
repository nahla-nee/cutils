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

#define CUTILS_TCP_CLIENT_BUFFER_SIZE 1024

typedef struct cutilsTcpClient{
	int sockfd;

	cutilsString server;
	cutilsByteStream buffer;
} cutilsTcpClient;

int cutilsTcpClientInit(cutilsTcpClient *client);
void cutilsTcpClientFree(cutilsTcpClient *client);

int cutilsTcPClientConnect(cutilsTcpClient *client, const char *node, const char *service);
void cutilsTcpClientDisconnect(cutilsTcpClient *client);

#endif
