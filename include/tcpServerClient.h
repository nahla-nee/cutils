#ifndef CUTILS_TCP_SERVER_CLIENT_H
#define CUTILS_TCP_SERVER_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "errors.h"
#include "str.h"
#include "bytestream.h"

typedef struct cutilsTcpServerClient{
	int sockfd;

	cutilsString address;
	cutilsByteStream buffer;
} cutilsTcpServerClient;

int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, size_t bufferSize);
void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client);

struct cutilsTcpServerClientArr;
void cutilsTcpServerClientArrDeinitCallback(struct cutilsTcpServerClientArr *arr, void *userData);

CUTILS_DEF_DYNARRAY_H(cutilsTcpServerClient, cutilsTcpServerClientArr);

#endif
