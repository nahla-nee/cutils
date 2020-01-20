#ifndef CUTILS_ERRORS_H
#define CUTILS_ERRORS_H

#define CUTILS_OK 0
#define CUTILS_NOMEM 1
#define CUTILS_OUT_OF_BOUNDS 2
#define CUTILS_FOPEN 3
#define CUTILS_GETADDRINFO 4
#define CUTILS_SOCKET 5
#ifndef CUTILS_NO_LIBEVENT
#define CUTILS_CREATE_EVENT 6
#endif

const char* cutilsErrorToStr(int code);

#endif
