# String

a struct to handle dynamic string management. Every so slightly different from
dynArray because it's null terminated.

This struct defines a dynArray of itself called `cutilsStringArr` along with its
own default callback function.

## Struct

`str` Null terminated char array.

`len` Length of `str`, not counting the null terminator.

`capacity` The max amount of characters the string can hold at the moment.

`lastError` The last error that occured during cutils function calls. This is a read only variable.

Note: The total amount of memory allocated is always `capacity+1`.

Note: The resize functions will only ever call `realloc` if they need to increase
the size of the buffer, if you want to free up some of the allocated memory use
`cutilsStringReserve`

```C
typedef struct cutilsString{
	char *str;
	size_t len;
	size_t capacity;

	int lastError;
} cutilsString;
```

## Functions

>`int cutilsStringInit(cutilsString *str, size_t capacity)`

Initializes a string to be empty, and have a capacity of `capacity`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringCopy(cutilsString *dst, cutilsString *src)`

Resizes `dst` to be the same size as `src` and copies the data from `src` into `dst`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`void cutilsStringMove(cutilsString *dst, cutilsString *src)`

Sets `dst` to `src` and zeroes out `src`.

>`void cutilsStringSwap(cutilsString *a, cutilsString *b)`

Swaps struct `a` with struct `b`.

>`void cutilsStringDeinit(cutilsString *str)`

Frees memory allocated by `str`, and zeroes out the struct.

>`cutilsString* cutilsStringNew(size_t capacity)`

Allocates a `cutilsString` struct, and calls `cutilsStringInit` with the given
`capacity`.

return value:

* A pointer to a `cutilsString` struct.
* `NULL` if the function failed to allocate or initialize the struct.

>`void cutilsStringFree(cutilsString *str)`

Free memory alloacted by `str`, zeroes out the struct, then frees `str`. Use this
function with strings allocated by `cutilsStringNew`.

>`int cutilsStringResize(cutilsString *str, size_t len)`

Attempts to resize the string to hold `len` characters. If `len` is greater than
`str->len` the new memory is uninitialized, if you want to initialize it use
`cutilsStringResizeRepeat`

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if this function failed to allocate the required memory.

>`int cutilsStringResizeRepeat(cutilsString *str, size_t len, char c)`

Attempts to resize the string to hold `len` characters. If `len` is larger than
`str->len` all new characters will be initialized to `c`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringReserve(cutilsString *str, size_t capcity)`

Attempts to reallocate the buffer to have a max capacity of `capacity`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringAppendChar(cutilsString *str, char x)`

Attempts to append `x` to the end of the string.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringAppendCStr(cutilsString *str, const char *x)`

Attempts to append `x` to the end of the string.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringAppendString(cutilsString *str, cutilsString *x)`

Attempts to append `x` to the end of the string.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringInsertChar(cutilsString *str, char x, size_t index)`

Attempts to insert `x` into `str` at `str->str+index`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` is greater than `str->len`
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringInsertCStr(cutilsString *str, const char *x, size_t index)`

Attempts to insert `x` into `str` at `str->str+index`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` is greater than `str->len`
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringInsertString(cutilsString *str, const cutilsString *x, size_t index)`

Attempts to insert `x` into `index` at `str->str+index`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` is greater than `str->len`
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringSetChar(cutilsString *str, char x)`

Attempts to set `str` to hold a single char `x`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringSetCStr(cutilsString *str, const char *x)`

Attempts to set `str` to hold the string `x`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringSetString(cutilsString *str, const cutilsString *x)`

Attempts to set `str` to hold the string `x`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int cutilsStringDelete(cutilsString *str, size_t index)`

Attempts to remove the char at `str->str+index` from `str`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` is greater than or equal to `str->len`.

>`int cutilsStringDeleteRange(cutilsString *str, size_t start, size_t end)`

Attempts to remove all chars from `str->str+start` to `str->str+end` from `str`.

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if either `start` or `end` is greater than or equal to `str->len`.

>`cutilsStringAppend(STR, X)`

A macro that determines the type of `X`, and calls the appropriate append function.

This macro returns the value of the function it calls.

>`cutilsStringInsert(STR, X, INDEX)`

A macro that determines the type of `X`, and calls the appropriate insert function.

This macro returns the value of the function it calls.

>`cutilsStringSet(STR, X)`

Determines the type of `X`, and calls the appropriate set function.

This macro returns the value of the function it calls.
