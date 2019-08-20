#include "str.h"

CUTILS_DEF_DYNARRAY_C(cutilsString, cutilsStringArr);

int cutilsStringInit(cutilsString *str, size_t capacity){
	//allocate one more byte for null terminator
	str->str = calloc(capacity+1, 1);
	if(str->str == NULL){
		return CUTILS_NOMEM;
	}
	str->len = 0;
	str->capacity = capacity;
	return CUTILS_OK;
}

cutilsString* cutilsStringNew(size_t capacity){
	cutilsString *ret = malloc(sizeof(cutilsString));
	if(ret == NULL){
		return NULL;
	}

	if(cutilsStringInit(ret, capacity) != CUTILS_OK){
		free(ret);
		return NULL;
	}

	return ret;
}

int cutilsStringCopy(cutilsString *dst, cutilsString *src){
	char *tmp = malloc(src->len);
	if(tmp == NULL){
		return CUTILS_NOMEM;
	}
	if(dst->str != NULL){
		cutilsStringFree(dst);
	}
	dst->str = tmp;
	dst->len = src->len;
	dst->capacity = src->capacity;
	memcpy(dst->str, src->str, src->len);

	return CUTILS_OK;
}

void cutilsStringMove(cutilsString *dst, cutilsString *src){
	*dst = *src;
	memset(src, 0, sizeof(cutilsString));
}

void cutilsStringSwap(cutilsString *a, cutilsString *b){
	cutilsString tmp = *a;
	*a = *b;
	*b = tmp;
}

void cutilsStringFree(cutilsString *str){
	free(str->str);
	memset(str, 0, sizeof(cutilsString));
}

int cutilsStringResize(cutilsString *str, size_t len){
	if(len >= str->capacity){
		char *tmp = realloc(str->str, len+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = len+1;
	}

	str->len = len;
	str->str[len] = '\0';

	return CUTILS_OK;
}

int cutilsStringResizeRepeat(cutilsString *str, size_t len, char c){
	cutilsStringResize(str, len);

	if(len > str->len){
		//repeat the last character of the string to fill up the space
		memset(str->str+str->len, c, len-str->len);
	}

	return CUTILS_OK;
}

int cutilsStringReserve(cutilsString *str, size_t capacity){
	char *tmp = realloc(str->str, capacity);
	if(tmp == NULL){
		return CUTILS_NOMEM;
	}
	str->str = tmp;
	str->capacity = capacity;
	if(str->len >= capacity){
		str->len = capacity-1;
		str->str[capacity] = '\0';
	}

	return CUTILS_OK;
}

int cutilsStringAppendChar(cutilsString *str, char x){
	int err = cutilsStringResize(str, str->len+1);
	if(err != CUTILS_OK){
		return err;
	}

	str->str[str->len] = x;
	str->len++;

	return CUTILS_OK;
}

int cutilsStringAppendCStr(cutilsString *str, const char *x){
	size_t xLen = strlen(x);
	int err = cutilsStringResize(str, str->len+xLen);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(str+str->len, x, xLen);
	str->len += xLen;

	return CUTILS_OK;
}

int cutilsStringAppendString(cutilsString *str, cutilsString *x){
	int err = cutilsStringResize(str, str->len+x->len);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(str+str->len, x, x->len);
	str->len += x->len;

	return CUTILS_OK;
}

int cutilsStringInsertChar(cutilsString *str, char x, size_t index){
	if(index >= str->len){
		return CUTILS_OUT_OF_BOUNDS;
	}

	int err = cutilsStringResize(str, str->len+1);
	if(err != CUTILS_OK){
		return err;
	}

	//move everything at index, till the end of the string including the null termiantor by 1
	memmove(str->str+index+1, str->str+index, str->len+1-index);
	str->str[index] = x;
	str->len++;

	return CUTILS_OK;
}

int cutilsStringInsertCStr(cutilsString *str, const char *x, size_t index){
	if(index >= str->len){
		return CUTILS_OUT_OF_BOUNDS;
	}

	size_t xLen = strlen(x);
	int err = cutilsStringResize(str, str->len+xLen);
	if(err != CUTILS_OK){
		return err;
	}

	memmove(str->str+index+xLen, str->str+index, str->len+1-index);
	memcpy(str->str+index, x, xLen);
	str->len += xLen;

	return CUTILS_OK;
}

int cutilsStringInsertString(cutilsString *str, const cutilsString *x, size_t index){
	if(index >= str->len){
		return CUTILS_OUT_OF_BOUNDS;
	}

	int err = cutilsStringResize(str, str->len+x->len);
	if(err != CUTILS_OK){
		return err;
	}

	memmove(str->str+index+x->len, str->str+index, str->len+1-index);
	memcpy(str->str+index, x->str, x->len);
	str->len += x->len;

	return CUTILS_OK;
}

int cutilsStringSetChar(cutilsString *str, char x){
	int err = cutilsStringResize(str, 1);
	if(err != CUTILS_OK){
		return err;
	}

	str->str[0] = x;

	return CUTILS_OK;
}

int cutilsStringSetCStr(cutilsString *str, const char *x){
	size_t xLen = strlen(x);
	int err = cutilsStringResize(str, xLen);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(str->str, x, xLen);

	return CUTILS_OK;
}

int cutilsStringSetString(cutilsString *str, const cutilsString *x){
	int err = cutilsStringResize(str, x->len);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(str->str, x->str, x->len);

	return CUTILS_OK;
}
