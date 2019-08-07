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

int cutilsStringInit(cutilsString *str, size_t len);
int cutilsStringCopy(cutilsString *dst, cutilsString *src);
void cutilsStringMove(cutilsString *dst, cutilsString *src);
void cutilsStringFree(cutilsString *str);

int cutilsStringResize(cutilsString *str, size_t len);
int cutilsStringResizeChar(cutilsString *str, size_t len, char c);
int cutilsStringReserve(cutilsString *str, size_t capcity);

int cutilsStringInsertChar(cutilsString *str, char x, size_t index);
int cutilsStringInsertCStr(cutilsString *str, const char *x, size_t index);
int cutilsStringInsertString(cutilsString *str, const cutilsString *x, size_t index);

int cutilsStringSetChar(cutilsString *str, char x);
int cutilsStringSetCStr(cutilsString *str, const char *x);
int cutilsStringSetString(cutilsString *str, const cutilsString *x);

#define cutilsStringInsert(str, X, index) _Generic((X),\
		char: cutilsStringInsertChar,\
		const char: cutilsStringInsertChar,\
		int: cutilsStringInsertChar,\
		const int: cutilsStringInsertChar,\
		char *: cutilsStringInsertCStr,\
		const char *: cutilsStringInsertCStr,\
		cutilsString *: cutilsStringInsertString,\
		const cutilsString *: cutilsStringInsertString,\
		default: cutilsStringInsertChar\
	)(str, X, index)

#define cutilsStringSet(str, X) _Generic((X),\
		char: cutilsStringSetChar,\
		const char: cutilsStringSetChar,\
		int: cutilsStringSetChar,\
		const int: cutilsStringSetChar,\
		char *: cutilsStringSetCStr,\
		const char *: cutilsStringSetCStr,\
		cutilsString *: cutilsStringSetString,\
		const cutilsString *: cutilsStringSetString,\
		default: cutilsStringSetChar\
	)(str, X)

#endif
