#ifndef CUTILS_STR_H
#define CUTILS_STR_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "errors.h"

typedef struct cutilsString{
	char *str;
	size_t len;
	size_t capacity;
} cutilsString;

/*
preallocates memory for the string, if successful it will have len bytes to hold chars in
if len is 0 then this function will just zero out the struct and return
*/
int cutilsStringInit(cutilsString *str, size_t capacity);

int cutilsStringCopy(cutilsString *dst, cutilsString *src);
void cutilsStringMove(cutilsString *dst, cutilsString *src);
void cutilsStringSwap(cutilsString *a, cutilsString *b);
void cutilsStringFree(cutilsString *str);

int cutilsStringResize(cutilsString *str, size_t len);
int cutilsStringResizeRepeat(cutilsString *str, size_t len, char c);
int cutilsStringReserve(cutilsString *str, size_t capcity);

int cutilsStringAppendChar(cutilsString *str, char x);
int cutilsStringAppendCStr(cutilsString *str, const char *x);
int cutilsStringAppendString(cutilsString *str, cutilsString *x);

int cutilsStringInsertChar(cutilsString *str, char x, size_t index);
int cutilsStringInsertCStr(cutilsString *str, const char *x, size_t index);
int cutilsStringInsertString(cutilsString *str, const cutilsString *x, size_t index);

int cutilsStringSetChar(cutilsString *str, char x);
int cutilsStringSetCStr(cutilsString *str, const char *x);
int cutilsStringSetString(cutilsString *str, const cutilsString *x);

#define cutilsStringAppend(STR, X) _Generic((X),\
		char: cutilsStringAppendChar,\
		const char: cutilsStringAppendChar,\
		int: cutilsStringAppendChar,\
		const int: cutilsStringAppendChar,\
		char *: cutilsStringAppendCStr,\
		const char *: cutilsStringAppendCStr,\
		cutilsString *: cutilsStringAppendString,\
		const cutilsString *: cutilsStringAppendString\
	)(STR, X)

#define cutilsStringInsert(STR, X, INDEX) _Generic((X),\
		char: cutilsStringInsertChar,\
		const char: cutilsStringInsertChar,\
		int: cutilsStringInsertChar,\
		const int: cutilsStringInsertChar,\
		char *: cutilsStringInsertCStr,\
		const char *: cutilsStringInsertCStr,\
		cutilsString *: cutilsStringInsertString,\
		const cutilsString *: cutilsStringInsertString\
	)(STR, X, INDEX)

#define cutilsStringSet(STR, X) _Generic((X),\
		char: cutilsStringSetChar,\
		const char: cutilsStringSetChar,\
		int: cutilsStringSetChar,\
		const int: cutilsStringSetChar,\
		char *: cutilsStringSetCStr,\
		const char *: cutilsStringSetCStr,\
		cutilsString *: cutilsStringSetString,\
		const cutilsString *: cutilsStringSetString\
	)(STR, X)

#endif
