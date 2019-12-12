# tcpServerClient

A struct to represent a client to a server.

## Struct data

`sockfd` The socket file descriptor

`ev` The event to be used.

`clientAddress` The address of the client.

```c
typedef struct cutilsTcpServerClient{
	int sockfd;

	struct event *ev;

	cutilsString clientAddress;
} cutilsTcpServerClient;
```

## Functions/Macros

>`int cutilsTcpServerClientInit(cutilsTcpServerClient *client, int sockfd, struct sockaddr address, struct event_base *eb, struct timeval *timeout, event_callback_fn callback)`

Initializes the tcp server client with the given `sockfd` and `address`. It will
also create, and register the event into `eb`'s event loop with `timeout` as the
timeout value, and `callback` as its callback function. If you wish to not register/create
an event for a client then pass `NULL` for `eb`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.
* `CUTILS_CREATE_EVENT` if the function failed to create the event or add the timeout

>`void cutilsTcpServerClientDeinit(cutilsTcpServerClient *client)`

Deinitializes the tcp server client.
