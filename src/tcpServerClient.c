#include "tcpServerClient.h"

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen, event_callback_fn callback){
#else
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct cutilsTcpServer *server,
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

	char server[INET6_ADDRSTRLEN];
	inet_ntop(addr.sa_family, &client, server, INET6_ADDRSTRLEN);
	cutilsStringSet(&client->address, server);
	client->server = server;

	#ifndef CUTILS_NO_LIBEVENT
	if(callback == NULL){
		client->ev = NULL;
		return CUTILS_OK;
	}
	client->ev = event_new(server->eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);
	if(client->ev == NULL){
		cutilsStringDeinit(&client->server);
		cutilsByteStreamDeinit(&client->buffer);
		client->server = NULL;
		return CUTILS_CREATE_EVENT;
	}
	if(event_add(ev, &server->timeoutClient) == -1){
		cutilsStringDeinit(&client->server);
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
