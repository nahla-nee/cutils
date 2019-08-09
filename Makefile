SRCS = src/str.c src/bytestream.c src/file.c src/cliOptions.c
INCLUDE = -Iinclude/
FLAGS = -std=c11 -O2 -fpic -c -D_FILE_OFFSET_BITS=64

.PHONY: .clean
.clean:
	rm out/*

default:
	cc ${FLAGS} ${SRCS} ${INCLUDE}
	mv *.o out
	cc -shared -o libcutils.so out/*.o
