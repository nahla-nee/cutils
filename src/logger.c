#include "logger.h"

void cutilsLoggerInit(cutilsLogger *logger, cutilsLogLevel level){
	logger->logLevel = level;
	logger->info = stdout;
	logger->warning = stdout;
	logger->error = stderr;
	logger->fatal = stderr;
}

cutilsLogger* cutilsLoggerNew(cutilsLogLevel level){
	cutilsLogger *ret = malloc(sizeof(cutilsLogger));
	if(ret == NULL){
		return NULL;
	}
	cutilsLoggerInit(ret, level);
	return ret;
}

void cutilsLoggerDeinit(cutilsLogger *logger){
	memset(logger, 0, sizeof(cutilsLogger));
}

void cutilsLoggerFree(cutilsLogger *logger){
	cutilsLoggerDeinit(logger);
	free(logger);
}

void cutilsLoggerLog(cutilsLogger *logger, const char *msg, cutilsLogLevel level){
	if(level >= logger->logLevel){
		fprintf(logger->info, "%s", msg);
		fflush(logger->info);
	}
	if(level >= logger->logLevel){
		fprintf(logger->warning, "%s", msg);
		fflush(logger->warning);
	}
	if(level >= logger->logLevel){
		fprintf(logger->error, "%s", msg);
		fflush(logger->error);
	}
	if(level >= logger->logLevel){
		fprintf(logger->fatal, "%s", msg);
		fflush(logger->fatal);
	}
}
