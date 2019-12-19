#include <stdio.h>

#include "intcode.h"

int64_t program[] = {
#include "inputs/input19"
};

int main(int argc, char * argv[])
{
	struct intcode_machine * m = NEW_INTCODE_MACHINE_EMEM(program, _I(), 10);

	int part1 = 0;
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			m = NEW_INTCODE_MACHINE(program, _I(i,j));
			PUT(m->inputs, i);
			PUT(m->inputs, j);
			run_machine(m);
			int64_t t = GET(m->outputs);
			printf("%ld ", t);
			part1 += t;
		}
		printf("\n");
	}
	printf("Part1: %d\n", part1);

	//for (int y = 1000; y < 1300; y++) {
	//	for (int x = 400; x < 600; x++) {
	for (int y = 400; y < 600; y++) {
		for (int x = 1000; x < 1300; x++) {
			m = NEW_INTCODE_MACHINE(program, _I(x,y));
			run_machine(m);
			int64_t t = GET(m->outputs);
			printf("%c", t ? '#' : ' ');
			part1 += t;
		}
		printf("\n");
	}

	// Again no programming solution. Just print the output and calculate manually!
	printf("Part2: %d\n", (400 + 11) + (10000 * (1000 + 73)));
	return 0;
}
