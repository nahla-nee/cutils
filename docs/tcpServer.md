# tcpServer

A struct to handle a tcp server.

## Struct data

`sockfd` Socket file descriptor.

`eb` The event_base used.

`ev` The event used.

`timeout` The timeout to use for listening socket.
Use appropriate function to modify this value.

`useTimeout` Whether to use the timeout value in `timeout` or not. Use appropriate
function to modify this value.

`clientTimeout` The timeout to use for client sockets.

`useClientTimeout` Whether to use the timeout value in `clientTimeout` or not.

`port` The port the server is open on.

`backlog` The server's max backlog.

`started` Whether the server has been started or not.

```c
typedef struct cutilsTcpServer{
	int sockfd;

	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	struct timeval clientTimeout;
	bool useClientTimeout;

	cutilsString port;
	int backlog;
	bool started;
} cutilsTcpServer;
```

## Functions/Macros

>`int cutilsTcpServerInit(cutilsTcpServer *server)`

Initializes the tcp server.

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`void cutilsTcpServerDeinit(cutilsTcpServer *server)`

Deinitializes the tcp server.

>`int cutilsTcpServerStart(cutilsTcpServer *server, const char *port, int backlog, void *usrptr, event_callback_fn callback)`

Starts the tcp server on `port`. `callback` is used as the callback function for
the structs `eb` with `usrptr` being passed as the custom user data when `callback`
is called.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_GETADDRINFO` if the call to `getaddrinfo` failed.
* `CUTILS_CREATE_EVENT` if the function failed to allocate memory/intialize..
the struct's `event` or `timeout`.
* `CUTILS_SOCKET` if the function failed to create a listening socket.

note: If `usrptr` is `NULL` then the user data
passed to the call back function is will be a pointer to `server`

>`void cutilsTcpServerStop(cutilsTcpServer *server)`

Stops the tcp server if it is running.

>`int cutilsTcpServerStartEventLoop(cutilsTcpServer *server)`

Calls `event_base_loop` with the `EVLOOP_NO_EXIT_ON_EMPTY` flag.

return value:

The return value of the `event_base_loop` call.

>`int cutilsTcpServerStopEventLoop(cutilsTcpServer *server)`

Calls `event_base_loopexit` with `NULL` for the timeout value.

return value:

The return value of the `event_base_loopexit` call.

>`int cutilsTcpServerForceStopEventLoop(cutilsTcpServer *server)`

Calls `event_base_loopbreak`.

return value:

The return value of the `event_base_loopbreak` call.

>`void cutilsTcpServerSetTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec)`

Sets the timeout on the struct's listening socket to `sec` and `usec`, and sets
the struct's `eb` to use the timeout`

>`void cutilsTcpServerClearTimeout(cutilsTcpServer *server)`

Clears the timeout and sets the struct's `eb` to not use a timeout.

>`void cutilsTcpServerSetClientTimeout(cutilsTcpServer *server, time_t sec, suseconds_t usec)`

Sets the struct's `clientTimeout` to `sec` and `usec`, and sets `useClientTimeout`
to `true`.

>`void cutilsTcpServerClearClientTimeout(cutilsTcpServer *server)`

Sets the struct's `useClientTimeout` to `false`.
