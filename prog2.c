#include <stdio.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input2"
};

#define MAGIC 19690720
int main(int argc, char * argv[])
{
	// Part 1
	struct intcode_machine * m = NEW_INTCODE_MACHINE(intcode_prog, _I());
	m->mem[1] = 12;
	m->mem[2] = 2;
	run_machine(m);
	printf("Part 1: %ld\n", m->mem[0]);

	// Part 2
	for (int64_t noun = 0; noun < 99; noun++) {
		for (int64_t verb = 0; verb < 99; verb++) {
			memcpy(m->mem, intcode_prog, PROG_SIZE(intcode_prog) * sizeof(int64_t));
			m->mem[1] = noun;
			m->mem[2] = verb;
			m->prog_ctr = &m->mem[0];
			run_machine(m);
			if (m->mem[0] == MAGIC) {
				printf("Part 2: %ld\n", noun * 100 + verb);
			}
		}
	}
}
