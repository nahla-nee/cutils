CC=clang
SRCS=src/str.c src/bytestream.c src/file.c src/cliOptions.c
INCLUDE=-Iinclude/
FLAGS=-O2 -fpic -c -D_FILE_OFFSET_BITS=64

.PHONY: .clean
.clean:
	rm out/*

cutils:
	${CC} ${FLAGS} ${SRCS} ${INCLUDE}
	mv *.o out
	${CC} -shared -o libcutils.so out/*.o

cutilsDebug:
	${CC} ${FLAGS} -g ${SRCS} ${INCLUDE}
	mv *.o out
	${CC} -shared -o libcutils.so out/*.o
