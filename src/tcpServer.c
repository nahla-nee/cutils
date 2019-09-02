#include "tcpServer.h"

int cutilsTcpServerInit(cutilsTcpServer *server, size_t clientBufferSize){
	server->sockfd = -1;
	server->clientBufferSize = clientBufferSize;
	int err = cutilsStringInit(&server->port, 0);
	if(err != CUTILS_OK){
		return err;
	}
	server->backlog = 0;

	#ifndef CUTILS_NO_LIBEVENT
	server->ev = NULL;
	server->eb = event_base_new();
	if(server->eb == NULL){
		cutilsTcpServerClientArrDeinit(&server->clients);
		cutilsStringDeinit(&server->port);
		return CUTILS_NOMEM;
	}
	server->useTimeout = server->useTimeoutClient = false;
	#endif

	server->started = false;

	return CUTILS_OK;
}

cutilsTcpServer* cutilsTcpServerNew(size_t clientBufferSize){
	cutilsTcpServer *ret = malloc(sizeof(cutilsTcpServer));
	if(ret == NULL){
		return NULL;
	}
	if(cutilsTcpServerInit(ret, clientBufferSize) != CUTILS_OK){
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
	int err = cutilsTcpServerClientArrInit(&server->clients, 0);
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
				return CUTILS_NOMEM;
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
	event_del(server->ev);
	event_free(server->ev);
	server->ev = NULL;
	server->useTimeout = server->useTimeoutClient = false;
	#endif

	cutilsTcpServerClientArrDeinit(&server->clients);

	close(server->sockfd);
	server->sockfd = -1;

	cutilsStringSet(&server->port, "");

	server->started = false;
}

int cutilsTcpServerStartEventLoop(cutilsTcpServer *server){
	return event_base_loop(server->eb, EVLOOP_NO_EXIT_ON_EMPTY);
}

int cutilsTcpServerStopEventLoop(cutilsTcpServer *server){
	return event_base_loopexit(server->eb, NULL);
}

int cutilsTcpServerForceStopEventLoop(cutilsTcpServer *server){
	return event_base_loopbreak(server->eb);
}

#ifndef CUTILS_NO_LIBEVENT
int cutilsTcpServerAddClient(cutilsTcpServer *server, int sockfd, event_callback_fn callback){
#else
int cutilsTcpServerAddClient(cutilsTcpServer *server, int sockfd){
#endif
	cutilsTcpServerClient client;
	#ifndef CUTILS_NO_LIBEVENT
	int err = cutilsTcpServerClientInit(&client, sockfd, server->clientBufferSize, server, callback);
	#else
	int err = cutilsTcpServerClientInit(&client, sockfd, server->clientBufferSize, server);
	#endif
	if(err != CUTILS_OK){
		return err;
	}

	err = cutilsTcpServerClientArrPushBack(&server->clients, client);
	if(err != CUTILS_OK){
		cutilsTcpServerClientDeinit(&client);
		return err;
	}

	return CUTILS_OK;
}

void cutilsTcpServerRemoveClient(cutilsTcpServer *server, size_t index){
	cutilsTcpServerClientArrDelete(&server->clients, index);
}
