# tcpClient

A set of macros to give any struct tcp client capabilities with optional libevent
support. If you don't want to use libevent define `CUTILS_NO_LIBEVENT` before
including any header files. This is made up of three macros: `CUTILS_DEF_TCP_CLIENT_STRUCT`,
`CUTILS_DEF_TCP_CLIENT_H`, and `CUTILS_DEF_TCP_CLIENT_C`. If you're using windows
you must initialize winsock yourself.

## Struct data

This set of macros does not define it's own struct, rather it is meant to be
placed in another structs definition, which will add the needed members to the
struct that macro being `CUTILS_DEF_TCP_CLIENT_STRUCT`.

`sockfd` Socket file descriptor.

`eb` The event_base used.

`ev` The event used.

`timeout` The timeout to use if any. Use appropriate
function to modify this value.

`useTimeout` Whether to use the timeout value in `timeout` or not. Use appropriate
function to modify this value.

`serverAddr` The server's IP address as a string.

`connected` Whether the client is connected to a server or not.

```c
struct client{
	CUTILS_DEF_TCP_CLIENT_STRUCT
};

//expands to
struct client{
	int sockfd;

	//these are only defined if you're using CUTILS_NO_LIBEVENT
	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	cutilsString serverAddr;
	bool connected;
};
```

## Functions/Macros

>`CUTILS_DEF_TCP_CLIENT_H(STRUCT_NAME)`

defines the needed function prototypes.

note: `STRUCT_NAME` must be the same as the name of the struct that you inserted
`CUTILS_DEF_TCP_CLIENT_STRUCT` into.

note: `STRUCT_NAME` must be the same as the one passed to `CUTILS_DEF_TCP_CLIENT_C`.

>`CUTILS_DEF_TCP_CLIENT_C(STRUCT_NAME)`

defines the functions to be used with the struct.

note: `STRUCT_NAME` must be the same as the name of the struct that you inserted
`CUTILS_DEF_TCP_CLIENT_STRUCT` into.

note: `STRUCT_NAME` must be the same as the one passed to `CUTILS_DEF_TCP_CLIENT_H`.

>`int STRUCT_NAME##TcpInit(STRUCT_NAME *client)`

Intializes the tcp client part of the given struct.

return value:
* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client)`

Deinitializes the tcp client part of the given struct.

>`int STRUCT_NAME##TcpConnect(STRUCT_NAME *client, const char *address, const char *port, event_callback_fn callback)`

if `CUTILS_NO_LIBEVENT` is defined:

>`int STRUCT_NAME##TcpConnect(STRUCT_NAME *client, const char *address, const char *port)`

This function attempts to connect the client to `address` at `port`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_GETADDRINFO` if the `gettaddrinfo` call failed.
* `CUTILS_CREATE_EVENT` if the function failed to create/initialize either the..
`event` or `timeout` part of the struct (only if `CUTILS_NO_LIBEVENT` is not defined).

>`void STRUCT_NAME##TcpDisconnect(STRUCT_NAME *client)`

Disconnects the client from whatever server it is currently connected to.

>`int STRUCT_NAME##TcpStartEventLoop(STRUCT_NAME *client)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Calls
`event_base_loop` on the structs `eb` with the `EVLOOP_NO_EXIT_ON_EMPTY` flag.

return value:

the return value of the `event_base_loop` call.

>`int STRUCT_NAME##TcpStopEventLoop(STRUCT_NAME *client)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. calls
`event_base_loopexit` on the structs `eb` with `NULL` for the timeout value.

return value:

the return value of the `event_base_loopexit` call.

>`int STRUCT_NAME##TcpForceStopEventLoop(STRUCT_NAME *client)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. calls
`event_base_loopbreak` on the structs `eb`.

return value:

the return value of the `event_base_loopbreak` call.

>`void STRUCT_NAME##TcpSetTimeout(STRUCT_NAME *client, time_t sec, suseconds_t usec)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Sets the
timeout to `sec` and `usec` and sets the struct's eb to use the timeout.

>`void STRUCT_NAME##TcpClearTimeout(STRUCT_NAME *client)`

This function can only be used if `CUTILS_NO_LIBEVENT` is not defined. Clears
the timeout and sets the struct's eb to not use a timeout.
