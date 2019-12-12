#ifndef CUTILS_TCP_CLIENT_H
#define CUTILS_TCP_CLIENT_H

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
#include <string.h>
#include <stdbool.h>

#include <event2/event.h>

#include "str.h"

typedef struct cutilsTcpClient{
	int sockfd;

	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	cutilsString serverAddr;
	bool connected;
} cutilsTcpClient;

int cutilsTcpClientInit(cutilsTcpClient *client);
void cutilsTcpClientDeinit(cutilsTcpClient *client);

int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address,
	const char *port, event_callback_fn callback);
void cutilsTcpClientDisconnect(cutilsTcpClient *client);

int cutilsTcpClientStartEventLoop(cutilsTcpClient *client);
int cutilsTcpClientStopEventLoop(cutilsTcpClient *client);
int cutilsTcpClientForceStopEventLoop(cutilsTcpClient *client);

void cutilsTcpClientSetTimeout(cutilsTcpClient *client, time_t sec, suseconds_t usec);
void cutilsTcpClientClearTimeout(cutilsTcpClient *client);

#endif
