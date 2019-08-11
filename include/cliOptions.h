#ifndef CUTILS_CLIOPTIONS_H
#define CUTILS_CLIOPTIONS_H

#include "errors.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

typedef struct cutilsCliOption{
	//input
	char *shortFlag;
	char *longFlag;

	//output
	int index;
} cutilsCliOption;

bool cutilsCliOptionFind(cutilsCliOption *option, int argc, char **argv);
int cutilsCliOptionFindFlag(char *flag, int argc, char **argv);

#endif
