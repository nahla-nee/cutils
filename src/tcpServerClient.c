#include "tcpServerClient.h"

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	char *address, void *usrptr, event_callback_fn callback){
#else
int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	char *address, void *usrptr){
#endif
	client->sockfd = sockfd;

	int err = cutilsStringInit(&client->address, INET6_ADDRSTRLEN);
	if(err != CUTILS_OK){
		return err;
	}

	err = cutilsByteStreamInit(&client->inBuffer, server->clientInBufferSize);
	if(err != CUTILS_OK){
		cutilsStringDeinit(&client->address);
		return err;
	}

	err = cutilsByteStreamInit(&client->outBuffer, server->clientOutBufferSize);
	if(err != CUTILS_OK){
		cutilsStringDeinit(&client->address);
		cutilsByteStreamDeinit(&client->inBuffer);
	}

	cutilsStringSet(&client->address, address);

	#ifndef CUTILS_NO_LIBEVENT
	if(callback == NULL){
		client->ev = NULL;
		return CUTILS_OK;
	}

	void *ptr = usrptr == NULL?client:usrptr;
	client->ev = event_new(server->eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);
	if(client->ev == NULL){
		cutilsStringDeinit(&client->address);
		cutilsByteStreamDeinit(&client->inBuffer);
		cutilsByteStreamDeinit(&client->outBuffer);
		client->server = NULL;
		return CUTILS_CREATE_EVENT;
	}
	struct timeval *timeout = server->useTimeoutClient?&server->timeoutClient:NULL;
	if(event_add(client->ev, timeout) == -1){
		cutilsStringDeinit(&client->address);
		cutilsByteStreamDeinit(&client->inBuffer);
		cutilsByteStreamDeinit(&client->outBuffer);
		event_free(client->ev);
		client->ev = NULL;
		client->server = NULL;
		return CUTILS_CREATE_EVENT;
	}
	#endif

	return CUTILS_OK;
}

#ifndef CUTILS_NO_LIBEVENT
cutilsTcpServerClient* cutilsTcpServerClientNew(int sockfd, cutilsTcpServer *server,
	char *address, void *usrptr, event_callback_fn callback){
#else
cutilsTcpServerClient* cutilsTcpServerClientNew(int sockfd, cutilsTcpServer *server,
	char *address, void *usrptr){
#endif
	cutilsTcpServerClient *ret = malloc(sizeof(cutilsTcpServerClient));
	if(ret == NULL){
		return NULL;
	}
	#ifndef CUTILS_NO_LIBEVENT
	int err = cutilsTcpServerClientInit(ret, sockfd, server, address, usrptr, callback);
	#else
	int err = cutilsTcpServerClientInit(ret, sockfd, server, address, usrptr);
	#endif
	if(err != CUTILS_OK){
		free(ret);
		return NULL;
	}

	return ret;
}

void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client){
	#ifndef CUTILS_NO_LIBEVENT
	if(client->ev != NULL){
		event_del(client->ev);
		event_free(client->ev);
		client->ev = NULL;
	}
	#endif
	close(client->sockfd);
	client->sockfd = -1;
	cutilsStringDeinit(&client->address);
	cutilsByteStreamDeinit(&client->inBuffer);
	cutilsByteStreamDeinit(&client->outBuffer);
	client->server = NULL;
}

void cutilsTcpServerClientFree(cutilsTcpServerClient *client){
	cutilsTcpServerClientDeinit(client);
	free(client);
}
