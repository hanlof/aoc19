all: $(patsubst %.c,bin/%,$(wildcard *.c))

CFLAGS += -Wall -g -O3 -D_BSD_SOURCE -std=c99 -lm

bin/prog2  bin/prog5  bin/prog7  bin/prog9  bin/prog11 bin/prog13: intcode.h
bin/prog15 bin/prog17 bin/prog19 bin/prog21 bin/prog23: intcode.h

bin/%: %.c bin/.STAMP Makefile
	gcc $(CFLAGS) $< -o $@

bin/.STAMP:
	mkdir bin
	touch bin/.STAMP
