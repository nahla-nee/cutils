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

typedef struct cutilsTcpServerClient{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event *ev;
	#endif

	cutilsTcpServer *server;
	cutilsString clientAddress;
	cutilsByteStream inBuffer;
	cutilsByteStream outBuffer;
} cutilsTcpServerClient;

#ifndef CUTILS_NO_LIBEVENT
#define CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT\
	int sockfd;\
\
	struct event *ev;\
\
	cutilsString clientAddress;

#define CUTILS_DEF_TCP_SERVER_CLIENT_H(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client, int sockfd, struct sockaddr address,\
		struct event_base *eb, struct timeval *timeout, event_callback_fn callback);\
\
	STRUCT_NAME* STRUCT_NAME##TcpNew(int sockfd, struct sockaddr address, struct event_base *eb,\
		struct timeval *timeout, event_callback_fn callback);\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client);\
	void STRUCT_NAME##TcpDelete(STRUCT_NAME *client);

#define CUTILS_DEF_TCP_SERVER_CLIENT_C(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client, int sockfd,\ struct sockaddr address,\
	struct event_base *eb, struct timeval *timeout event_callback_fn callback){\
		client->sockfd = sockfd;\
\
		int err = cutilsStringInit(&client->address, INET6_ADDRSTRLEN);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		char clientAddr[INET6_ADDRSTRLEN];\
		inet_ntop(address.sa_family, &address, clientAddr, INET6_ADDRSTRLEN);\
		cutilsStringSet(&client->address, address);\
\
		client->ev = NULL;\
\
		if(callback != NULL){\
			client->ev = event_new(eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);\
			if(client->ev == NULL){\
				cutilsStringDeinit(&client->address);\
				return CUTILS_CREATE_EVENT;\
			}\
\
			if(event_add(client->ev, timeout) == -1){\
				cutilsStringDeinit(&client->address);\
				event_free(client->ev);\
				client->ev = NULL;\
				return CUTILS_CREATE_EVENT;\
			}\
		}\
\
		return CUTILS_OK;\
	}\
\
	STRUCT_NAME* STRUCT_NAME##TcpNew(int sockfd, struct sockaddr address, struct event_base *eb,\
	struct timeval *timeout, event_callback_fn callback){\
		STRUCT_NAME *ret = malloc(sizeof(STRUCT_NAME));\
		if(ret == NULL){\
			return NULL;\
		}\
		if(STRUCT_NAME##TcpInit(ret, address, eb, timeout, callback) != CUTILS_OK){\
			free(ret);\
			return NULL;\
		}\
\
		return ret;\
	}\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client){\
		if(client->ev != NULL){\
			event_del(client->ev);\
			event_free(client->ev);\
			client->ev = NULL;\
		}\
\
		close(client->sockfd);\
		client->sockfd = -1;\
		cutilsStringDeinit(&client->address);\
	}\
\
	void STRUCT_NAME##TcpDelete(STRUCT_NAME *client){\
		STRUCT_NAME##Deinit(client);\
		free(client);\
	}
#else
#define CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT\
	int sockfd;\
\
	cutilsString address;\

#define CUTILS_DEF_TCP_SERVER_CLIENT_H(STRUCT_NAME)\
int STRUCT_NAME##TcpInit(STRUCT_NAME *client, int sockfd, struct sockaddr address);\
STRUCT_NAME* TcpNew(int sockfd, struct sockaddr address);\
void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client);\
void STRUCT_NAME##TcpDelete(STRUCT_NAME *client);

#define CUTILS_DEF_TCP_SERVER_CLIENT_C(STRUCT_NAME)\
	int STRUCT_NAME##TcpInit(STRUCT_NAME *client, int sockfd,\ struct sockaddr address){\
		client->sockfd = sockfd;\
\
		int err = cutilsStringInit(&client->address, INET6_ADDRSTRLEN);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		char clientAddr[INET6_ADDRSTRLEN];\
		inet_ntop(address.sa_family, &address, clientAddr, INET6_ADDRSTRLEN);\
		cutilsStringSet(&client->address, address);\
\
		return CUTILS_OK;\
	}\
\
	STRUCT_NAME* STRUCT_NAME##TcpNew(int sockfd, struct sockaddr address){\
		STRUCT_NAME *ret = malloc(sizeof(STRUCT_NAME));\
		if(ret == NULL){\
			return NULL;\
		}\
		if(STRUCT_NAME##TcpInit(ret, address) != CUTILS_OK){\
			free(ret);\
			return NULL;\
		}\
\
		return ret;\
	}\
\
	void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client){\
		close(client->sockfd);\
		client->sockfd = -1;\
		cutilsStringDeinit(&client->address);\
	}\
\
	void STRUCT_NAME##TcpDelete(STRUCT_NAME *client){\
		STRUCT_NAME##Deinit(client);\
		free(client);\
	}
#endif

#endif
