# String

a struct to handle dynamic string management. Every so slightly different from dynArray because it's null terminated.

This struct defines a dynArray of itself called `cutilsStringArr` along with its own default callback function.

## Struct

`str` Null terminated char array.

`len` Length of `str`, not counting the null terminator.

`capacity` The max amount of characters the string can hold at the moment.

Note: The total amount of memory allocated is always `capacity+1`

```C
typedef struct cutilsString{
	char *str;
	size_t len;
	size_t capacity;
} cutilsString;
```

## Functions

>`int cutilsStringInit(cutilsString *str, size_t capacity)`

Initializes a string to be empty, and have a capacity of `capacity`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the memory required.

>`cutilsString* cutilsStringNew(size_t capacity)`

Allocates a `cutilsString` struct, and calls `cutilsStringInit` with the given `capacity`

This function returns a pointer to a `cutilsString` struct if no errors occurred, or `NULL` if it failed to allocate, or intialize the struct.

>`int cutilsStringCopy(cutilsString *dst, cutilsString *src)`

Resizes `dst` to be the same size as `src` and copies the data from `src` into `dst`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the memory required.

>`void cutilsStringMove(cutilsString *dst, cutilsString *src)`

Sets `dst` to `src` and zeroes out `src`.

>`void cutilsStringSwap(cutilsString *a, cutilsString *b)`

Swaps struct `a` with struct `b`.

>`void cutilsStringDeinit(cutilsString *str)`

Frees memory allocated by `str`, and zeroes out the struct.

>`void cutilsStringFree(cutilsString *str)`

Free memory alloacted by `str`, zeroes out the struct, then frees `str`. Use this function with strings allocated by `cutilsStringNew`.

>`int cutilsStringResize(cutilsString *str, size_t len)`

Attempts to resize the string to hold `len` characters.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringResizeRepeat(cutilsString *str, size_t len, char c)`

Attempts to resize the string to hold `len` characters. If `len` is larger than `str->len` all new characters will be initialized to `c`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringReserve(cutilsString *str, size_t capcity)`

Attempts to reallocate the buffer to have a max capacity of `capacity`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringAppendChar(cutilsString *str, char x)`

Attempts to append `x` to the end of the string.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringAppendCStr(cutilsString *str, const char *x)`

Attempts to append `x` to the end of the string.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringAppendString(cutilsString *str, cutilsString *x)`

Attempts to append `x` to the end of the string.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringInsertChar(cutilsString *str, char x, size_t index)`

Attempts to insert `x` into `index` at `index`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringInsertCStr(cutilsString *str, const char *x, size_t index)`

Attempts to insert `x` into `index` at `index`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringInsertString(cutilsString *str, const cutilsString *x, size_t index)`

Attempts to insert `x` into `index` at `index`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringSetChar(cutilsString *str, char x)`

Attempts to set `str` to `x`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringSetCStr(cutilsString *str, const char *x)`

Attempts to set `str` to `x`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringSetString(cutilsString *str, const cutilsString *x)`

Attempts to set `str` to `x`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int cutilsStringDelete(cutilsString *str, size_t index)`

Attempts to remove the char at `index` from `str`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_OUT_OF_BOUNDS` if `index` is greater than `str->len`.

>`int cutilsStringDeleteRange(cutilsString *str, size_t start, size_t end)`

Attempts to remove all chars from `start` to `end` from `str`.

This function returns `CUTILS_OK` if no errors occurred, or `CUTILS_OUT_OF_BOUNDS` if `index` is greater than `str->len`.

>`cutilsStringAppend(STR, X)`

A macro that determines the type of `X`, and calls the appropriate append function.

This macro returns the value of the function it calls.

>`cutilsStringInsert(STR, X, INDEX)`

A macro that determines the type of `X`, and calls the appropriate insert function.

This macro returns the value of the function it calls.

>`cutilsStringSet(STR, X)`

Determines the type of `X`, and calls the appropriate set function.

This macro returns the value of the function it calls.
