#ifndef CUTILS_TCP_SERVER_CLIENT_H
#define CUTILS_TCP_SERVER_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef CUTILS_NO_LIBEVENT
#include <event2/event.h>
#endif

#include "errors.h"
#include "str.h"
#include "bytestream.h"
#include "linkedList.h"

typedef struct cutilsTcpServer cutilsTcpServer;
typedef struct cutilsTcpServerClientLLNode cutilsTcpServerClientLLNode;

typedef struct cutilsTcpServerClient{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event *ev;
	#endif

	cutilsTcpServer *server;
	cutilsString address;
	cutilsByteStream buffer;

	cutilsTcpServerClientLLNode *node;
} cutilsTcpServerClient;

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen, event_callback_fn callback);
#else
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen);
#endif
void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client);

CUTILS_DEF_LINKED_LIST_H(cutilsTcpServerClient, cutilsTcpServerClientLL);

void cutilsTcpServerClientLLRemoveCallback(cutilsTcpServerClientLLNode *node, size_t count, void *userData);

#endif
