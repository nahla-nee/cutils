#include "tcpServer.h"

#include <stdio.h>

int cutilsTcpServerInit(cutilsTcpServer *server, size_t inBufferSize, size_t outBufferSize){
	server->sockfd = -1;
	server->clientInBufferSize = inBufferSize;
	server->clientOutBufferSize = outBufferSize;
	int err = cutilsStringInit(&server->port, 0);
	if(err != CUTILS_OK){
		return err;
	}
	server->backlog = 0;

	#ifndef CUTILS_NO_LIBEVENT
	server->ev = NULL;
	server->eb = event_base_new();
	if(server->eb == NULL){
		cutilsTcpServerClientLLDeinit(&server->clients);
		cutilsStringDeinit(&server->port);
		return CUTILS_NOMEM;
	}
	server->useTimeout = server->useTimeoutClient = false;
	#endif

	server->started = false;

	return CUTILS_OK;
}

cutilsTcpServer* cutilsTcpServerNew(size_t inBufferSize, size_t outBufferSize){
	cutilsTcpServer *ret = malloc(sizeof(cutilsTcpServer));
	if(ret == NULL){
		return NULL;
	}
	if(cutilsTcpServerInit(ret, inBufferSize, outBufferSize) != CUTILS_OK){
		free(ret);
		return NULL;
	}
	return ret;
}

void cutilsTcpServerDeinit(cutilsTcpServer *server){
	if(server->started){
		cutilsTcpServerClose(server);
	}

	#ifndef CUTILS_NO_LIBEVENT
	event_base_free(server->eb);
	server->eb = NULL;
	#endif

	cutilsStringDeinit(&server->port);
}

void cutilsTcpServerFree(cutilsTcpServer *server){
	cutilsTcpServerDeinit(server);
	free(server);
}

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog, event_callback_fn callback){
#else
int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog){
#endif
	if(server->started){
		cutilsTcpServerClose(server);
	}

	int err = cutilsTcpServerClientLLInit(&server->clients, 0);
	if(err != CUTILS_OK){
		return err;
	}

	err = cutilsStringSet(&server->port, port);
	if(err != CUTILS_OK){
		return err;
	}
	server->backlog = backlog;

	int yes = 1;

	struct addrinfo *res, hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if(getaddrinfo(NULL, port, &hints, &res) != 0){
		return CUTILS_GETADDRINFO;
	}

	struct addrinfo *p = res;
	while(p != NULL){
		int err = (server->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol));
		if(err == -1){
			continue;
		}
		err = setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if(err == -1){
			close(server->sockfd);
			server->sockfd = -1;
			continue;
		}
		err = bind(server->sockfd, p->ai_addr, p->ai_addrlen);
		if(err == -1){
			close(server->sockfd);
			server->sockfd = -1;
			continue;
		}
		err = listen(server->sockfd, backlog);
		if(err == -1){
			close(server->sockfd);
			server->sockfd = -1;
			continue;
		}
		#ifndef CUTILS_NO_LIBEVENT
		if(callback != NULL){
			server->ev = event_new(server->eb, server->sockfd, EV_READ | EV_PERSIST, callback, server);
			if(server->ev == NULL){
				close(server->sockfd);
				server->sockfd = -1;
				freeaddrinfo(res);
				server->started = false;
				return CUTILS_CREATE_EVENT;
			}

			struct timeval *timeout = server->useTimeout?&server->timeout:NULL;
			if(event_add(server->ev, timeout) == -1){
				event_free(server->ev);
				close(server->sockfd);
				server->sockfd = -1;
				server->started = false;
				return CUTILS_CREATE_EVENT;
			}
		}
		#endif
		server->started = true;
		break;

		p = p->ai_next;
	}

	freeaddrinfo(res);

	if(!server->started){
		return CUTILS_SOCKET;
	}

	return CUTILS_OK;
}

void cutilsTcpServerClose(cutilsTcpServer *server){
	if(!server->started){
		return;
	}

	#ifndef CUTILS_NO_LIBEVENT
	if(server->ev != NULL){
		event_del(server->ev);
		event_free(server->ev);
		server->ev = NULL;
	}
	server->useTimeout = server->useTimeoutClient = false;
	#endif

	cutilsTcpServerClientLLDeinit(&server->clients);

	close(server->sockfd);
	server->sockfd = -1;

	cutilsStringSet(&server->port, "");

	server->started = false;
}

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerStartEventLoop(cutilsTcpServer *server){
	return event_base_loop(server->eb, EVLOOP_NO_EXIT_ON_EMPTY);
}

int cutilsTcpServerStopEventLoop(cutilsTcpServer *server){
	return event_base_loopexit(server->eb, NULL);
}

int cutilsTcpServerForceStopEventLoop(cutilsTcpServer *server){
	return event_base_loopbreak(server->eb);
}

void cutilsTcpServerSetTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec){
	server->timeout.tv_sec = sec;
	server->timeout.tv_usec = usec;
	if(server->started){
		event_add(server->ev, &server->timeout);
	}
	server->useTimeout = true;
}

void cutilsTcpServerClearTimeout(cutilsTcpServer *server){
	if(server->started){
		event_add(server->ev, NULL);
	}
	server->useTimeout = false;
}

void cutilsTcpServerSetClientTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec){
	server->timeoutClient.tv_sec = sec;
	server->timeoutClient.tv_usec = usec;
	if(server->started){
		cutilsTcpServerClientLLNode *client = server->clients.head;
		while(client != NULL){
			event_add(client->data.ev, &server->timeoutClient);
			client = client->next;
		}
	}
	server->useTimeoutClient = true;
}

void cutilsTcpServerClearClientTimeout(cutilsTcpServer *server){
	if(server->started){
		cutilsTcpServerClientLLNode *client = server->clients.head;
		while(client != NULL){
			event_add(client->data.ev, NULL);
			client = client->next;
		}
	}
	server->useTimeoutClient = false;
}
#endif

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerAddClient(cutilsTcpServer *server, int sockfd, struct sockaddr addr,
	void *usrptr, event_callback_fn callback){
#else
int cutilsTcpServerAddClient(cutilsTcpServer *server, int sockfd, struct sockaddr addr,
	void *usrptr){
#endif
	cutilsTcpServerClientLLNode *newClient = malloc(sizeof(cutilsTcpServerClientLLNode));
	if(newClient == NULL){
		return CUTILS_NOMEM;
	}

	char address[INET6_ADDRSTRLEN];
	inet_ntop(addr.sa_family, &addr, address, INET6_ADDRSTRLEN);

	#ifndef CUTILS_NO_LIBEVENT
	int err = cutilsTcpServerClientInit(&newClient->data, sockfd, server, address, usrptr, callback);
	#else
	int err = cutilsTcpServerClientInit(&newClient->data, sockfd, server, address, usrptr);
	#endif
	if(err != CUTILS_OK){
		free(newClient);
		return err;
	}

	newClient->data.node = newClient;
	cutilsTcpServerClientLLConnectNodeEnd(&server->clients, newClient);

	return CUTILS_OK;
}

void cutilsTcpServerRemoveClient(cutilsTcpServer *server, cutilsTcpServerClient *client){
	cutilsTcpServerClientLLRemoveNode(client->node);
}
