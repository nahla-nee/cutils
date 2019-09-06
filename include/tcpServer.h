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

#ifndef CUTILS_NO_LIBEVENT
#define CUTILS_DEF_TCP_SERVER_STRUCT\
	int sockfd;\
\
	struct event_base *eb;\
	struct event *ev;\
	struct timeval timeout;\
	bool useTimeout;\
\
	struct timeval clientTimeout;\
	bool useClientTimeout;\
\
	cutilsString port;\
	int backlog;\
	bool started;

#define CUTILS_DEF_TCP_SERVER_H(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *server);\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *server);\
\
	int STRUCT_NAME##TcpStart(STRUCT_NAME *server, const char *port, int backlog,\
	void *usrptr, event_callback_fn callback);\
	void STRUCT_NAME##TcpStop(STRUCT_NAME *server);\
\
	int STRUCT_NAME##TcpStartEventLoop(STRUCT_NAME *server);\
	int STRUCT_NAME##TcpStopEventLoop(STRUCT_NAME *server);\
	int STRUCT_NAME##TcpForceStopEventLoop(STRUCT_NAME *server);\
\
	void STRUCT_NAME##TcpSetTimeout(STRUCT_NAME *server, time_t sec, suseconds_t usec);\
	void STRUCT_NAME##TcpClearTimeout(STRUCT_NAME *server);\
\
	void STRUCT_NAME##TcpSetClientTimeout(STRUCT_NAME *server, time_t sec, suseconds_t usec);\
	void STRUCT_NAME##TcpClearClientTimeout(STRUCT_NAME *server);

#define CUTILS_DEF_TCP_SERVER_C(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *server){\
		server->sockfd = -1;\
		int err = cutilsStringInit(&server->port, 0);\
		if(err != CUTILS_OK){\
			return err;\
		}\
		server->backlog = 0;\
\
		server->ev = NULL;\
		server->eb = event_base_new();\
		if(server->eb == NULL){\
			cutilsStringDeinit(&server->port);\
			return CUTILS_NOMEM;\
		}\
		server->useTimeout = server->useClientTimeout = false;\
\
		server->started = false;\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *server){\
		if(server->started){\
			STRUCT_NAME##TcpStop(server);\
		}\
\
		event_base_free(server->eb);\
		server->eb = NULL;\
\
		cutilsStringDeinit(&server->port);\
	}\
\
	int STRUCT_NAME##TcpStart(STRUCT_NAME *server, const char *port, int backlog,\
	void *usrptr, event_callback_fn callback){\
		if(server->started){\
			STRUCT_NAME##TcpStop(server);\
		}\
\
		int err = cutilsStringSet(&server->port, port);\
		if(err != CUTILS_OK){\
			return err;\
		}\
		server->backlog = backlog;\
\
		int yes = 1;\
\
		struct addrinfo *res, hints;\
		memset(&hints, 0, sizeof(struct addrinfo));\
		hints.ai_family = AF_UNSPEC;\
		hints.ai_socktype = SOCK_STREAM;\
		hints.ai_flags = AI_PASSIVE;\
\
		if(getaddrinfo(NULL, port, &hints, &res) != 0){\
			return CUTILS_GETADDRINFO;\
		}\
\
		struct addrinfo *p = res;\
		while(p != NULL){\
			int err = (server->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol));\
			if(err == -1){\
				continue;\
			}\
			err = setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));\
			if(err == -1){\
				close(server->sockfd);\
				server->sockfd = -1;\
				continue;\
			}\
			err = bind(server->sockfd, p->ai_addr, p->ai_addrlen);\
			if(err == -1){\
				close(server->sockfd);\
				server->sockfd = -1;\
				continue;\
			}\
			err = listen(server->sockfd, backlog);\
			if(err == -1){\
				close(server->sockfd);\
				server->sockfd = -1;\
				continue;\
			}\
			if(callback != NULL){\
				void *ptr = usrptr == NULL?server:usrptr;\
				server->ev = event_new(server->eb, server->sockfd, EV_READ | EV_PERSIST, callback, ptr);\
				if(server->ev == NULL){\
					close(server->sockfd);\
					server->sockfd = -1;\
					freeaddrinfo(res);\
					server->started = false;\
					return CUTILS_CREATE_EVENT;\
				}\
\
				struct timeval *timeout = server->useTimeout?&server->timeout:NULL;\
				if(event_add(server->ev, timeout) == -1){\
					event_free(server->ev);\
					close(server->sockfd);\
					server->sockfd = -1;\
					server->started = false;\
					return CUTILS_CREATE_EVENT;\
				}\
			}\
			server->started = true;\
			break;\
\
			p = p->ai_next;\
		}\
\
		freeaddrinfo(res);\
\
		if(!server->started){\
			return CUTILS_SOCKET;\
		}\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpStop(STRUCT_NAME *server){\
		if(!server->started){\
			return;\
		}\
\
		if(server->ev != NULL){\
			event_del(server->ev);\
			event_free(server->ev);\
			server->ev = NULL;\
		}\
		server->useTimeout = server->useClientTimeout = false;\
\
		close(server->sockfd);\
		server->sockfd = -1;\
\
		cutilsStringSet(&server->port, "");\
\
		server->started = false;\
	}\
\
	int STRUCT_NAME##TcpStartEventLoop(STRUCT_NAME *server){\
		return event_base_loop(server->eb, EVLOOP_NO_EXIT_ON_EMPTY);\
	}\
\
	int STRUCT_NAME##TcpStopEventLoop(STRUCT_NAME *server){\
		return event_base_loopexit(server->eb, NULL);\
	}\
\
	int STRUCT_NAME##TcpForceStopEventLoop(STRUCT_NAME *server){\
		return event_base_loopbreak(server->eb);\
	}\
\
	void STRUCT_NAME##TcpSetTimeout(STRUCT_NAME *server, time_t sec, suseconds_t usec){\
		server->timeout.tv_sec = sec;\
		server->timeout.tv_usec = usec;\
		if(server->started){\
			event_add(server->ev, &server->timeout);\
		}\
\
		server->useTimeout = true;\
	}\
\
	void STRUCT_NAME##TcpClearTimeout(STRUCT_NAME *server){\
		if(server->started){\
			event_add(server->ev, NULL);\
		}\
\
		server->useTimeout = false;\
	}\
\
	void STRUCT_NAME##TcpSetClientTimeout(STRUCT_NAME *server, time_t sec, suseconds_t usec){\
		server->clientTimeout.tv_sec = sec;\
		server->clientTimeout.tv_usec = usec;\
\
		server->useClientTimeout = true;\
	}\
\
	void STRUCT_NAME##TcpClearClientTimeout(STRUCT_NAME *server){\
		server->useClientTimeout = false;\
	}
#else
#define CUTILS_DEF_TCP_SERVER_STRUCT\
	int sockfd;\
\
	cutilsString port;\
	int backlog;\
	bool started;

#define CUTILS_DEF_TCP_SERVER_H(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *server);\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *server);\
\
	int STRUCT_NAME##TcpStart(STRUCT_NAME *server, const char *port, int backlog);\
	void STRUCT_NAME##TcpStop(STRUCT_NAME *server);

#define CUTILS_DEF_TCP_SERVER_C(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *server){\
		server->sockfd = -1;\
		int err = cutilsStringInit(&server->port, 0);\
		if(err != CUTILS_OK){\
			return err;\
		}\
		server->backlog = 0;\
\
		server->started = false;\
\
		return CUTILS_OK;\
	}\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *server){\
		if(server->started){\
			cutilsTcpServerStop(server);\
		}\
\
		cutilsStringDeinit(&server->port);\
	}\
\
	int STRUCT_NAME##TcpStart(STRUCT_NAME *server, const char *port, int backlog,\
	event_callback_fn callback){\
		if(server->started){\
			cutilsTcpServerClose(server);\
		}\
\
		int err = cutilsStringSet(&server->port, port);\
		if(err != CUTILS_OK){\
			return err;\
		}\
		server->backlog = backlog;\
\
		int yes = 1;\
\
		struct addrinfo *res, hints;\
		memset(&hints, 0, sizeof(struct addrinfo));\
		hints.ai_family = AF_UNSPEC;\
		hints.ai_socktype = SOCK_STREAM;\
		hints.ai_flags = AI_PASSIVE;\
\
		if(getaddrinfo(NULL, port, &hints, &res) != 0){\
			return CUTILS_GETADDRINFO;\
		}\
\
		struct addrinfo *p = res;\
		while(p != NULL){\
			int err = (server->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol));\
			if(err == -1){\
				continue;\
			}\
			err = setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));\
			if(err == -1){\
				close(server->sockfd);\
				server->sockfd = -1;\
				continue;\
			}\
			err = bind(server->sockfd, p->ai_addr, p->ai_addrlen);\
			if(err == -1){\
				close(server->sockfd);\
				server->sockfd = -1;\
				continue;\
			}\
			err = listen(server->sockfd, backlog);\
			if(err == -1){\
				close(server->sockfd);\
				server->sockfd = -1;\
				continue;\
			}\
			server->started = true;\
			break;\
\
			p = p->ai_next;\
		}\
\
		freeaddrinfo(res);\
\
		if(!server->started){\
			return CUTILS_SOCKET;\
		}\

		return CUTILS_OK;
	}\
\
	void STRUCT_NAME##TcpStop(STRUCT_NAME *server){\
		if(!server->started){\
			return;\
		}\
\
		close(server->sockfd);\
		server->sockfd = -1;\
\
		cutilsStringSet(&server->port, "");\
\
		server->started = false;\
	}\

#endif

#endif
