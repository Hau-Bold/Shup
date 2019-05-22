.PHONY: all

all: libs
	cc -Wall -o a13 a13.c libs/*

clean:
	rm a13
	rm -rf libs/

libsdir:
	mkdir -p libs/

libs: libsdir
	cc -c -g -Wall -o libs/utils.o utils.c
