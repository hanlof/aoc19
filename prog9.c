#include <stdio.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input9"
};

int main(int argc, char * argv[])
{
	struct intcode_machine * m;

	m = NEW_INTCODE_MACHINE(intcode_prog, _I(1));
	run_machine(m);
	printf("Part 1: %ld\n", GET(*(m->outputs_ptr)));

	m = NEW_INTCODE_MACHINE(intcode_prog, _I(2));
	run_machine(m);
	printf("Part 2: %ld\n", GET(*(m->outputs_ptr)));

	return 0;
}
