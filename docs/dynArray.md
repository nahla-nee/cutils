#dynArray

A way to define a dynamic array struct for any given data type through two macros.
`CUTILS_DEF_DYNARRAY_H` which defines the appropriate header info (struct and function
prototypes), and `CUTILS_DEF_DYNARRAY_C` which define the functions.

##Struct

`data` pointer to the data allocated.

`size` how many elements are in `@data`. This is 0 intially regardless of how much memory is allocated.

`capacity` the maximum amount of elements the array can currently hold.

`usrptr` custom user data passed to callback

`callback` the function to call when an element is about to removed from a list. Works like a destructor.

```c
typedef struct NAME{
	TYPE *data;
	size_t size;
	size_t capacity;
	void *usrptr;
	NAME##RemoveFn callback;
} NAME;
```

>`CUTILS_DEF_DYNARRAY_H(TYPE, NAME)`

This macro defines the dynamic array struct. `TYPE` will be what datatype thes struct
holds, `NAME` is the actual struct name.

>`CUTILS_DEF_DYNARRAY_C(TYPE, NAME, DEFAULT_CALLBACK)`

This macro defines the actual functions to be used with the struct. `TYPE` and `NAME` should be the same as th ones used with `CUTILS_DEF_DYNARRAY_H`. `DEFAULT_CALLBACK` is the default value for the struct's `callback`.

>`typedef void(*NAME##RemoveFn)(TYPE *arr, size_t count, void* usrptr)`

the callback function type. Gives you a pointer the first elemenet being removed along with how many elemnts in total are being removed.

>`int NAME##Init(NAME *arr, size_t size)`

initializes struct struct and allocates `size` amount of members of the struct's data type.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`NAME* NAME##New(size_t size)`

allocates a `NAME` struct and calls `NAME##Init` on it with the given `size`.

This function returns a pointer to a `NAME` struct if no errors occured, or `NULL` if it failed to allocate, or intialize the struct.

>`int NAME##Copy(NAME *dst, NAME *src)`

Resizes `dst` to be the same size as `src`, and copies all data from src to dst.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`void NAME##Move(NAME *dst, NAME *src)`

Sets `dst` to `src` and then zeroes out `src`.

>`void NAME##Swap(NAME *a, NAME *b)`

Swaps struct `a` with struct `b`.

>`void NAME##Deinit(NAME *arr)`

Deintializes the struct pointed to by `arr` by freeing its memory, and zeroing it out.

>`void NAME##Free(NAME *arr)`

Deintializes the struct pointed to by `arr` by freeing its memory, and zeroing it out. And then calls free on `arr`. Use this on structs created using `NAME##New`

>`void NAME##SetUsrptr(NAME *arr, void *data)`

Sets `arr->usrptr` to `data`.

>`void NAME##SetFreeCallback(NAME *arr, NAME##RemoveFn callback)`

Sets `arr->callback` to `callback`.

>`int NAME##Resize(NAME *arr, size_t size)`

Attempts to resize the string to hold `size` members.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##Reserve(NAME *arr, size_t capacity)`

Attempts to reallocate the buffer to have a max capacity of `capacity`.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##PushBack(NAME *arr, TYPE x)`

Attempts to append `x` to the end of the array.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##PushBackPtr(NAME *arr, TYPE *x, size_t len)`

Attempts to append `x` to the end of the array.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##PushBackArr(NAME *arr, NAME *x)`

Attempts to append `x` to the end of the array.

This function returns `CUTILS_OK` if no errors occured, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##Insert(NAME *arr, TYPE x, size_t index)`

Attempts to insert `x` into `arr` at `index`.

This function returns `CUTILS_OK` if no errors occured, `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##InsertPtr(NAME *arr, TYPE *x, size_t len, size_t index)`

Attempts to insert `x` into `arr` at `index`.

This function returns `CUTILS_OK` if no errors occured, `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##InsertArr(NAME *arr, NAME *x, size_t index)`

Attempts to insert `x` into `arr` at `index`.

This function returns `CUTILS_OK` if no errors occured, `CUTILS_OUT_OF_BOUNDS` if `index` was greater than `arr->size`, or `CUTILS_NOMEM` if it failed to allocate the required memory.

>`int NAME##Delete(NAME *arr, size_t index)`

Attempts to remove data at `index` from `arr`.

This function returns `CUTILS_OK` if not errors occured, or `CUTILS_OUT_OF_BOUNDS` if `index` was greater than or equal to `arr->size`.

>`int NAME##DeleteRange(NAME *arr, size_t start, size_t end)`

Attempts to remove all data between `start`, and `end` from `arr`.

This function returns `CUTILS_OK` if not errors occured, or `CUTILS_OUT_OF_BOUNDS` if `index` was greater than or equal to `arr->size`.
