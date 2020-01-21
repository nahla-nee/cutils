#ifndef CUTILS_ERRORS_H
#define CUTILS_ERRORS_H

#define CUTILS_OK 0
#define CUTILS_NOMEM 1
#define CUTILS_OUT_OF_BOUNDS 2
#define CUTILS_FOPEN 3

const char* cutilsErrorToStr(int code);

#endif
