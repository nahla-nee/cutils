#ifndef CUTILS_TCP_SERVER_H
#define CUTILS_TCP_SERVER_H

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

typedef struct cutilsTcpServer{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	struct timeval timeoutClient;
	bool useTimeoutClient;
	#endif

	size_t clientInBufferSize;
	size_t clientOutBufferSize;
	cutilsString port;
	int backlog;
	bool started;
} cutilsTcpServer;

int cutilsTcpServerInit(cutilsTcpServer *server, size_t inBufferSize, size_t outBufferSize);
cutilsTcpServer* cutilsTcpServerNew(size_t inBufferSize, size_t outBufferSize);
void cutilsTcpServerDeinit(cutilsTcpServer *server);
void cutilsTcpServerFree(cutilsTcpServer *server);

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog, event_callback_fn callback);
#else
int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog);
#endif
void cutilsTcpServerClose(cutilsTcpServer *server);

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerStartEventLoop(cutilsTcpServer *server);
int cutilsTcpServerStopEventLoop(cutilsTcpServer *server);
int cutilsTcpServerForceStopEventLoop(cutilsTcpServer *server);

void cutilsTcpServerSetTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec);
void cutilsTcpServerClearTimeout(cutilsTcpServer *server);

void cutilsTcpServerSetClientTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec);
void cutilsTcpServerClearClientTimeout(cutilsTcpServer *server);
#endif

#endif
