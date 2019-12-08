#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input5"
};

int main(int argc, char * argv[])
{
	int64_t ret = 0;
	struct fifo in = { 0, 0 };
	struct fifo out = { 0, 0 };
	int64_t * mem = malloc(PROG_SIZE(intcode_prog) * sizeof(int64_t));
	memcpy(mem, intcode_prog, PROG_SIZE(intcode_prog) * sizeof(int64_t));
	PUT(in, 1);
	run_prog(mem, NULL, &in, &out);
	while (!EMPTY(out)) {
		ret = GET(out);
	}
	printf("Part 1: %ld\n", ret);

	// part 2
	memcpy(mem, intcode_prog, PROG_SIZE(intcode_prog) * sizeof(int64_t));
	PUT(in, 5);
	run_prog(mem, NULL, &in, &out);
	while (!EMPTY(out)) {
		ret = GET(out);
	}
	printf("Part 2: %ld\n", ret);
	return 0;
}
