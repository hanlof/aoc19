#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intcode.h"

int intcode_prog[] = {
#include "inputs/input5"
};

int main(int argc, char * argv[])
{
	int ret;
	ret = run_prog_with_input(intcode_prog, PROG_SIZE(intcode_prog), 1);
	printf("Part 1: %d\n", ret);

	ret = run_prog_with_input(intcode_prog, PROG_SIZE(intcode_prog), 5);
	printf("Part 2: %d\n", ret);
	// Part 1: 4601506
	// Part 2: 5525561
	return 0;
}
