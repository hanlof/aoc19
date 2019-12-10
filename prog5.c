#include <stdio.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input5"
};

int main(int argc, char * argv[])
{
	struct intcode_machine * m;
	m = NEW_INTCODE_MACHINE(intcode_prog, _I(1));
	run_machine(m);
	// need to get the final output (many outputs are emitted)
	int64_t ret = 0;
	while (!EMPTY(m->outputs)) {
	       ret = GET(m->outputs);
	}
	printf("Part 1: %ld\n", ret);

	m = NEW_INTCODE_MACHINE(intcode_prog, _I(5));
	run_machine(m);
	printf("Part 2: %ld\n", GET(*(m->outputs_ptr)));

	return 0;
}
