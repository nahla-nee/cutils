#ifndef CUTILS_TCP_SERVER_H
#define CUTILS_TCP_SERVER_H

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

typedef struct cutilsTcpServer{
	int sockfd;

	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	struct timeval clientTimeout;
	bool useClientTimeout;

	cutilsString port;
	int backlog;
	bool started;
} cutilsTcpServer;

int cutilsTcpServerInit(cutilsTcpServer *server);
void cutilsTcpServerDeinit(cutilsTcpServer *server);

int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog,
	void *usrptr, event_callback_fn callback);
void cutilsTcpServerStop(cutilsTcpServer *server);

int cutilsTcpServerStartEventLoop(cutilsTcpServer *server);
int cutilsTcpServerStopEventLoop(cutilsTcpServer *server);
int cutilsTcpServerForceStopEventLoop(cutilsTcpServer *server);

void cutilsTcpServerSetTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec);
void cutilsTcpServerClearTimeout(cutilsTcpServer *server);

void cutilsTcpServerSetClientTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec);
void cutilsTcpServerClearClientTimeout(cutilsTcpServer *server);

#endif
