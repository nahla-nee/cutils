# tcpServer

A set of macros to give any struct tcp server capabilities with optional libevent
support. If you don't want to use libevent define `CUTILS_NO_LIBEVENT` before
including any header files. This is made up of three macros: `CUTILS_DEF_TCP_SERVER_STRUCT`,
`CUTILS_DEF_TCP_SERVER_H`, and `CUTILS_DEF_TCP_SERVER_C`. If you're using windows
you must initialize winsock yourself.

## Struct data

This set of macros does not define it's own struct, rather it is meant to be
placed in another structs definition, which will add the needed members to the
struct that macro being `CUTILS_DEF_TCP_SERVER_STRUCT`.

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
struct server{
	CUTILS_DEF_TCP_SERVER_STRUCT
};

//expands
struct server{
	int sockfd;

	//only if you're using libevent
	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;
	struct timeval clientTimeout;
	bool useClientTimeout;

	cutilsString port;
	int backlog;
	bool started;
};
```

## Functions/Macros

>`CUTILS_DEF_TCP_SERVER_H(STRUCT_NAME)`

defines the needed function prototypes.

note: `STRUCT_NAME` must be the same as the name of the struct that you inserted
`CUTILS_DEF_TCP_SERVER_STRUCT` into.

note: `STRUCT_NAME` must be the same as the one passed to `CUTILS_DEF_TCP_SERVER_C`.

>`CUTILS_DEF_TCP_SERVER_C(STRUCT_NAME)`

defines the functions to be used with the struct.

note: `STRUCT_NAME` must be the same as the name of the struct that you inserted
`CUTILS_DEF_TCP_SERVER_STRUCT` into.

note: `STRUCT_NAME` must be the same as the one passed to `CUTILS_DEF_TCP_SERVER_H`.

>`int STRUCT_NAME##TcpInit(STRUCT_NAME *server)`

Initializes the tcp server part of the given struct.

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`void STRUCT_NAME##TcpDeinit(STRUCT_NAME *server)`

Deinitializes the tcp server part of the struct.

>`int STRUCT_NAME##TcpStart(STRUCT_NAME *server, const char *port, int backlog, void *usrptr, event_callback_fn callback)`

if `CUTILS_NO_LIBEVENT` is defined:

>`int STRUCT_NAME##TcpStart(STRUCT_NAME *server, const char *port, int backlog)`

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

>`void STRUCT_NAME##TcpStop(STRUCT_NAME *server)`

Stops the tcp server if it is running.

>`int STRUCT_NAME##TcpStartEventLoop(STRUCT_NAME *server)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Calls
`event_base_loop` with the `EVLOOP_NO_EXIT_ON_EMPTY` flag.

return value:

The return value of the `event_base_loop` call.

>`int STRUCT_NAME##TcpStopEventLoop(STRUCT_NAME *server)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Calls
`event_base_loopexit` with `NULL` for the timeout value.

return value:

The return value of the `event_base_loopexit` call.

>`int STRUCT_NAME##TcpForceStopEventLoop(STRUCT_NAME *server)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Calls
`event_base_loopbreak`.

return value:

The return value of the `event_base_loopbreak` call.

>`void STRUCT_NAME##TcpSetTimeout(STRUCT_NAME *server, time_t sec, suseconds_t usec)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Sets the
timeout on the struct's listening socket to `sec` and `usec`, and sets the struct's
`eb` to use the timeout`

>`void STRUCT_NAME##TcpClearTimeout(STRUCT_NAME *server)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Clears the
timeout and sets the struct's `eb` to not use a timeout.

>`void STRUCT_NAME##TcpSetClientTimeout(STRUCT_NAME *server, time_t sec, suseconds_t usec)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Sets the
struct's `clientTimeout` to `sec` and `usec`, and sets `useClientTimeout` to `true`.

>`void STRUCT_NAME##TcpClearClientTimeout(STRUCT_NAME *server)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Sets the
struct's `useClientTimeout` to `false`.
