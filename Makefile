SRCS = src/str.c src/bytestream.c src/file.c
INCLUDE = -Iinclude/
FLAGS = -O2 -fpic -c -D_FILE_OFFSET_BITS=64

.PHONY: .clean
.clean:
	rm out/*

default:
	cc ${FLAGS} ${SRCS} ${INCLUDE}
	mv *.o out
	cc -shared -o libcutils.so out/*.o
