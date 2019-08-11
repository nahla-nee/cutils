#ifndef CUTILS_BYTESTREAM_H
#define CUTILS_BYTESTREAM_H

#include "errors.h"
#include "dynArray.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

//really similar to string, except it has no null temrinator

typedef struct cutilsByteStream{
	unsigned char *stream;
	size_t size;
	size_t capacity;
} cutilsByteStream;

CUTILS_DEF_DYNARRAY_H(cutilsByteStream, cutilsByteStreamArr);

int cutilsByteStreamInit(cutilsByteStream *stream, size_t capacity);
int cutilsByteStreamCopy(cutilsByteStream *dst, cutilsByteStream *src);
void cutilsByteStreamMove(cutilsByteStream *dst, cutilsByteStream *src);
void cutilsByteStreamSwap(cutilsByteStream *a, cutilsByteStream *b);
void cutilsByteStreamFree(cutilsByteStream *stream);

int cutilsByteStreamResize(cutilsByteStream *stream, size_t size);
int cutilsByteStreamReserve(cutilsByteStream *stream, size_t capacity);

int cutilsByteStreamAppendByte(cutilsByteStream *stream, unsigned char x);
int cutilsByteStreamAppendStream(cutilsByteStream *stream, cutilsByteStream *x);

int cutilsByteStreamInsertByte(cutilsByteStream *stream, unsigned char x, size_t index);
int cutilsByteStreamInsertStream(cutilsByteStream *stream, cutilsByteStream *x, size_t index);

#define cutilsByteStreamAppend(STREAM, X) _Generic((X),\
		char: cutilsByteStreamAppendByte,\
		const char: cutilsByteStreamAppendByte,\
		unsigned char: cutilsByteStreamAppendByte,\
		const unsigned char: cutilsByteStreamAppendByte,\
		int: cutilsByteStreamAppendByte,\
		const int: cutilsByteStreamAppendByte,\
		cutilsByteStream*: cutilsByteStreamAppendStream,\
		const cutilsByteStream*: cutilsByteStreamAppendStream\
	) (STREAM, X)

#define cutilsByteStreamInsert(STREAM, X, INDEX) _Generic((X),\
		char: cutilsByteStreamInsertByte,\
		const char: cutilsByteStreamInsertByte,\
		unsigned char: cutilsByteStreamInsertByte,\
		const unsigned char: cutilsByteStreamInsertByte,\
		int: cutilsByteStreamInsertByte,\
		const int: cutilsByteStreamInsertByte,\
		cutilsByteStream*: cutilsByteStreamInsertStream,\
		const cutilsByteStream*: cutilsByteStreamInsertStream\
	) (STREAM, X, INDEX)

#endif
