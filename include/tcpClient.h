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

#ifndef CUTILS_NO_LIBEVENT
#include <event2/event.h>
#endif

#include "str.h"

#ifndef CUTILS_NO_LIBEVENT
#define CUTILS_DEF_TCP_CLIENT_STRUCT\
	int sockfd;\
\
	struct event_base *eb;\
	struct event *ev;\
	struct timeval timeout;\
	bool useTimeout;\
\
	cutilsString serverAddr;\
	bool connected;

#define CUTILS_DEF_TCP_CLIENT_H(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client);\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client);\
\
	int STRUCT_NAME##TcpConnect(STRUCT_NAME *client, const char *address,\
	const char *port, event_callback_fn callback);\
	void STRUCT_NAME##TcpDisconnect(STRUCT_NAME *client);\
\
	int STRUCT_NAME##TcpStartEventLoop(STRUCT_NAME *client);\
	int STRUCT_NAME##TcpStopEventLoop(STRUCT_NAME *client);\
	int STRUCT_NAME##TcpForceStopEventLoop(STRUCT_NAME *client);\
\
	void STRUCT_NAME##TcpSetTimeout(STRUCT_NAME *client, time_t sec, suseconds_t usec);\
	void STRUCT_NAME##TcpClearTimeout(STRUCT_NAME *client);

#define CUTILS_DEF_TCP_CLIENT_C(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client){\
		client->sockfd = -1;\
		int err = cutilsStringInit(&client->serverAddr, INET6_ADDRSTRLEN);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		client->eb = event_base_new();\
		if(client->eb == NULL){\
			cutilsStringDeinit(&client->serverAddr);\
			return CUTILS_NOMEM;\
		}\
\
		client->ev = NULL;\
		client->useTimeout = false;\
\
		client->connected = false;\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client){\
		if(client->connected){\
			STRUCT_NAME##TcpDisconnect(client);\
		}\
\
		cutilsStringDeinit(&client->serverAddr);\
\
		event_base_free(client->eb);\
		client->eb = NULL;\
	}\
\
	int STRUCT_NAME##TcpConnect(STRUCT_NAME *client, const char *address,\
	const char *port, event_callback_fn callback){\
		if(client->connected){\
			STRUCT_NAME##TcpDisconnect(client);\
		}\
\
		struct addrinfo *res, hints;\
		memset(&hints, 0, sizeof(struct addrinfo));\
		hints.ai_family = AF_UNSPEC;\
		hints.ai_socktype = SOCK_STREAM;\
\
		if(getaddrinfo(address, port, &hints, &res) != 0){\
			return CUTILS_GETADDRINFO;\
		}\
\
		struct addrinfo *p = res;\
		while(p != NULL){\
			int err = (client->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol));\
			if(err == -1){\
				continue;\
			}\
			err = connect(client->sockfd, p->ai_addr, p->ai_addrlen);\
			if(err == -1){\
				close(client->sockfd);\
				client->sockfd = -1;\
				continue;\
			}\
\
			if(callback != NULL){\
				client->ev = event_new(client->eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);\
				if(client->ev == NULL){\
					close(client->sockfd);\
					client->sockfd = -1;\
					freeaddrinfo(res);\
					client->connected = false;\
					return CUTILS_CREATE_EVENT;\
				}\
\
				struct timeval *timeout = client->useTimeout?&client->timeout:NULL;\
				if(event_add(client->ev, timeout) == -1){\
					event_free(client->ev);\
					close(client->sockfd);\
					client->sockfd = -1;\
					freeaddrinfo(res);\
					client->connected = false;\
					return CUTILS_CREATE_EVENT;\
				}\
			}\
\
			char server[INET6_ADDRSTRLEN];\
			inet_ntop(p->ai_family, p->ai_addr, server, INET6_ADDRSTRLEN);\
			cutilsStringSet(&client->serverAddr, server);\
\
			client->connected = true;\
			break;\
\
			p = p->ai_next;\
		}\
\
		freeaddrinfo(res);\
\
		if(!client->connected){\
			return CUTILS_SOCKET;\
		}\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpDisconnect(STRUCT_NAME *client){\
		if(!client->connected){\
			return;\
		}\
\
		close(client->sockfd);\
		client->sockfd = -1;\
		client->connected = false;\
\
		if(client->ev != NULL){\
			event_del(client->ev);\
			event_free(client->ev);\
			client->ev = NULL;\
		}\
\
		cutilsStringSet(&client->serverAddr, '\0');\
	}\
\
	int STRUCT_NAME##TcpStartEventLoop(STRUCT_NAME *client){\
		return event_base_loop(client->eb, EVLOOP_NO_EXIT_ON_EMPTY);\
	}\
\
	int STRUCT_NAME##TcpStopEventLoop(STRUCT_NAME *client){\
		return event_base_loopexit(client->eb, NULL);\
	}\
	int STRUCT_NAME##TcpForceStopEventLoop(STRUCT_NAME *client){\
		return event_base_loopbreak(client->eb);\
	}\
\
	void STRUCT_NAME##TcpSetTimeout(STRUCT_NAME *client, time_t sec, suseconds_t usec){\
		client->timeout.tv_sec = sec;\
		client->timeout.tv_usec = usec;\
		if(client->connected){\
			event_add(client->ev, &client->timeout);\
		}\
		client->useTimeout = true;\
	}\
\
	void STRUCT_NAME##TcpClearTimeout(STRUCT_NAME *client){\
		if(client->connected){\
			event_add(client->ev, NULL);\
		}\
		client->useTimeout = false;\
	}
#else
#define CUTILS_DEF_TCP_CLIENT_STRUCT\
	int sockfd;\
\
	cutilsString serverAddr;\
	bool connected;

#define CUTILS_DEF_TCP_CLIENT_H(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client);\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client);\
\
	int STRUCT_NAME##TcpConnect(STRUCT_NAME *client, const char *address,\
	const char *port);\
	void STRUCT_NAME##TcpDisconnect(STRUCT_NAME *client);

#define CUTILS_DEF_TCP_CLIENT_C(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client){\
		client->sockfd = -1;\
		int err = cutilsStringInit(&client->server, INET6_ADDRSTRLEN);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		client->connected = false;\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client){\
		if(client->connected){\
			STRUCT_NAME##TcpDisconnect(client);\
		}\
\
		cutilsStringDeinit(client->serverAddr);\
	}\
\
	int STRUCT_NAME##TcpConnect(STRUCT_NAME *client, const char *address,\
	const char *port){\
		if(client->connected){\
			STRUCT_NAME##TcpDisconnect(client);\
		}\
\
		struct addrinfo *res, hints;\
		memset(&hints, 0, sizeof(struct addrinfo));\
		hints.ai_family = AF_UNSPEC;\
		hints.ai_socktype = SOCK_STREAM;\
\
		if(getaddrinfo(address, port, &hints, &res) != 0){\
			return CUTILS_GETADDRINFO;\
		}\
\
		struct addrinfo *p = res;\
		while(p != NULL){\
			int err = (client->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol));\
			if(err == -1){\
				continue;\
			}\
			err = connect(client->sockfd, p->ai_addr, p->ai_addrlen);\
			if(err == -1){\
				close(client->sockfd);\
				client->sockfd = -1;\
				continue;\
			}\
\
			char server[INET6_ADDRSTRLEN];\
			inet_ntop(p->ai_family, p->ai_addr, server, INET6_ADDRSTRLEN);\
			cutilsStringSet(&client->serverAddr, server);\
\
			client->connected = true;\
			break;\
\
			p = p->ai_next;\
		}\
\
		freeaddrinfo(res);\
\
		if(!client->connected){\
			return CUTILS_SOCKET;\
		}\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpDisconnect(STRUCT_NAME *client){\
		if(!client->connected){\
			return;\
		}\
\
		close(client->sockfd);\
		client->sockfd = -1;\
		client->connected = false;\
\
		cutilsStringSet(&client->serverAddr, '\0');\
	}\

#endif

#endif
