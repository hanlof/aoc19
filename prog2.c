#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input2"
};

#define MAGIC 19690720
int main(int argc, char * argv[])
{
	// Part 1
	struct fifo in = { 0, 0 };
	struct fifo out = { 0, 0 };
	int64_t * mem = malloc(PROG_SIZE(intcode_prog) * sizeof(int64_t));
	memcpy(mem, intcode_prog, PROG_SIZE(intcode_prog) * sizeof(int64_t));
	mem[1] = 12;
	mem[2] = 2;
	run_prog(mem, NULL, &in, &out);
	printf("Part 1: %ld\n", mem[0]);

	// Part 2
	for (int64_t noun = 0; noun < 99; noun++) {
		for (int64_t verb = 0; verb < 99; verb++) {
			memcpy(mem, intcode_prog, PROG_SIZE(intcode_prog) * sizeof(int64_t));
			mem[1] = noun;
			mem[2] = verb;
			run_prog(mem, NULL, &in, &out);
			if (mem[0] == MAGIC) {
				printf("Part 2: %ld\n", noun * 100 + verb);
			}
		}
	}
}
