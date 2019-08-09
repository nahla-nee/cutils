#include "cliOptions.h"

bool cutilsCliOptionFind(cutilsCliOption *option, int argc, char **argv){
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], option->shortFlag) == 0 || strcmp(argv[i], option->longFlag) == 0){
			option->index = i;
			return true;
		}
	}
	return false;
}

int cutilsCliOptionFindFlag(char *flag, int argc, char **argv){
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], flag) == 0){
			return i;
		}
	}
	return -1;
}
