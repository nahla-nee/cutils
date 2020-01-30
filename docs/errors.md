# errors

A set of macros that define all errors that can be returned by this libraries
functions, as well as a function to convert each code to a string.

## Codes

* `CUTILS_OK`: no error.
* `CUTILS_NOMEM`: failed to allocate memory.
* `CUTILS_OUT_OF_BOUNDS`: out of bound access.
* `CUTILS_FOPEN`: failed to open file.
* `CUTILS_REALLOC_SHRINK`: failed to shrink memory using realloc, performing fake resize

## Functions

> `const char* cutilsErrorToStr(int code)`

returns an error message that matches `code`.
