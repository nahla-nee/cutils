# tcpServer

A struct to create a tcp server. This struct, and all other networking structs offer optional libevent integration.

There are functions in this docpage that have the same name. This is because some of them are made for libevent integration while others are not. Only one or the other will be defined.

## Struct

`sockfd` The server's listening socket.

`clients` A linked list of `cutilsTcpServerClient`.

`eb` The event_base used in case the user wants to use libevent.

`ev` The event struct associated with `sockfd`

`timeout` A timeout struct in case the user wants to use a timeout with `ev`

`useTimeout` Whether `ev` is currently using a timeout or not.

`timeoutClient` A timeout struct in case the user wants to use a timeout for the clients.

`useTimeoutClient` Whether the clients are currently using a timeout or not.

`clientBufferSize` The default buffer size for newly created clients.

`port` The port the server is running on.

`backlog` The server's maximum backlog for accept.

`started` Whether the server has been started or not.

```c
typedef struct cutilsTcpServer{
	int sockfd;
	cutilsTcpServerClientLL clients;

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
```

## Functions

>`int cutilsTcpServerInit(cutilsTcpServer *server, size_t clientBufferSize)`

intitializes the server, and sets the default buffer size for clients.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`cutilsTcpServer* cutilsTcpServerNew(size_t clientBufferSize)`

Allocates a `cutilsTcpServer` and intializes it with `clientBufferSize`.

This function returns a pointer to a `cutilsTcpServer` struct if no errors occured, or `NULL` if it failed to allocate memory for the struct, or initialize it.

>`void cutilsTcpServerDeinit(cutilsTcpServer *server)`

Stops the server if it is started, deinitializes the server, and frees all its memory.

>`void cutilsTcpServerFree(cutilsTcpServer *server)`

Stops the server if it is started, deintializes the server, and frees all its memory. It will also free the server struct. Use this function for structs allocated with `cutilsTcpServerNew`.

>`int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog, event_callback_fn callback)`

Starts the server on `port` with a backlog of `backlog`. If the server is already running it will close it, and open it again with the given port, and backlog. Will assign `callback` to be the function call back if `server->sockfd` is ready for reading. adds `server->ev` to `server->eb` with the flags `EV_READ | EV_PERSIST`, and the user pointer points to `server`.

This function returns `CUTILS_OK` if no errors occured, `CUTILS_GETADDRINFO` if the call to `getaddrinfo` failed, `CUTILS_SOCKET` if it failed to create a listening socket, or `CUTILS_EVENT_CREATE` if it failed to allocate, or add the event.

>`int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog)`

This function is identical to the above function except all the arguments, and errors relating to libevent do not apply to this function.

>`void cutilsTcpServerClose(cutilsTcpServer *server)`

Closes the server, and all its connections if the server is started.

>`int cutilsTcpServerStartEventLoop(cutilsTcpServer *server)`

With libevent integration only. Starts the event loop associated with `server->eb`, with the flag `EVLOOP_NO_EXIT_ON_EMPTY`.

This function returns the value of the call to `event_base_loop`.

>`int cutilsTcpServerStopEventLoop(cutilsTcpServer *server)`

With libevent integration only. Stops the event loop after all callbacks have been executed.

This function returns the value of the call to `event_base_loopexit`.

>`int cutilsTcpServerForceStopEventLoop(cutilsTcpServer *server)`

With libevent integration only. Stops the event loop immediately.

This function returns the value of the call to `event_base_loopbreak`.

>`void cutilsTcpServerSetTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec)`

With libevent integration only. Tells the server to use the timeout for the server sockfd, if the server is started it modifies it to use the new timeout.

>`void cutilsTcpServerClearTimeout(cutilsTcpServer *server)`

With libevent integration only. Tells the server not to use a timeout for the sockfd, if the server is started it modifies it to not use a timeout.

>`void cutilsTcpServerSetClientTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec)`

With libevent integration only. Tells server to use the timeout for the clients, if the server is started it modifies all current clients to use the new timeout.

>`void cutilsTcpServerClearClientTimeout(cutilsTcpServer *server)`

With libevent integration only. Tells the server not to usre a timeout for the clients, if the server is started it modifies all current clients to not use a timeout.

>`int cutilsTcpServerAddClient(cutilsTcpServer *server, int sockfd,
	struct sockaddr addr, socklen_t addrLen, event_callback_fn callback)`

Adds a client to the back of the linked list of clients. `callback` will be called when the client sockfd is ready for reading. Adds the clients event to `server->eb` with the flags `EV_READ | EV_PERSIST`. The user pointer will point to the client struct.

This function returns `CUTILS_OK` if no error occured, `CUTILS_NOMEM` if it failed to allocate the required memory, or `CUTILS_CREATE_EVENT` if it failed to allocate or add the event to `server->eb`.

>`int cutilsTcpServerAddClient(cutilsTcpServer *server, int sockfd,
	struct sockaddr addr, socklen_t addrLen)`

This function is identical to the function above, except all arguments, and errors relating to libevent do not apply to this function.

>`void cutilsTcpServerRemoveClient(cutilsTcpServer *server, cutilsTcpServerClient *client)`

Thsi function removed `client` from the `server->clients` linked list, it aso deintializes the client.


