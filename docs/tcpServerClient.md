# tcpServerClient

The struct `tcpServer` uses to represent clients. This header also defines a linked list of this struct alongside the proper remove callback.


## Struct

`sockfd` The socket file descriptor.

`ev` The event struct associated with `sockfd`

`server` The server struct this client belongs to.

`address` The client's address.

`buffer` The client's buffer.

`node` A pointer to the node that contains this struct.

```c
typedef struct cutilsTcpServerClient{
	int sockfd;

	#ifndef CUTILS_NO_LIBEVENT
	struct event *ev;
	#endif

	cutilsTcpServer *server;
	cutilsString address;
	cutilsByteStream buffer;

	cutilsTcpServerClientLLNode *node;
} cutilsTcpServerClient;
```

## Functions

>`int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen, event_callback_fn callback)`

Intializes the the client with the given sockfd, and it's address. Adds `client-ev` to to `server->eb` with the flags `EV_READ | EV_PERSIST`, and the user pointer pointing to `client`. You can pass `NULL` for `callback` if you don't want to use libevent.

This functions returns `CUTILS_OK` if no errors occurred, `CUTILS_NOMEM` if failed to allocate the required memory, or `CUTILS_CREATE_EVENT` if it failed to allocate, or add the event to `server->eb`.

>`int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, cutilsTcpServer *server,
	struct sockaddr addr, socklen_t addrLen)`

This function is identical to the above function except all the arguments, and errors relating to libevent do not apply to this function.

>`void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client)`

Deintializes the client by freeing all memory allocated, deleting, and freeing the event if allocated.
