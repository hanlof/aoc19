
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <termios.h>

#include "intcode.h"
#include "printu.h"

int64_t program[] = {
#include "inputs/input23"
};

struct intcode_machine * nodes[50];

int64_t nat_x = 0;
int64_t nat_y = 0;

int64_t part1 = 0;
int64_t part2 = 0;

int main(int argc, char * argv[])
{
	for (int i = 0; i < 50; i++) {
		nodes[i] = NEW_INTCODE_MACHINE_EMEM(program, _I(i, -1), 100);
	}

	int count = 0;
	int idle = 0;
	int last_nat = 0;
	while (1) {
		idle++;
		for (int i = 0; i < 50; i++) {
			if (EMPTY(nodes[i]->inputs)) {
				nodes[i]->inputs.read = 0;
				nodes[i]->inputs.write = 0;
				PUT(nodes[i]->inputs, -1);
			} else {
				idle = 0;
			}
			run_machine(nodes[i]);
			if (!EMPTY(nodes[i]->outputs)) {
				int64_t addr = GET(nodes[i]->outputs);
				int64_t x = GET(nodes[i]->outputs);
				int64_t y = GET(nodes[i]->outputs);
				if (addr == 255) {
					if (part1 == 0) part1 = y;
					// empty NAT queue, (always overwrite)
					nat_x = x;
					nat_y = y;
				} else {
					PUT(nodes[addr]->inputs, x);
					PUT(nodes[addr]->inputs, y);
				}
			}
		}
		if (idle > 1) {
			if (last_nat == nat_y) {
				part2 = nat_y;
				break;
			}
			last_nat = nat_y;
			PUT(nodes[0]->inputs, nat_x);
			PUT(nodes[0]->inputs, nat_y);
		}
		count++;
	}
	printf("Part1: %ld\n", part1);
	printf("Part2: %ld\n", part2);
}

