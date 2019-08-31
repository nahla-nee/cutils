#include "tcpServerClient.h"

int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, size_t bufferSize){
	client->sockfd = sockfd;

	int err = cutilsStringInit(&client->address, INET6_ADDRSTRLEN);
	if(err != CUTILS_OK){
		return err;
	}

	err = cutilsByteStreamInit(&client->buffer, bufferSize);
	if(err != CUTILS_OK){
		cutilsStringDeinit(&client->address);
		return err;
	}

	struct sockaddr sa;
	socklen_t len;
	getsockname(sockfd, &sa, &len);
	int family = sa.sa_family;

	char server[INET6_ADDRSTRLEN];
	
	inet_ntop(sa.sa_family, &client, server, INET6_ADDRSTRLEN);
	cutilsStringSet(&client->address, server);
}

void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client){
	close(client->sockfd);
	cutilsStringDeinit(&client->address);
	cutilsByteStreamDeinit(&client->buffer);
	client->sockfd = -1;
}

CUTILS_DEF_DYNARRAY_C(cutilsTcpServerClient, cutilsTcpServerClientArr, cutilsTcpServerClientArrDeinitCallback);
void cutilsTcpServerClientArrDeinitCallback(struct cutilsTcpServerClientArr *arr, void *userData){
	for(size_t i = 0; i < arr->size; i++){
		cutilsTcpServerClientDeinit(&arr->data[i]);
	}
}
