SRCS = src/str.c src/bytestream.c
INCLUDE = -Iinclude/
FLAGS = -O2 -fpic -c

.PHONY: .clean
.clean:
	rm out/*

default:
	cc ${FLAGS} ${SRCS} ${INCLUDE}
	mv *.o out
	cc -shared -o libcutils.so out/*.o
