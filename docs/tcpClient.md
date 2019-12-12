# tcpClient

A struct to handle connecting to a tcp server.

## Struct data

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
typedef struct cutilsTcpClient{
	int sockfd;

	struct event_base *eb;
	struct event *ev;
	struct timeval timeout;
	bool useTimeout;

	cutilsString serverAddr;
	bool connected;
} cutilsTcpClient;
```

## Functions/Macros

>`int cutilsTcpClientInit(cutilsTcpClient *client)`

Intializes the tcp client.

return value:
* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`void cutilsTcpClientDeinit(cutilsTcpClient *client)`

Deinitializes the tcp client.

>`int cutilsTcpClientConnect(cutilsTcpClient *client, const char *address, const char *port, event_callback_fn callback)`

This function attempts to connect the client to `address` at `port`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_GETADDRINFO` if the `gettaddrinfo` call failed.
* `CUTILS_CREATE_EVENT` if the function failed to create/initialize either the..
`event` or `timeout` part of the struct.

>`void cutilsTcpClientDisconnect(cutilsTcpClient *client)`

Disconnects the client if it is currently connected.

>`int cutilsTcpClientStartEventLoop(cutilsTcpClient *client)`

Calls `event_base_loop` on the structs `eb` with the `EVLOOP_NO_EXIT_ON_EMPTY` flag.

return value:

the return value of the `event_base_loop` call.

>`int cutilsTcpClientStopEventLoop(cutilsTcpClient *client)`

Calls `event_base_loopexit` on the structs `eb` with `NULL` for the timeout value.

return value:

the return value of the `event_base_loopexit` call.

>`int cutilsTcpClientForceStopEventLoop(cutilsTcpClient *client)`

Calls `event_base_loopbreak` on the structs `eb`.

return value:

the return value of the `event_base_loopbreak` call.

>`void cutilsTcpClientSetTimeout(cutilsTcpClient *client, time_t sec, suseconds_t usec)`

Sets the timeout to `sec` and `usec` and sets the struct's eb to use the timeout.

>`void cutilsTcpClientClearTimeout(cutilsTcpClient *client)`

Clears the timeout and sets the struct's eb to not use a timeout.
