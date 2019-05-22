.PHONY: all

all: libs
	cc -Wall -o a52 a52.c libs/*

clean:
	rm a52
	rm -rf libs/

libsdir:
	mkdir -p libs/

libs: libsdir
	cc -c -g -Wall -o libs/utils.o utils.c
