#include "file.h"

void cutilsFileInit(cutilsFile *file){
	file->file = NULL;
	cutilsStringInit(&file->path, 0);
	cutilsStringInit(&file->mode, 0);
	file->read = file->write = file->binary = file->append = file->open = false;
}

cutilsFile* cutilsFileNew(){
	cutilsFile *ret = malloc(sizeof(cutilsFile));
	if(ret == NULL){
		return NULL;
	}
	cutilsFileInit(ret);
	return ret;
}

int cutilsFileOpen(cutilsFile *file, const char *filepath, const char *filemode){
	if(file->open){
		cutilsFileClose(file);
	}

	cutilsStringSet(&file->path, filepath);
	cutilsStringSet(&file->mode, filemode);

	if(strstr(filemode, "+") != NULL){
		file->read = true;
		file->write = true;
	}
	if(strstr(filemode, "w") != NULL){
		file->write = true;
	}
	if(strstr(filemode, "r")){
		file->read = true;
	}
	if(strstr(filemode, "a")){
		file->append = true;
	}
	if(strstr(filemode, "b") != NULL){
		file->binary = true;
	}

	file->file = fopen(filepath, filemode);
	if(file->file == NULL){
		return CUTILS_FOPEN;
	}
	file->open = true;

	struct stat buf;
	stat(file->path.str, &buf);
	file->size = buf.st_size;

	return CUTILS_OK;
}

int cutilsFileCopy(cutilsFile *dst, cutilsFile *src){
	/*
	try to open a new file first so that if dst is open we dont destroy it's only
	to find out that we couldn't open the file anyway
	*/
	cutilsFile tmp;
	int err = cutilsFileOpen(&tmp, src->path.str, src->mode.str);
	if(err != CUTILS_OK){
		return err;
	}

	if(dst->open){
		cutilsFileClose(dst);
	}
	*dst = tmp;

	return CUTILS_OK;
}

void cutilsFileMove(cutilsFile *dst, cutilsFile *src){
	*dst = *src;
	memset(src, 0, sizeof(cutilsFile));
}

void cutilsFileSwap(cutilsFile *a, cutilsFile *b){
	cutilsFile tmp = *a;
	*a = *b;
	*b = tmp;
}

void cutilsFileClose(cutilsFile *file){
	if(!file->open){
		return;
	}

	fclose(file->file);
	file->file = NULL;
	cutilsStringSet(&file->path, "");
	cutilsStringSet(&file->mode, "");

	file->read = file->write = file->binary = file->append = file->open = false;
}

void cutilsFileDeinit(cutilsFile *file){
	if(file->open){
		fclose(file->file);
		file->file = NULL;
	}

	cutilsStringDeinit(&file->path);
	cutilsStringDeinit(&file->mode);

	file->read = file->write = file->binary = file->append = file->open = false;
}

void cutilsFileFree(cutilsFile *file){
	cutilsFileDeinit(file);
	free(file);
}

size_t cutilsFileReadString(cutilsFile *file, cutilsString *string){
	return cutilsFileRead(file, string->str);
}

size_t cutilsFileReadStringSize(cutilsFile *file, cutilsString *string, size_t size){
	return cutilsFileReadSize(file, string->str, size);
}

size_t cutilsFileReadByteStream(cutilsFile *file, cutilsByteStream *stream){
	return cutilsFileRead(file, stream->data);
}

size_t cutilsFileReadByteStreamSize(cutilsFile *file, cutilsByteStream *stream, size_t size){
	return cutilsFileReadSize(file, stream->data, size);
}

size_t cutilsFileRead(cutilsFile *file, void *data){
	return fread(data, 1, file->size, file->file);
}

size_t cutilsFileReadSize(cutilsFile *file, void *data, size_t size){
	return fread(data, 1, size, file->file);
}

size_t cutilsFileWriteString(cutilsFile *file, cutilsString *string){
	return cutilsFileWrite(file, string->str, string->len);
}

size_t cutilsFileWriteStringSize(cutilsFile *file, cutilsString *string, size_t size){
	size = size > string->len?string->len:size;
	return cutilsFileWrite(file, string->str, size);
}

size_t cutilsFileWriteByteStream(cutilsFile *file, cutilsByteStream *stream){
	return cutilsFileWrite(file, stream->data, stream->size);
}

size_t cutilsFileWriteByteStreamSize(cutilsFile *file, cutilsByteStream *stream, size_t size){
	size = size > stream->size?stream->size:size;
	return cutilsFileWrite(file, stream->data, size);
}

size_t cutilsFileWrite(cutilsFile *file, void *data, size_t size){
	return fwrite(data, 1, size, file->file);
}
