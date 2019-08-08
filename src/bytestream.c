#include "bytestream.h"

int cutilsByteStreamInit(cutilsByteStream *stream, size_t capacity){
	if(capacity == 0){
		memset(stream, 0, sizeof(cutilsByteStream));
		return CUTILS_OK;
	}

	stream->stream = malloc(capacity);
	if(stream->stream == NULL){
		return CUTILS_NOMEM;
	}
	stream->size = 0;
	stream->capacity = capacity;

	return CUTILS_OK;
}

int cutilsByteStreamCopy(cutilsByteStream *dst, cutilsByteStream *src){
	cutilsByteStream tmp;
	int err = cutilsByteStreamInit(&tmp, src->capacity);
	if(err != CUTILS_OK){
		return err;
	}
	memcpy(tmp.stream, src->stream, src->size);
	tmp.size = src->size;

	if(dst->stream != NULL){
		cutilsByteStreamFree(dst);
	}
	*dst = tmp;

	return CUTILS_OK;
}

void cutilsByteStreamMove(cutilsByteStream *dst, cutilsByteStream *src){
	*dst = *src;
	memset(src, 0, sizeof(cutilsByteStream));
}

void cutilsByteStreamSwap(cutilsByteStream *a, cutilsByteStream *b){
	cutilsByteStream tmp = *a;
	*a = *b;
	*b = tmp;
}

void cutilsByteStreamFree(cutilsByteStream *stream){
	free(stream->stream);
	memset(stream, 0, sizeof(cutilsByteStream));
}

int cutilsByteStreamResize(cutilsByteStream *stream, size_t size){
	if(size <= stream->capacity){
		stream->size = size;
		return CUTILS_OK;
	}

	unsigned char *tmp = realloc(stream->stream, size);
	if(tmp == NULL){
		return CUTILS_NOMEM;
	}
	stream->stream = tmp;
	stream->size = stream->capacity = size;

	return CUTILS_OK;
}

int cutilsByteStreamReserve(cutilsByteStream *stream, size_t capacity){
	unsigned char *tmp = realloc(stream->stream, capacity);
	if(tmp == NULL){
		return CUTILS_NOMEM;
	}
	stream->stream = tmp;
	stream->capacity = capacity;
	stream->size = (stream->size > capacity?(capacity):(stream->size));

	return CUTILS_OK;
}

int cutilsByteStreamAppendByte(cutilsByteStream *stream, unsigned char x){
	int err = cutilsByteStreamResize(stream, stream->size+1);
	if(err != CUTILS_OK){
		return err;
	}

	stream->stream[stream->size] = x;
	stream->size++;

	return CUTILS_OK;
}

int cutilsByteStreamAppendStream(cutilsByteStream *stream, cutilsByteStream *x){
	int err = cutilsByteStreamResize(stream, stream->size+1);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(stream->stream+stream->size, x->stream, x->size);
	stream->size += x->size;

	return CUTILS_OK;
}

int cutilsByteStreamInsertByte(cutilsByteStream *stream, unsigned char x, size_t index){
	if(index > stream->size){
		return CUTILS_OUT_OF_BOUNDS;
	}

	int err = cutilsByteStreamResize(stream, stream->size+1);
	if(err != CUTILS_OK){
		return err;
	}

	memmove(stream->stream+index+1, stream->stream+index, stream->size-index);
	stream->stream[index] = x;

	return CUTILS_OK;
}

int cutilsByteStreamInsertStream(cutilsByteStream *stream, cutilsByteStream *x, size_t index){
	if(index > stream->size){
		return CUTILS_OUT_OF_BOUNDS;
	}

	int err= cutilsByteStreamResize(stream, stream->size+x->size);
	if(err != CUTILS_OK){
		return err;
	}

	memmove(stream->stream+index+x->size, stream->stream+index, stream->size-index);
	memcpy(stream->stream+index, x->stream, x->size);

	return CUTILS_OK;
}
