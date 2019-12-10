# tcpServerClient

A set of macros to give any struct the capability to be a tcp server client. Use
this to create/manage clients from the server side. If you don't want to use libevent
define `CUTILS_NO_LIBEVENT` before including any header files. This is made up
of three macros: `CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT`, `CUTILS_DEF_TCP_SERVER_CLIENT_H`,
and `CUTILS_DEF_TCP_SERVER_CLIENT_C`. If you are using windows you must initialize
winsock yourself.

## Struct data

This set of macros does not define it's own struct, rather it is meant to be
placed in another structs definition, which will add the needed members to the
struct that macro being `CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT`.

`sockfd` The socket file descriptor

`ev` The event to be used.

`clientAddress` The address of the client.

```c
struct serverClient{
	CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT
};

//expands to
struct serverClient{
	int sockfd;

	//only if you're using libevent
	struct event *ev;

	cutilsString clientAddress;
};
```

## Functions/Macros

>`CUTILS_DEF_TCP_SERVER_CLIENT_H(STRUCT_NAME)`

defines the needed function prototypes.

note: `STRUCT_NAME` must be the same as the name of the struct that you inserted
`CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT` into.

note: `STRUCT_NAME` must be the same as the one passed to `CUTILS_DEF_TCP_SERVER_CLIENT_C`.

>`CUTILS_DEF_TCP_SERVER_CLIENT_C(STRUCT_NAME)`

defines the functions to be used with the struct.

note: `STRUCT_NAME` must be the same as the name of the struct that you inserted
`CUTILS_DEF_TCP_SERVER_CLIENT_STRUCT` into.

note: `STRUCT_NAME` must be the same as the one passed to `CUTILS_DEF_TCP_SERVER_CLIENT_H`.

>`int STRUCT_NAME##TcpInit(STRUCT_NAME *client, int sockfd, struct sockaddr address, struct event_base *eb, struct timeval *timeout, event_callback_fn callback)`

if `CUTILS_NO_LIBEVENT` is defined:

>`int STRUCT_NAME##TcpInit(STRUCT_NAME *client, int sockfd, struct sockaddr address)`

Initializes the tcp server client part of the struct with the given `sockfd` and
`address`. It will also create, and register the event into `eb`'s event loop
with `timeout` as the timeout value, and `callback` as its callback function. If
you wish to not register/create an event for a client then pass `NULL` for `eb`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.
* `CUTILS_CREATE_EVENT` if the function failed to create the event or add the..
timeout (only if `CUTILS_NO_LIBEVENT` is not defined).

>`void STRUCT_NAME##TcpDeinit(STRUCT_NAME *client)`

Deinitializes the tcp server client part of the struct
