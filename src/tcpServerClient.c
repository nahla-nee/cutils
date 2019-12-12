#include "tcpServerClient.h"

int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct sockaddr address,
	struct event_base *eb, struct timeval *timeout, event_callback_fn callback){

	client->sockfd = sockfd;

	int err = cutilsStringInit(&client->clientAddress, INET6_ADDRSTRLEN);
	if(err != CUTILS_OK){
		return err;
	}

	char clientAddr[INET6_ADDRSTRLEN];
	inet_ntop(address.sa_family, &address, clientAddr, INET6_ADDRSTRLEN);
	cutilsStringSet(&client->clientAddress, clientAddr);

	client->ev = NULL;

	if(eb != NULL){
		client->ev = event_new(eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);
		if(client->ev == NULL){
			cutilsStringDeinit(&client->clientAddress);
			return CUTILS_CREATE_EVENT;
		}

		if(event_add(client->ev, timeout) == -1){
			cutilsStringDeinit(&client->clientAddress);
			event_free(client->ev);
			client->ev = NULL;
			return CUTILS_CREATE_EVENT;
		}
	}

	return CUTILS_OK;
}

void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client){
	if(client->ev != NULL){
		event_del(client->ev);
		event_free(client->ev);
		client->ev = NULL;
	}

	close(client->sockfd);
	client->sockfd = -1;
	cutilsStringDeinit(&client->clientAddress);
}
