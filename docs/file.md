# file

A file struct to aid in reading and writing from a file.

## Struct

`file` The actual file pointer used for i/o.

`path` Path to the file.

`mode` The mode the file was opened with.

`size` The total size of the file in bytes.

`read` If reading is allowed.

`write` If writing is allowed.

`binary` If the file was opened in binary mode.

`append` If the file was opened in append mode.

`open` if the file is currently open.

```c
typedef struct cutilsFile{
	FILE *file;
	cutilsString path;
	cutilsString mode;

	size_t size;

	bool read;
	bool write;
	bool binary;
	bool append;
	bool open;
} cutilsFile;
```

## Functions

>`void cutilsFileInit(cutilsFile *file)`

Initializes the file structure.

>`cutilsFile* cutilsFileNew()`

Allocates a new `cutilsFile` struct, and intializes it.

return value:

* A pointer to a `cutilsfile` struct.
* `NULL` if  the function failed to allocate the required memory for the struct.

>`int cutilsFileOpen(cutilsFile *file, const char *filepath, const char *filemode)`

Opens the file at `filepath` with `filemode`. If `file` points to an already open
file struct, it will be closed.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_FOPEN` if the function failed to to open the file.

>`int cutilsFileCopy(cutilsFile *dst, cutilsFile *src)`

Opens the file a copy of the file that `src` points to, with `src->mode`. `dst`
points to an open file it will be closed.

return value:

* `CUTILS_OK` if no errors occured.
* `CUTILS_FOPEN` if it failed to open the file.

note: In case of an error occuring the original values of `dst` are presevred.

>`void cutilsFileMove(cutilsFile *dst, cutilsFile *src)`

Copies the `src` struct into `dst`, and zeroes out `src.`

>`void cutilsFileSwap(cutilsFile *a, cutilsFile *b)`

Swaps the values of struct `a` with struct `b`.

>`void cutilsFileClose(cutilsFile *file)`

Closes the file.

>`void cutilsFileDeinit(cutilsFile *file)`

Deinitalizes the file struct and frees any memory it allocated.

>`void cutilsFileFree(cutilsFile *file)`

Deinitalizes the file struct and frees any memory it allocated, also calls free
on `file`. Use this with structs that have been allocate with `cutilsFileNew`.

>`size_t cutilsFileReadString(cutilsFile *file, cutilsString *string)`

Reads from the current position indicator of the file till the end of the file
into string.

return value:

Number of bytes read into `string`.

>`size_t cutilsFileReadStringSize(cutilsFile *file, cutilsString *string, size_t size)`

Reads `size` bytes from the current position indicator of the file into `string`.

return value:

Number of bytes read into `string`.

>`size_t cutilsFileReadByteStream(cutilsFile *file, cutilsByteStream *stream)`

Reads from the current position indicator of the file till the end of the file
into `stream`.

return value:

Number of bytes read into `stream`.

>`size_t cutilsFileReadByteStreamSize(cutilsFile *file, cutilsByteStream *stream, size_t size)`

Reads `size` bytes from the current position indicator of the file into `stream`.

return value:

Number of bytes read into `stream`.

>`size_t cutilsFileRead(cutilsFile *file, void *data)`

Reads from the current position indicator of the file till the end of the file
into `data`.

return value:

Number of bytes read into `data`.

>`size_t cutilsFileReadSize(cutilsFile *file, void *data, size_t size)`

Reads `size` bytes from the current position indicator of the file into `data`.

return value:

Number of bytes read into `data`.

>`size_t cutilsFileWriteString(cutilsFile *file, cutilsString *string)`

Writes all of the data in `string` to the file, not including the null terminator.

return value:

Number of bytes written to `file`.

>`size_t cutilsFileWriteStringSize(cutilsFile *file, cutilsString *string, size_t size)`

Writes `size` bytes from `string` into the file, if `size` is greater than
`string->len` it will only write `string->len` bytes.

return value:

Number of bytes return to `file`.

>`size_t cutilsFileWriteByteStream(cutilsFile *file, cutilsByteStream *stream)`

Writes all of the data in `stream` to the file.

return value:

Number of bytes written to `file`.

>`size_t cutilsFileWriteByteStreamSize(cutilsFile *file, cutilsByteStream *stream, size_t size)`

Writes `size` bytes from `stream` into the file, if `size` is greater than
`stream->size` it will only write `stream->size` bytes.

return value:

Number of bytes written to `file`.

>`size_t cutilsFileWrite(cutilsFile *file, void *data, size_t size)`

Writes `size` bytes from `data` into the file.

return value:

Number of bytes written to `file`.
