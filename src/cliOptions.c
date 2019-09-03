#include "cliOptions.h"

void cutilsCliOptionFindArr(cutilsCliOption *option, size_t size, int argc, char **argv){
	for(size_t i = 0; i < size; i++){
		option[i].index = -1;
	}

	for(int i = 1; i < argc; i++){
		for(size_t j = 0; j < size; j++){
			if(strcmp(argv[i], option[j].shortFlag) == 0 || strcmp(argv[i], option[j].longFlag) == 0){
				option->index = i;
				break;
			}
		}
	}
}

bool cutilsCliOptionFind(cutilsCliOption *option, int argc, char **argv){
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], option->shortFlag) == 0 || strcmp(argv[i], option->longFlag) == 0){
			option->index = i;
			return true;
		}
	}
	option->index = -1;
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

int cutilsCliOptionFindFlags(char *shortFlag, char *longFlag, int argc, char **argv){
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], shortFlag) == 0 || strcmp(argv[i], longFlag) == 0){
			return i;
		}
	}
	return -1;
}
