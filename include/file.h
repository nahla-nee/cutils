#ifndef CUTILS_FILE_H
#define CUTILS_FILE_H

#include "errors.h"
#include "str.h"

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
void cutilsFileDeinit(cutilsFile *file);
void cutilsFileFree(cutilsFile *file);

size_t cutilsFileReadString(cutilsFile *file, cutilsString *string);
size_t cutilsFileReadStringSize(cutilsFile *file, cutilsString *string, size_t size);

size_t cutilsFileRead(cutilsFile *file, void *data);
size_t cutilsFileReadSize(cutilsFile *file, void *data, size_t size);

size_t cutilsFileWriteString(cutilsFile *file, cutilsString *string);
size_t cutilsFileWriteStringSize(cutilsFile *file, cutilsString *string, size_t size);

size_t cutilsFileWrite(cutilsFile *file, void *data, size_t size);

#endif
