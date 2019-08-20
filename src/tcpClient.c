#include "tcpClient.h"

int cutilsTcpClientInit(cutilsTcpClient *client, size_t bufferSize){
	client->sockfd = -1;
	int err = cutilsStringInit(&client->server, INET6_ADDRSTRLEN);
	if(err != CUTILS_OK){
		return err;
	}

	err = cutilsByteStreamInit(&client->buffer, bufferSize);
	if(err != CUTILS_OK){
		return err;
	}

	return CUTILS_OK;
}

cutilsTcpClient* cutilsTcpClientNew(size_t bufferSize){
	cutilsTcpClient *ret = malloc(sizeof(cutilsTcpClient));
	if(ret == NULL){
		return NULL;
	}
	if(cutilsTcpClientInit(ret, bufferSize) != CUTILS_OK){
		free(ret);
		return NULL;
	}
	return ret;
}

void cutilsTcpClientFree(cutilsTcpClient *client){
	cutilsTcpClientDisconnect(client);
	cutilsStringFree(&client->server);
	cutilsByteStreamFree(&client->buffer);
}

int cutilsTcpClientConnect(cutilsTcpClient *client, const char *node, const char *service){
	if(client->sockfd != -1){
		cutilsTcpClientDisconnect(client);
	}

	char server[INET6_ADDRSTRLEN];

	struct addrinfo *res, hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(node, service, &hints, &res) != 0){
		return CUTILS_GETADDRINFO;
	}

	struct addrinfo *p = res;
	while(p != NULL){
		if((client->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1){
			if(connect(client->sockfd, p->ai_addr, p->ai_addrlen) != -1){
				break;
			}
			else{
				close(client->sockfd);
				client->sockfd = -1;
			}
		}
		p = p->ai_next;
	}

	if(client->sockfd == -1){
		return CUTILS_SOCKET;
	}

	inet_ntop(p->ai_family, p->ai_addr, server, INET6_ADDRSTRLEN);
	cutilsStringSet(&client->server, server);

	freeaddrinfo(res);
	return CUTILS_OK;
}

void cutilsTcpClientDisconnect(cutilsTcpClient *client){
	close(client->sockfd);
	client->sockfd = -1;
}
