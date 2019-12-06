all: $(patsubst %.c,bin/%,$(wildcard *.c))

bin/prog2 bin/prog5: intcode.h
bin/prog2: inputs/input2
bin/prog4: inputs/input4
bin/prog5: inputs/input5

bin/%: %.c bin/.STAMP
	gcc -g -O3 -std=c99 $< -o $@

bin/.STAMP:
	mkdir bin
	touch bin/.STAMP
