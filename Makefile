all: $(patsubst %.c,bin/%,$(wildcard *.c))

CFLAGS = -Wall -g -O3 -D_BSD_SOURCE -std=c99
bin/prog2 bin/prog5 bin/prog7: intcode.h
bin/prog2: inputs/input2
bin/prog4: inputs/input4
bin/prog5: inputs/input5

bin/%: %.c bin/.STAMP
	gcc $(CFLAGS) $< -o $@

bin/.STAMP:
	mkdir bin
	touch bin/.STAMP
