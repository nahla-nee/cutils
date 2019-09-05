#include "cliOptions.h"

void cutilsCliOptionFindArr(cutilsCliOption *options, size_t size, int argc, char **argv){
	for(size_t i = 0; i < size; i++){
		options[i].index = -1;
	}

	for(int i = 1; i < argc; i++){
		for(size_t j = 0; j < size; j++){
			if(options[j].shortFlag != NULL){
				if(strcmp(argv[i], options[j].shortFlag) == 0){
					options[j].index = i;
					break;
				}
			}
			if(options[j].longFlag != NULL){
				if(strcmp(argv[i], options[j].longFlag) == 0){
					options[j].index = i;
					break;
				}
			}
		}
	}
}

bool cutilsCliOptionFind(cutilsCliOption *option, int argc, char **argv){
	for(int i = 1; i < argc; i++){
		if(option->shortFlag != NULL){
			if(strcmp(argv[i], option->shortFlag) == 0){
				option->index = i;
				break;
			}
		}
		if(option->longFlag != NULL){
			if(strcmp(argv[i], option->longFlag) == 0){
				option->index = i;
				break;
			}
		}
	}
	option->index = -1;
	return false;
}

int cutilsCliOptionFindFlag(char *flag, int argc, char **argv){
	int index = -1;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], flag) == 0){
			index = i;
		}
	}
	return index;
}

int cutilsCliOptionFindFlags(char *shortFlag, char *longFlag, int argc, char **argv){
	int index = -1;
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], shortFlag) == 0 || strcmp(argv[i], longFlag) == 0){
			index = i;
		}
	}
	return index;
}
