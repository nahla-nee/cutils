#cliOptions

A struct, with helper functions to quickly, and easily identify what cli options have been passed to the program.

##Struct

`shortFlag` The short version of the cli option to test for. If set to `NULL` it will be ignored.

`longFlag` the long version of the cli option to test for. If set to `NULL` it will be ignored.

`index` Index into argv that matches the last occurance of the flag, otherwise it will be -1.

```c
typedef struct cutilsCliOption{
	char *shortFlag;
	char *longFlag;

	int index;
} cutilsCliOption;
```

>`void cutilsCliOptionFindArr(cutilsCliOption *options, size_t size, int argc, char **argv)`

Searches if any options in the `options` array match any arguments in `argv` starting at `argv[1]`, and sets `index` for the structs if the option was found.

>`bool cutilsCliOptionFind(cutilsCliOption *option, int argc, char **argv)`

Searches if `option` matches any arguments in `argv` starting at `argv[1]`, and sets `index` for the struct of the option was found.

This function returns `true` if a match was found, otherwise it returns `false`

>`int cutilsCliOptionFindFlag(char *flag, int argc, char **argv)`

Searches if `flag` matches any arguments in `argv` starting at `argv[1]`.

This function returns the index for the last matched occurance of `flag`, otherwise it returns -1.

>`int cutilsCliOptionFindFlags(char *shortFlag, char *longFlag, int argc, char **argv)`

Searches if `shortFlag`, or `longFlag` matches any arguments in `argv` starting at `argv[1]`.

This function returns the index for the last matched occurance of `shortFlag`, or `longFlag` otherwise it returns -1.
