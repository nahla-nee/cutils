#include "tcpServerClient.h"

typedef struct cutilsTcpServer{
	int sockfd;
	cutilsTcpServerClientArr clients;

	#ifndef CUTILS_NO_LIBEVENT
	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	struct timeval timeoutClient;
	bool useTimeoutClient;
	#endif

	size_t clientBufferSize;
	cutilsString port;
	int backlog;
	bool started;
} cutilsTcpServer;

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen, event_callback_fn callback){
#else
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen){
#endif
	client->sockfd = sockfd;

	int err = cutilsStringInit(&client->address, INET6_ADDRSTRLEN);
	if(err != CUTILS_OK){
		return err;
	}

	err = cutilsByteStreamInit(&client->buffer, server->clientBufferSize);
	if(err != CUTILS_OK){
		cutilsStringDeinit(&client->address);
		return err;
	}

	char address[INET6_ADDRSTRLEN];
	inet_ntop(addr.sa_family, &client, address, INET6_ADDRSTRLEN);
	cutilsStringSet(&client->address, address);
	client->server = server;

	#ifndef CUTILS_NO_LIBEVENT
	if(callback == NULL){
		client->ev = NULL;
		return CUTILS_OK;
	}
	client->ev = event_new(server->eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);
	if(client->ev == NULL){
		cutilsStringDeinit(&client->address);
		cutilsByteStreamDeinit(&client->buffer);
		client->server = NULL;
		return CUTILS_CREATE_EVENT;
	}
	if(event_add(client->ev, &server->timeoutClient) == -1){
		cutilsStringDeinit(&client->address);
		cutilsByteStreamDeinit(&client->buffer);
		event_free(client->ev);
		client->ev = NULL;
		client->server = NULL;
		return CUTILS_CREATE_EVENT;
	}
	#endif

	return CUTILS_OK;
}

void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client){
	#ifdef CUTILS_NO_LIBEVENT
	if(client->ev != NULL){
		event_del(client->ev);
		event_free(client->ev);
		client->ev = NULL;
	}
	#endif
	close(client->sockfd);
	client->sockfd = -1;
	cutilsStringDeinit(&client->address);
	cutilsByteStreamDeinit(&client->buffer);
	client->server = NULL;
}

CUTILS_DEF_DYNARRAY_C(cutilsTcpServerClient, cutilsTcpServerClientArr, cutilsTcpServerClientArrDeinitCallback);
void cutilsTcpServerClientArrDeinitCallback(cutilsTcpServerClient *arr, size_t count, void *userData){
	for(size_t i = 0; i < count; i++){
		cutilsTcpServerClientDeinit(arr+i);
	}
}
