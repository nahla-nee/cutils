#include "str.h"

int cutilsStringInit(cutilsString *str, size_t capacity){
	if(capacity == 0){
		memset(str, 0, sizeof(cutilsString));
		return CUTILS_OK;
	}

	//allocate one more byte for null terminator
	str->str = calloc(capacity+1, 1);
	if(str->str == NULL){
		return CUTILS_NOMEM;
	}
	str->len = 0;
	str->capacity = capacity;
	return CUTILS_OK;
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
	return cutilsStringResizeChar(str, len, str->str[str->len-1]);
}

int cutilsStringResizeChar(cutilsString *str, size_t len, char c){
	if(len >= str->capacity){
		//allocate memory for new len AND the null terminator
		char *tmp = realloc(str->str, len+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
	}

	if(len > str->len){
		//repeat the last character of the string to fill up the space
		memset(str->str+str->len, c, len-str->len);
	}
	str->str[len] = '\0';
	str->len = len;
	str->capacity = len;

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
	if(str->len+1 >= str->capacity){
		char *tmp = realloc(str->str, str->len+2);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = str->len+2;
	}

	str->str[str->len] = x;
	str->str[str->len+1] = '\0';
	str->len++;

	return CUTILS_OK;
}

int cutilsStringAppendCStr(cutilsString *str, const char *x){
	size_t xLen = strlen(x);
	if(str->len+xLen >= str->capacity){
		char *tmp = realloc(str->str, str->len+xLen+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = str->len+xLen+1;
	}

	memcpy(str+str->len, x, xLen);
	str->len += xLen;
	str->str[str->len] = '\0';

	return CUTILS_OK;
}

int cutilsStringAppendString(cutilsString *str, cutilsString *x){
	if(str->len+x->len >= str->capacity){
		char *tmp = realloc(str->str, str->len+x->len+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = str->len+x->len+1;
	}

	memcpy(str+str->len, x, x->len);
	str->len += x->len;
	str->str[str->len] = '\0';

	return CUTILS_OK;
}

int cutilsStringInsertChar(cutilsString *str, char x, size_t index){
	if(index >= str->len){
		return CUTILS_OUT_OF_BOUNDS;
	}

	if(str->len+1 >= str->capacity){
		char *tmp = realloc(str->str, str->len+2);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = str->len+2;
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

	if(str->len+xLen >= str->capacity){
		char *tmp = realloc(str->str, str->len+xLen+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = str->len+xLen+1;
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

	if(str->len+x->len >= str->capacity){
		char *tmp = realloc(str->str, str->len+x->len+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = str->len+x->len+1;
	}

	memmove(str->str+index+x->len, str->str+index, str->len+1-index);
	memcpy(str->str+index, x->str, x->len);
	str->len += x->len;

	return CUTILS_OK;
}

int cutilsStringSetChar(cutilsString *str, char x){
	//2 to account for null temrinator
	if(2 >= str->capacity){
		char *tmp = realloc(str->str, 2);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = 2;
	}

	str->str[0] = x;
	str->str[1] = '\0';
	str->len = 1;

	return CUTILS_OK;
}

int cutilsStringSetCStr(cutilsString *str, const char *x){
	size_t xLen = strlen(x);
	if(xLen >= str->capacity){
		char *tmp = realloc(str->str, xLen+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = xLen+1;
	}

	memcpy(str->str, x, xLen);
	str->str[xLen] = '\0';
	str->len = xLen;

	return CUTILS_OK;
}

int cutilsStringSetString(cutilsString *str, const cutilsString *x){
	if(x->len >= str->capacity){
		char *tmp = realloc(str->str, x->len+1);
		if(tmp == NULL){
			return CUTILS_NOMEM;
		}
		str->str = tmp;
		str->capacity = x->len+1;
	}

	memcpy(str->str, x->str, x->len);
	str->str[x->len] = '\0';
	str->len = x->len;

	return CUTILS_OK;
}
