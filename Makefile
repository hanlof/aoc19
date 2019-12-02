all: $(patsubst %.c,bin/%,$(wildcard *.c))

bin/%: %.c bin/.STAMP
	gcc -O3 -std=c99 $< -o $@

bin/.STAMP:
	mkdir bin
	touch bin/.STAMP
