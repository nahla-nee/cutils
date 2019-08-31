#ifndef CUTILS_LOGGER_H
#define CUTILS_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum cutilsLogLevel{
	CUTILS_INFO,
	CUTILS_WARNING,
	CUTILS_ERROR,
	CUTILS_FATAL
} cutilsLogLevel;

typedef struct cutilsLogger{
	FILE *info;
	FILE *warning;
	FILE *error;
	FILE *fatal;
	enum cutilsLogLevel logLevel;
} cutilsLogger;

void cutilsLoggerInit(cutilsLogger *logger, cutilsLogLevel level);
cutilsLogger* cutilsLoggerNew(cutilsLogLevel level);
void cutilsLoggerDeinit(cutilsLogger *logger);
void cutilsLoggerFree(cutilsLogger *logger);

void cutilsLoggerLog(cutilsLogger *logger, const char *msg, cutilsLogLevel level);

#endif
