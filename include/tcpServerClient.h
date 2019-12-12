#ifndef CUTILS_TCP_SERVER_CLIENT_H
#define CUTILS_TCP_SERVER_CLIENT_H

#ifndef _WIN32
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <event2/event.h>

#include "errors.h"
#include "str.h"

typedef struct cutilsTcpServerClient{
	int sockfd;

	struct event *ev;

	cutilsString clientAddress;
} cutilsTcpServerClient;

int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct sockaddr address,
	struct event_base *eb, struct timeval *timeout, event_callback_fn callback);
void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client);

#endif
