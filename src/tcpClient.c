#include "tcpClient.h"

int cutilsTcpClientInit(cutilsTcpClient *client){
	client->sockfd = -1;
	int err = cutilsStringInit(&client->serverAddr, INET6_ADDRSTRLEN);
	if(err != CUTILS_OK){
		return err;
	}

	client->eb = event_base_new();
	if(client->eb == NULL){
		cutilsStringDeinit(&client->serverAddr);
		return CUTILS_NOMEM;
	}

	client->ev = NULL;
	client->useTimeout = false;

	client->connected = false;

	return CUTILS_OK;
}

void cutilsTcpClientDeinit(cutilsTcpClient *client){
	if(client->connected){
		cutilsTcpClientDisconnect(client);
	}

	cutilsStringDeinit(&client->serverAddr);

	event_base_free(client->eb);
	client->eb = NULL;
}

int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address,
const char *port, event_callback_fn callback){
	if(client->connected){
		cutilsTcpClientDisconnect(client);
	}

	struct addrinfo *res, hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(address, port, &hints, &res) != 0){
		return CUTILS_GETADDRINFO;
	}

	struct addrinfo *p = res;
	while(p != NULL){
		int err = (client->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol));
		if(err == -1){
			continue;
		}
		err = connect(client->sockfd, p->ai_addr, p->ai_addrlen);
		if(err == -1){
			close(client->sockfd);
			client->sockfd = -1;
			continue;
		}

		if(callback != NULL){
			client->ev = event_new(client->eb, client->sockfd, EV_READ | EV_PERSIST, callback, client);
			if(client->ev == NULL){
				close(client->sockfd);
				client->sockfd = -1;
				freeaddrinfo(res);
				client->connected = false;
				return CUTILS_CREATE_EVENT;
			}

			struct timeval *timeout = client->useTimeout?&client->timeout:NULL;
			if(event_add(client->ev, timeout) == -1){
				event_free(client->ev);
				close(client->sockfd);
				client->sockfd = -1;
				freeaddrinfo(res);
				client->connected = false;
				return CUTILS_CREATE_EVENT;
			}
		}

		char server[INET6_ADDRSTRLEN];
		inet_ntop(p->ai_family, p->ai_addr, server, INET6_ADDRSTRLEN);
		cutilsStringSet(&client->serverAddr, server);

		client->connected = true;
		break;

		p = p->ai_next;
	}

	freeaddrinfo(res);

	if(!client->connected){
		return CUTILS_SOCKET;
	}

	return CUTILS_OK;
}

void cutilsTcpClientDisconnect(cutilsTcpClient *client){
	if(!client->connected){
		return;
	}

	close(client->sockfd);
	client->sockfd = -1;
	client->connected = false;

	if(client->ev != NULL){
		event_del(client->ev);
		event_free(client->ev);
		client->ev = NULL;
	}

	cutilsStringSet(&client->serverAddr, '0');
}

int cutilsTcpClientStartEventLoop(cutilsTcpClient *client){
	return event_base_loop(client->eb, EVLOOP_NO_EXIT_ON_EMPTY);
}

int cutilsTcpClientStopEventLoop(cutilsTcpClient *client){
	return event_base_loopexit(client->eb, NULL);
}
int cutilsTcpClientForceStopEventLoop(cutilsTcpClient *client){
	return event_base_loopbreak(client->eb);
}

void cutilsTcpClientSetTimeout(cutilsTcpClient *client, time_t sec, suseconds_t usec){
	client->timeout.tv_sec = sec;
	client->timeout.tv_usec = usec;
	if(client->connected){
		event_add(client->ev, &client->timeout);
	}
	client->useTimeout = true;
}

void cutilsTcpClientClearTimeout(cutilsTcpClient *client){
	if(client->connected){
		event_add(client->ev, NULL);
	}
	client->useTimeout = false;
}
