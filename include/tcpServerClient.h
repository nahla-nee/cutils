#ifndef CUTILS_TCP_SERVER_CLIENT_H
#define CUTILS_TCP_SERVER_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#ifndef CUTILS_NO_LIBEVENT
#include <event2/event.h>
#endif

#include "errors.h"
#include "str.h"
#include "bytestream.h"

struct cutilsTcpServer;

typedef struct cutilsTcpServerClient{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event *ev;
	#endif

	struct cutilsTcpServer *server;
	cutilsString address;
	cutilsByteStream buffer;
} cutilsTcpServerClient;

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen, event_callback_fn callback);
#else
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen);
#endif
void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client);

void cutilsTcpServerClientArrDeinitCallback(cutilsTcpServerClient *arr, size_t count, void *userData);

CUTILS_DEF_DYNARRAY_H(cutilsTcpServerClient, cutilsTcpServerClientArr);

#endif
