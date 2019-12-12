#include "tcpServer.h"

int cutilsTcpServerInit(cutilsTcpServer *server){
	server->sockfd = -1;
	int err = cutilsStringInit(&server->port, 0);
	if(err != CUTILS_OK){
		return err;
	}
	server->backlog = 0;

	server->ev = NULL;
	server->eb = event_base_new();
	if(server->eb == NULL){
		cutilsStringDeinit(&server->port);
		return CUTILS_NOMEM;
	}
	server->useTimeout = server->useClientTimeout = false;

	server->started = false;

	return CUTILS_OK;
}

void cutilsTcpServerDeinit(cutilsTcpServer *server){
	if(server->started){
		cutilsTcpServerStop(server);
	}

	event_base_free(server->eb);
	server->eb = NULL;

	cutilsStringDeinit(&server->port);
}

int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog,
void *usrptr, event_callback_fn callback){
	if(server->started){
		cutilsTcpServerStop(server);
	}

	int err = cutilsStringSet(&server->port, port);
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
		if(callback != NULL){
			void *ptr = usrptr == NULL?server:usrptr;
			server->ev = event_new(server->eb, server->sockfd, EV_READ | EV_PERSIST, callback, ptr);
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

void cutilsTcpServerStop(cutilsTcpServer *server){
	if(!server->started){
		return;
	}

	if(server->ev != NULL){
		event_del(server->ev);
		event_free(server->ev);
		server->ev = NULL;
	}
	server->useTimeout = server->useClientTimeout = false;

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
	server->clientTimeout.tv_sec = sec;
	server->clientTimeout.tv_usec = usec;

	server->useClientTimeout = true;
}

void cutilsTcpServerClearClientTimeout(cutilsTcpServer *server){
	server->useClientTimeout = false;
}
