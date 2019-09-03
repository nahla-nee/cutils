#include "bytestream.h"

CUTILS_DEF_DYNARRAY_C(char, cutilsByteStream, NULL);

CUTILS_DEF_DYNARRAY_C(cutilsByteStream, cutilsByteStreamArr, cutilsByteStreamArrRemoveCallback);

void cutilsByteStreamArrRemoveCallback(cutilsByteStream *arr, size_t count, void *usrData){
	for(size_t i = 0; i < count; i++){
		cutilsByteStreamDeinit(arr+i);
	}
}
