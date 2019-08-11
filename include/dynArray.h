#ifndef CUTILS_DYNARRAY_H
#define CUTILS_DYNARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>

#include "errors.h"

#define CUTILS_DEF_DYNARRAY(TYPE, NAME)\
	typedef struct NAME{\
		TYPE *data;\
		size_t size;\
		size_t capacity;\
	} NAME;\
\
	int NAME##Init(NAME *arr, size_t size);\
	int NAME##Copy(NAME *dst, NAME *src);\
	void NAME##Move(NAME *dst, NAME *src);\
	void NAME##Free(NAME *arr);\
	int NAME##Resize(NAME *arr, size_t size);\
	int NAME##Reserve(NAME *arr, size_t capacity);\
	int NAME##PushBack(NAME *arr, TYPE x);\
	int NAME##Insert(NAME *arr, TYPE x, size_t index);\
\
	int NAME##Init(NAME *arr, size_t size){\
		arr->data = malloc(sizeof(TYPE)*size);\
		if(arr->data == NULL){\
			return CUTILS_NOMEM;\
		}\
		arr->size = size;\
\
		return CUTILS_OK;\
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
		arr->data = NULL;\
		arr->size = arr->capacity = 0;\
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
		arr->data[arr->size] = x;\
		arr->size++;\
		return CUTILS_OK;\
	}\
	int NAME##Insert(NAME *arr, TYPE x, size_t index){\
		int err = NAME##Resize(arr, arr->size+1);\
		if(err != CUTILS_OK){\
			return err;\
		}\
\
		/*this function can act the same as PushBack if index == arr->size*/\
		if(index != arr->size){\
			memmove(arr->data+index, arr->data+index+1, sizeof(TYPE)*(arr->size-index));\
		}\
		arr->data[index] = x;\
		arr->size++;\
		return CUTILS_OK;\
	}

#endif
