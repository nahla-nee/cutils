#include "str.h"
#include "errors.h"

int cutilsStringInit(cutilsString *str, size_t capacity){
	//allocate one more byte for null terminator
	str->str = malloc(capacity+1);
	if(str->str == NULL){
		str->lastError = CUTILS_NOMEM;
		return CUTILS_NOMEM;
	}

	str->str[0] = '\0';
	str->len = 0;
	str->capacity = capacity;
	str->lastError = CUTILS_OK;

	return CUTILS_OK;
}

int cutilsStringCopy(cutilsString *dst, cutilsString *src){
	//lastError will best set by resize if an error does happen
	int err = cutilsStringResize(dst, src->len);
	if(err != CUTILS_OK){
		return err;
	}
	memcpy(dst->str, src->str, src->len);

	dst->lastError = CUTILS_OK;
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

void cutilsStringDeinit(cutilsString *str){
	free(str->str);
	memset(str, 0, sizeof(cutilsString));
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

void cutilsStringFree(cutilsString *str){
	cutilsStringDeinit(str);
	free(str);
}

int cutilsStringResize(cutilsString *str, size_t len){
	if(len > str->capacity){
		char *tmp = realloc(str->str, len+1);
		if(tmp == NULL){
			str->lastError = CUTILS_NOMEM;
			return CUTILS_NOMEM;
		}

		str->str = tmp;
		str->capacity = len;
	}

	str->len = len;
	str->str[len] = '\0';

	str->lastError = CUTILS_OK;
	return CUTILS_OK;
}

int cutilsStringResizeRepeat(cutilsString *str, size_t len, char c){
	/*cutilsStringResize already set lastError to whatever the appropriate value
	is so we don't need to touch it*/

	size_t oldLen = str->len;
	int err = cutilsStringResize(str, len);
	if(err != CUTILS_OK){
		return err;
	}

	if(len > str->len){
		//repeat the last character of the string to fill up the space
		memset(str->str+oldLen, c, str->len-oldLen);
	}

	return CUTILS_OK;
}

int cutilsStringReserve(cutilsString *str, size_t capacity){
	char *tmp = realloc(str->str, capacity+1);
	if(tmp == NULL){
		return CUTILS_NOMEM;
	}

	str->str = tmp;
	str->capacity = capacity;
	if(str->len > capacity){
		str->len = capacity;
		str->str[capacity] = '\0';
	}

	str->lastError = CUTILS_OK;
	return CUTILS_OK;
}

/*cutilsStringResize will already set lastError to whatever the appropriate value
is for all the following append, insert and set functions so we don't need to
touch it*/
int cutilsStringAppendChar(cutilsString *str, char x){
	int err = cutilsStringResize(str, str->len+1);
	if(err != CUTILS_OK){
		return err;
	}

	str->str[str->len-1] = x;

	return CUTILS_OK;
}

int cutilsStringAppendCStr(cutilsString *str, const char *x){
	size_t xLen = strlen(x);
	size_t oldLen = str->len;

	int err = cutilsStringResize(str, str->len+xLen);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(str->str+oldLen, x, xLen);

	return CUTILS_OK;
}

int cutilsStringAppendString(cutilsString *str, cutilsString *x){
	size_t oldLen = str->len;

	int err = cutilsStringResize(str, str->len+x->len);
	if(err != CUTILS_OK){
		return err;
	}

	memcpy(str->str+oldLen, x, x->len);

	return CUTILS_OK;
}

int cutilsStringInsertChar(cutilsString *str, char x, size_t index){
	if(index > str->len){
		str->lastError = CUTILS_OUT_OF_BOUNDS;
		return CUTILS_OUT_OF_BOUNDS;
	}

	int err = cutilsStringResize(str, str->len+1);
	if(err != CUTILS_OK){
		return err;
	}

	//move everything at index, till the end of the string including the null termiantor by 1
	memmove(str->str+index+1, str->str+index, str->len+1-index);
	str->str[index] = x;

	return CUTILS_OK;
}

int cutilsStringInsertCStr(cutilsString *str, const char *x, size_t index){
	if(index > str->len){
		str->lastError = CUTILS_OUT_OF_BOUNDS;
		return CUTILS_OUT_OF_BOUNDS;
	}

	size_t xLen = strlen(x);

	int err = cutilsStringResize(str, str->len+xLen);
	if(err != CUTILS_OK){
		return err;
	}

	memmove(str->str+index+xLen, str->str+index, str->len+1-index);
	memcpy(str->str+index, x, xLen);

	return CUTILS_OK;
}

int cutilsStringInsertString(cutilsString *str, const cutilsString *x, size_t index){
	if(index > str->len){
		str->lastError = CUTILS_OUT_OF_BOUNDS;
		return CUTILS_OUT_OF_BOUNDS;
	}

	int err = cutilsStringResize(str, str->len+x->len);
	if(err != CUTILS_OK){
		return err;
	}

	memmove(str->str+index+x->len, str->str+index, str->len+1-index);
	memcpy(str->str+index, x->str, x->len);

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
	return cutilsStringCopy(str, x);
}

int cutilsStringDelete(cutilsString *str, size_t index){
	if(index >= str->len){
		str->lastError = CUTILS_OUT_OF_BOUNDS;
		return CUTILS_OUT_OF_BOUNDS;
	}

	memmove(str->str+index, str->str+index+1, str->len+1-index);
	str->len--;

	return CUTILS_OK;
}

int cutilsStringDeleteRange(cutilsString *str, size_t start, size_t end){
	if(start >= str->len || end >= str->len){
		str->lastError = CUTILS_OUT_OF_BOUNDS;
		return CUTILS_OUT_OF_BOUNDS;
	}

	if(start == 0 && end == str->len){
		cutilsStringResize(str, 0);
		return CUTILS_OK;
	}

	memmove(str->str+start, str->str+end+1, str->len-end);
	//dont need to check return, shrinks always work
	cutilsStringResize(str, str->len-(end-start)-1);

	return CUTILS_OK;
}
