#ifndef CUTILS_BYTESTREAM_H
#define CUTILS_BYTESTREAM_H

#include "errors.h"
#include "dynArray.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

CUTILS_DEF_DYNARRAY_H(unsigned char, cutilsByteStream);

CUTILS_DEF_DYNARRAY_H(cutilsByteStream, cutilsByteStreamArr);

#endif
