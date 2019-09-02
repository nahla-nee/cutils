#ifndef CUTILS_TCP_CLIENT_H
#define CUTILS_TCP_CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#ifndef CUTILS_NO_LIBEVENT
#include <event2/event.h>
#endif

#include "str.h"
#include "bytestream.h"

typedef struct cutilsTcpClient{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;
	#endif

	cutilsString server;
	cutilsByteStream buffer;
	bool connected;
} cutilsTcpClient;

int cutilsTcpClientInit(cutilsTcpClient *client, size_t bufferSize);
cutilsTcpClient* cutilsTcpClientNew(size_t bufferSize);
void cutilsTcpClientDeinit(cutilsTcpClient *client);
void cutilsTcpClientFree(cutilsTcpClient *client);

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address, const char *port, event_callback_fn callback);
#else
int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address, const char *port);
#endif
void cutilsTcpClientDisconnect(cutilsTcpClient *client);

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpClientStartEventLoop(cutilsTcpClient *client);
int cutilsTcpClientStopEventLoop(cutilsTcpClient *client);
int cutilsTcpClientForceStopEventLoop(cutilsTcpClient *client);
#endif

#ifndef CUTILS_NO_LIBEVENT
void cutilsTcpClientSetTimeout(cutilsTcpClient *client, time_t sec, suseconds_t usec);
void ctuilsTcpClientClearTimeout(cutilsTcpClient *client);
#endif

#endif
