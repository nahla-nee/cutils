#ifndef CUTILS_FILE_H
#define CUTILS_FILE_H

#include "errors.h"
#include "str.h"
#include "bytestream.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include <sys/stat.h>

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

void cutilsFileInit(cutilsFile *file);
cutilsFile* cutilsFileNew();
int cutilsFileOpen(cutilsFile *file, const char *filepath, const char *filemode);
int cutilsFileCopy(cutilsFile *dst, cutilsFile *src);
void cutilsFileMove(cutilsFile *dst, cutilsFile *src);
void cutilsFileSwap(cutilsFile *a, cutilsFile *b);
void cutilsFileClose(cutilsFile *file);
void cutilsFileFree(cutilsFile *file);

int cutilsFileRead(cutilsFile *file, cutilsByteStream *stream);
int cutilsFileReadIndex(cutilsFile *file, cutilsByteStream *stream, size_t index);
int cutilsFileReadBytes(cutilsFile *file, cutilsByteStream *stream, size_t bytes);
int cutilsFileReadBytesIndex(cutilsFile *file, cutilsByteStream *stream, size_t bytes, size_t index);

int cutilsFileWrite(cutilsFile *file, cutilsByteStream *stream);
int cutilsFileWriteIndex(cutilsFile *file, cutilsByteStream *stream, size_t index);
int cutilsFileWriteBytes(cutilsFile *file, cutilsByteStream *stream, size_t bytes);
int cutilsFileWriteBytesIndex(cutilsFile *file, cutilsByteStream *stream, size_t bytes, size_t index);

#endif
