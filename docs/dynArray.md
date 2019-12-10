# dynArray

A set of macros to define a dynamic array struct for any given data type. It is
made up of two macros: `CUTILS_DEF_DYNARRAY_H`, and `CUTILS_DEF_DYNARRAY_C`.

## Struct

`data` pointer to the data allocated.

`size` how many elements are in `@data`. This is 0 intially regardless of how
much memory is allocated.

`capacity` the maximum amount of elements the array can currently hold.

`usrptr` custom user data passed to callback

`callback` the function to call when an element is about to removed from a list.
Works like a destructor.

```c
typedef struct NAME{
	TYPE *data;
	size_t size;
	size_t capacity;
	void *usrptr;
	NAME##RemoveFn callback;
} NAME;
```

## Functions/Macros

>`CUTILS_DEF_DYNARRAY_H(TYPE, NAME)`

This macro defines the dynamic array struct, and the needed function prototypes.
`TYPE` will be what datatype the struct holds, `NAME` is the actual struct name.

note: the `TYPE` and `NAME` passed to this macro must be the same as the ones
passed to `CUTILS_DEF_DYNARRAY_C`.

>`CUTILS_DEF_DYNARRAY_C(TYPE, NAME, DEFAULT_CALLBACK)`

This macro defines the functions to be used with the struct.

note: the `TYPE` and `NAME` passed to this macro must be the same as the ones
passed to `CUTILS_DEF_DYNARRAY_H`.

>`typedef void(*NAME##RemoveFn)(TYPE *arr, size_t count, void* usrptr)`

the callback function type. Gives you a pointer the first elemenet being removed
along with how many elemnts in total are being removed.

>`int NAME##Init(NAME *arr, size_t size)`

initializes struct struct and allocates `size` amount of members of the struct's
data type.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`NAME* NAME##New(size_t size)`

allocates a `NAME` struct and calls `NAME##Init` on it with the given `size`.

return value:

* A pointer to a `NAME` struct.
* `NULL` if the function failed to allocate or initialize the struct.

>`int NAME##Copy(NAME *dst, NAME *src)`

Resizes `dst` to be the same size as `src`, and copies all data from src to dst.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`void NAME##Move(NAME *dst, NAME *src)`

Sets `dst` to `src` and then zeroes out `src`.

>`void NAME##Swap(NAME *a, NAME *b)`

Swaps struct `a` with struct `b`.

>`void NAME##Deinit(NAME *arr)`

Deintializes the struct pointed to by `arr` by freeing its memory, and zeroing
it out.

>`void NAME##Free(NAME *arr)`

Deintializes the struct pointed to by `arr` by freeing its memory, and zeroing
it out. And then calls free on `arr`. Use this on structs created using `NAME##New`.

>`void NAME##SetUsrptr(NAME *arr, void *data)`

Sets `arr->usrptr` to `data`.

>`void NAME##SetFreeCallback(NAME *arr, NAME##RemoveFn callback)`

Sets `arr->callback` to `callback`.

>`int NAME##Resize(NAME *arr, size_t size)`

Attempts to resize the string to hold `size` members.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##Reserve(NAME *arr, size_t capacity)`

Attempts to reallocate the buffer to have a max capacity of `capacity`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##PushBack(NAME *arr, TYPE x)`

Attempts to append `x` to the end of the array.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##PushBackPtr(NAME *arr, TYPE *x, size_t len)`

Attempts to append `x` to the end of the array.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##PushBackArr(NAME *arr, NAME *x)`

Attempts to append `x` to the end of the array.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##Insert(NAME *arr, TYPE x, size_t index)`

Attempts to insert `x` into `arr` at `index`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##InsertPtr(NAME *arr, TYPE *x, size_t len, size_t index)`

Attempts to insert `x` into `arr` at `index`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##InsertArr(NAME *arr, NAME *x, size_t index)`

Attempts to insert `x` into `arr` at `index`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`.
* `CUTILS_NOMEM` if the function failed to allocate the required memory.

>`int NAME##Delete(NAME *arr, size_t index)`

Attempts to remove data at `index` from `arr`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`.

>`int NAME##DeleteRange(NAME *arr, size_t start, size_t end)`

Attempts to remove all data between `start`, and `end` from `arr`.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_OUT_OF_BOUNDS` if either `start` or `end` is greater than `arr->size`.
