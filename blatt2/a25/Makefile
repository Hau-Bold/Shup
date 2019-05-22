.PHONY: all

all: libs
	cc -Wall -o a25 a25.c libs/*

clean:
	rm a25
	rm -rf libs/

libsdir:
	mkdir -p libs/

libs: libsdir
	cc -c -g -Wall -o libs/utils.o utils.c
