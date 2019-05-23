.PHONY: all

all: libs
	cc -Wall -o a31 a31.c libs/*

clean:
	rm a31
	rm -rf libs/

libsdir:
	mkdir -p libs/

libs: libsdir
	cc -c -g -Wall -o libs/utils.o utils.c
