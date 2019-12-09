all: $(patsubst %.c,bin/%,$(wildcard *.c))

CFLAGS += -Wall -g -O3 -D_BSD_SOURCE -std=c99

bin/prog2 bin/prog5 bin/prog7 bin/prog9: intcode.h

bin/%: %.c bin/.STAMP Makefile
	gcc $(CFLAGS) $< -o $@

bin/.STAMP:
	mkdir bin
	touch bin/.STAMP
