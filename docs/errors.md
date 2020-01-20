# errors

A set of macros that define all errors that can be returned by this libraries
functions, as well as a function to convert each code to a string.

## Codes

* `CUTILS_OK`: no error.
* `CUTILS_NOMEM`: failed to allocate memory.
* `CUTILS_OUT_OF_BOUNDS`: argument goes out of bounds.
* `CUTILS_FOPEN`: failed to open file.
* `CUTILS_GETADDRINFO`: failed to call `getaddrinfo`.
* `CUTILS_SOCKET`: failed to create socket.
* `CUTILS_CREATE_EVENT`: failed to create/initialize event/timeout struct.

## Functions

> `const char* cutilsErrorToStr(int code)`

returns an error message that matches `code`.
