#include "file.h"

void cutilsFileInit(cutilsFile *file){
	file->file = NULL;
	cutilsStringInit(file->path, 0);
	cutilsStringInit(file->mode, 0);
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
	int err =cutilsFileOpen(&tmp, src->path.str, src->mode.str);
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
	fclose(file->file);
	file->file = NULL;
	cutilsStringSet(&file->path, "");
	cutilsStringSet(&file->mode, "");
}

void cutilsFileFree(cutilsFile *file){
	fclose(file->file);
	file->file = NULL;
	cutilsStringFree(&file->path);
	cutilsStringFree(&file->mode);
}

int cutilsFileRead(cutilsFile *file, cutilsByteStream *stream){
	return cutilsFileReadBytesIndex(file, stream, file->size, 0);
}

int cutilsFileReadIndex(cutilsFile *file, cutilsByteStream *stream, size_t index){
	return cutilsFileReadBytesIndex(file, stream, file->size, index);
}

int cutilsFileReadBytes(cutilsFile *file, cutilsByteStream *stream, size_t bytes){
	return cutilsFileReadBytesIndex(file, stream, bytes, 0);
}

int cutilsFileReadBytesIndex(cutilsFile *file, cutilsByteStream *stream, size_t bytes, size_t index){
	if(index > stream->size){
		return CUTILS_OUT_OF_BOUNDS;
	}

	if(bytes > (stream->size-index)){
		int err = cutilsByteStreamResize(stream, bytes+stream->size);
		if(err != CUTILS_OK){
			return err;
		}
	}
	memmove(stream->data+index+bytes, stream->data+index, bytes);

	size_t bytesRead = fread(stream->data+index, 1, bytes, file->file);
	if(bytesRead != bytes){
		return CUTILS_BAD_READ;
	}

	return CUTILS_OK;
}

int cutilsFileWrite(cutilsFile *file, cutilsByteStream *stream){
	return cutilsFileWriteBytesIndex(file, stream, stream->size, 0);
}

int cutilsFileWriteIndex(cutilsFile *file, cutilsByteStream *stream, size_t index){
	return cutilsFileWriteBytesIndex(file, stream, stream->size, index);
}

int cutilsFileWriteBytes(cutilsFile *file, cutilsByteStream *stream, size_t bytes){
	return cutilsFileWriteBytesIndex(file, stream, bytes, 0);
}

int cutilsFileWriteBytesIndex(cutilsFile *file, cutilsByteStream *stream, size_t bytes, size_t index){
	size_t bytesWritten = fwrite(stream->data, 1, bytes, file->file);
	if(bytesWritten != bytes){
		return CUTILS_BAD_WRITE;
	}

	return CUTILS_OK;
}
