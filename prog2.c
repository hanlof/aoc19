#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intcode.h"

int intcode_prog[] = {
#include "inputs/input2"
};

#define MAGIC 19690720
int main(int argc, char * argv[])
{
	// Part 1
	intcode_prog[1] = 12;
	intcode_prog[2] = 2;
	int ret = run_prog(intcode_prog, PROG_SIZE(intcode_prog));
	printf("Part 1: %d\n", ret);

	// Part 2
	for (int noun = 0; noun < 99; noun++) {
		for (int verb = 0; verb < 99; verb++) {
			intcode_prog[1] = noun;
			intcode_prog[2] = verb;
			int ret = run_prog(intcode_prog, PROG_SIZE(intcode_prog));
			if (ret == MAGIC) {
				printf("Part 2: %d\n", noun * 100 + verb);
			}
		}
	}
}
