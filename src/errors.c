#include "errors.h"

const char* cutilsErrorToStr(int code){
	switch(code){
		case CUTILS_OK:
			return "No error.";
		case CUTILS_NOMEM:
			return "Failed to allocate memory.";
		case CUTILS_OUT_OF_BOUNDS:
			return "Out of bounds access.";
		case CUTILS_FOPEN:
			return "Failed to open file.";
		default:
			return "Unkown error code.";
	}
}
