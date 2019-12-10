# C Utils

A C library of some common utilities.

All documentation can be found in `docs/`

## Utilities available:

* bytestream
	A dynamic array of bytes, implemented using dynArray.
* cliOptions
	Struct and utility functions to parse arugments passed to program.
* dynArray
	A set of two macros to create a dynamic array struct along with it's functions.
* file
	A struct to make reading and writing to files easier.
* linkedList
	A double linked list implementation.
* logger
	A simple logging implementation.
* str
	A simple string implementation.
* tcpClient
	A set of macros to add tcp client capabilities to any struct with optional
	libevent support with optional libevent support.
* tcpServer
	A set of macros to add tcp server capabilities to any struct with optional
	libevent support with optional libevent support.
* tcpServerClient
	A set of macros to add tcp server client (how the server represents a
	client) capabilities to any struct with optional libevent support.

