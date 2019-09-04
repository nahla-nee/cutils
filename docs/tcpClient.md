#tcpClient

A struct to esablish a tcp connection. This struct, and all the other networking structs offer optional libevent integration.

There are functions in this docpage that have the same name. This is because some of them are made for libevent integration while others are not. Only one or the other will be defined.

##Struct

`sockfd` the socket file descriptor.

`eb` The event_base used in case the user wants to use libevent.

`ev` The event struct associated with `sockfd`.

`timeout` A timeout struct in case the user wants to use a timeout with `ev`

`useTimeout` Whether the event is currently using a timeout or not.

`server` The string address of the server we are connected to.

`buffer` A buffer used to communicate with the server.

`connected` Whether the client is connected to a server or not

```c
typedef struct cutilsTcpClient{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;
	#endif

	cutilsString server;
	cutilsByteStream buffer;
	bool connected;
} cutilsTcpClient;
```

>`int cutilsTcpClientInit(cutilsTcpClient *client, size_t bufferSize)`

Initializes the client, and gives the buffer a size of `bufferSize`. Will also allocate the event_base if libevent integration is enabled.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`cutilsTcpClient* cutilsTcpClientNew(size_t bufferSize)`

Allocates a `cutilsTcpClient` struct, and intializes it with `bufferSize`.

This function returns a pointer to a `cutilsTcpClient` struct if no errors occured, or a `NULL` pointer if it failed to allocte the struct, or initialize it.

>`void cutilsTcpClientDeinit(cutilsTcpClient *client)`

This function deintializes, and frees all data allocated by the client. It also disconnects the client if it is connected to a server.

>`void cutilsTcpClientFree(cutilsTcpClient *client)`

This function deintializes, and frees all data allocated by the client. It also disconnects the client if it is connected to a server. After it frees the client. Use this function with clients allocated with `cutilsTcpClientNew`.

>`int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address, const char *port, event_callback_fn callback)`

Attempts to connect to the server specified by `address`, and `port`. It also creates, and adds the socket to `client->eb` with the flags `EV_READ | EV_PERSIST`, and the callback `callback`. The user pointer sent to callback will be a pointer to `client`.

This function returns `CUTILS_OK` if no errors occured, `CUTILS_GETADDRINFO` if the call to `getaddrinfo` failed, `CUTILS_SOCKET` if it failed to create, and connect a socket to the server, or `CUTILS_CREATE_EVENT` if it failed to allocate, or add the event to `client->eb`. 

>`int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address, const char *port)`

This function is the same as the one above, however it excludes all errors that can be caused by libevent, and does not require a callback.

>`void cutilsTcpClientDisconnect(cutilsTcpClient *client)`

Disconnect a client from the current server.

>`int cutilsTcpClientStartEventLoop(cutilsTcpClient *client)`

With libevent integration only. Starts the event loop associated with `client->eb` with the flag `EVLOOP_NO_EXIT_ON_EMPTY`.

This function returns the value of the call to `event_base_loop`.

>`int cutilsTcpClientStopEventLoop(cutilsTcpClient *client)`

With libevent integration only. Stops the event loop after all callbacks have finished executing.

This function returns the value of the call to `event_base_loopexit`.

>`int cutilsTcpClientForceStopEventLoop(cutilsTcpClient *client)`

With libevent integration only. Stops the event loop immidiately.

This function returns the value of the call to `event_base_loopbreak`.

>`void cutilsTcpClientSetTimeout(cutilsTcpClient *client, time_t sec, suseconds_t usec)`

Tells the client to use a timeout with the values of `sec` and `usec` when connecting. If the client is already connected it calls `event_add` on `client->ev` to use the specified timeout.

>`void ctuilsTcpClientClearTimeout(cutilsTcpClient *client)`

Tells the client to not use a timeout. If the client is already connected it calls `event_add` on `client->ev` to not use a timeout.
