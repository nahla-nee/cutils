SRCS = src/str.c
INCLUDE = -Iinclude/
FLAGS = -O2 -fpic -c

.PHONY: .clean
.clean:
	rm out/*

default:
	cc ${FLAGS} ${SRCS} ${INCLUDE}
	mv *.o out
	cc -shared -o libcutils out/*.o
