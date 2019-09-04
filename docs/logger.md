# logger

A simple logging utility

## Enum

```c
typedef enum cutilsLogLevel{
	CUTILS_INFO,
	CUTILS_WARNING,
	CUTILS_ERROR,
	CUTILS_FATAL
} cutilsLogLevel;
```

## Struct

`info` file to print to when logging info level messages.

`warning` file to print to when logging warning level messages.

`error` file to print to when logging error level messages.

`fatal` file to print to when logging fatal level messages.

`logLevel` the minimum level for that a message must be to to be logged.

Note: the init function intializes `info`, and `warning` to be `stdout`, and `error`, and `fatal` to be `stderr` by default.

```c
typedef struct cutilsLogger{
	FILE *info;
	FILE *warning;
	FILE *error;
	FILE *fatal;
	enum cutilsLogLevel logLevel;
} cutilsLogger;
```

## Functions

>`void cutilsLoggerInit(cutilsLogger *logger, cutilsLogLevel level)`

Initializes the logger files, and sets `logger->logLevel` to `level`.

>`cutilsLogger* cutilsLoggerNew(cutilsLogLevel level)`

Allocates a new `cutilsLogger` struct, and intializes it with `level`.

This function returns a pointer to a `cutilsLogger` struct if no errors occurred, or `NULL` if it could not allocate the memory required for the struct.

>`void cutilsLoggerDeinit(cutilsLogger *logger)`

Zeroes out the logger struct.

>`void cutilsLoggerFree(cutilsLogger *logger)`

Zeroes out the logger struct, and then calls free on `logger`. Use this with structs that have been allocated using `cutilsLoggerNew`

>`void cutilsLoggerLog(cutilsLogger *logger, const char *msg, cutilsLogLevel level)`

Prints `msg` to the appropriate log file if `level` is greater than or equal to `logger->logLevel`.

