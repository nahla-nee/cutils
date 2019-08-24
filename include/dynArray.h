#ifndef CUTILS_DYNARRAY_H
#define CUTILS_DYNARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

#include "errors.h"

#define CUTILS_DEF_DYNARRAY_H(TYPE, NAME)\
	typedef struct NAME{\
		TYPE *data;\
		size_t size;\
		size_t capacity;\
	} NAME;\
\
	int NAME##Init(NAME *arr, size_t size);\
	NAME* NAME##New(size_t size);\
	int NAME##Copy(NAME *dst, NAME *src);\
	void NAME##Move(NAME *dst, NAME *src);\
	void NAME##Swap(NAME *a, NAME *b);\
	void NAME##Free(NAME *arr);\
	int NAME##Resize(NAME *arr, size_t size);\
	int NAME##Reserve(NAME *arr, size_t capacity);\
	int NAME##PushBack(NAME *arr, TYPE x);\
	int NAME##Insert(NAME *arr, TYPE x, size_t index);\
	int NAME##InsertPtr(NAME *arr, TYPE *x, size_t len, size_t index);\
	int NAME##InsertArr(NAME *arr, NAME *x, size_t index);\
	int NAME##Delete(NAME *arr, size_t index);\
	int NAME##DeleteRange(NAME *arr, size_t start, size_t end);

#define CUTILS_DEF_DYNARRAY_C(TYPE, NAME)\
	int NAME##Init(NAME *arr, size_t size){\
		arr->data = malloc(sizeof(TYPE)*size);\
		if(arr->data == NULL){\
			return CUTILS_NOMEM;\
		}\
		arr->capacity = size;\
		arr->size = 0;\
\
		return CUTILS_OK;\
	}\
\
	NAME* NAME##New(size_t size){\
		NAME *ret = malloc(sizeof(NAME));\
		if(ret == NULL){\
			return NULL;\
		}\
		if(NAME##Init(ret, size) != CUTILS_OK){\
			free(ret);\
			return NULL;\
		}\
		return ret;\
	}\
\
	int NAME##Copy(NAME *dst, NAME *src){\
		/*allocate the memory first before we free, so that if we cant allocate old mem is not lost*/\
		TYPE *tmp = malloc(sizeof(TYPE)*src->size);\
		if(tmp == NULL){\
			return CUTILS_NOMEM;\
		}\
\
		dst->data = tmp;\
		dst->size = src->size;\
		dst->capacity = dst->size;\
		memcpy(dst->data, src->data, sizeof(TYPE)*dst->size);\
\
		return CUTILS_OK;\
	}\
\
	void NAME##Move(NAME *dst, NAME *src){\
		*dst = *src;\
		memset(src, 0, sizeof(NAME));\
	}\
\
	void NAME##Swap(NAME *a, NAME *b){\
		NAME tmp = *a;\
		*a = *b;\
		*b = tmp;\
	}\
\
	void NAME##Free(NAME *arr){\
		free(arr->data);\
		memset(arr, 0, sizeof(NAME));\
	}\
\
	int NAME##Resize(NAME *arr, size_t size){\
		if(size <= arr->capacity){\
			arr->size = size;\
			return CUTILS_OK;\
		}\
\
		TYPE *tmp = realloc(arr->data, sizeof(TYPE)*size);\
		if(tmp == NULL){\
			return CUTILS_NOMEM;\
		}\
		arr->data = tmp;\
		arr->size = size;\
		arr->capacity = size;\
		return CUTILS_OK;\
	}\
\
	int NAME##Reserve(NAME *arr, size_t capacity){\
		TYPE *tmp = realloc(arr->data, sizeof(TYPE)*capacity);\
		if(tmp == NULL){\
			return CUTILS_NOMEM;\
		}\
		arr->data = tmp;\
		arr->capacity = capacity;\
		if(capacity < arr->size){\
			arr->size = capacity;\
		}\
		return CUTILS_OK;\
	}\
\
	int NAME##PushBack(NAME *arr, TYPE x){\
		int err = NAME##Resize(arr, arr->size+1);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		arr->data[arr->size-1] = x;\
		return CUTILS_OK;\
	}\
	int NAME##Insert(NAME *arr, TYPE x, size_t index){\
		if(index > arr->size){\
			return CUTILS_OUT_OF_BOUNDS;\
		}\
\
		size_t oldSize = arr->size;\
		int err = NAME##Resize(arr, arr->size+1);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		/*this function can act the same as PushBack if index == arr->size*/\
		if(index != oldSize){\
			memmove(arr->data+index+1, arr->data+index, sizeof(TYPE)*(oldSize-index));\
		}\
		arr->data[index] = x;\
\
		return CUTILS_OK;\
	}\
	int NAME##InsertPtr(NAME *arr, TYPE *x, size_t len, size_t index){\
		if(index > arr->size){\
			return CUTILS_OUT_OF_BOUNDS;\
		}\
\
		size_t oldSize = arr->size;\
		int err = NAME##Resize(arr, arr->size+len);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		if(index != oldSize){\
			memmove(arr->data+index+len, arr->data+index, sizeof(TYPE)*(oldSize-index));\
		}\
		memcpy(arr->data+index, x, sizeof(TYPE)*len);\
\
		return CUTILS_OK;\
	}\
	int NAME##InsertArr(NAME *arr, NAME *x, size_t index){\
		if(index > arr->size){\
			return CUTILS_OUT_OF_BOUNDS;\
		}\
\
		size_t oldSize = arr->size;\
		int err = NAME##Resize(arr, arr->size+x->size);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		if(index != oldSize){\
			memmove(arr->data+index+x->size, arr->data+index, sizeof(TYPE)*(oldSize-index));\
		}\
		memcpy(arr->data+index, x, sizeof(TYPE)*x->size);\
\
		return CUTILS_OK;\
	}\
	int NAME##Delete(NAME *arr, size_t index){\
		if(index >= arr->size){\
			return CUTILS_OUT_OF_BOUNDS;\
		}\
\
		memmove(arr->data+index, arr->data+index+1, sizeof(TYPE)*(arr->size-index-1));\
		arr->size--;\
\
		return CUTILS_OK;\
	}\
	int NAME##DeleteRange(NAME *arr, size_t start, size_t end){\
		if(start >= arr->size || end >= arr->size){\
			return CUTILS_OUT_OF_BOUNDS;\
		}\
\
		if(start == 0 && end == arr->size){\
			NAME##Resize(arr, 0);\
			return CUTILS_OK;\
		}\
\
		memmove(arr->data+start, arr->data+end+1, sizeof(TYPE)*(arr->size-end-1));\
		NAME##Resize(arr, arr->size-(end-start)-1);\
\
		return CUTILS_OK;\
	}

#endif
