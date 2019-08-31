#include "bytestream.h"

CUTILS_DEF_DYNARRAY_C(unsigned char, cutilsByteStream, NULL);

CUTILS_DEF_DYNARRAY_C(cutilsByteStream, cutilsByteStreamArr, cutilsByteStreamArrDeinitCallback);

void cutilsByteStreamArrDeinitCallback(struct cutilsByteStreamArr *arr, void *usrData){
	for(size_t i = 0; i < arr->size; i++){
		cutilsByteStreamDeinit(&arr->data[i]);
	}
}
