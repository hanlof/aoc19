
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

int main(int argc, char * argv[])
{
	for (int i = 0; i < 50; i++) {
		nodes[i] = NEW_INTCODE_MACHINE_EMEM(program, _I(i, -1), 100);
	}

	int count = 0;
	while (1) {
		for (int i = 0; i < 50; i++) {
			if (EMPTY(nodes[i]->inputs)) {
				PUT(nodes[i]->inputs, -1);
			}
			run_machine(nodes[i]);
			if (!EMPTY(nodes[i]->outputs)) {
				int64_t addr = GET(nodes[i]->outputs);
				int64_t x = GET(nodes[i]->outputs);
				int64_t y = GET(nodes[i]->outputs);
				printf("%d: %d -> %ld {%ld,%ld)\n", count, i, addr, x, y);
				if (addr == 255) {
					printf("DONE! %ld\n", y);
					return (0);
				}
				PUT(nodes[addr]->inputs, x);
				PUT(nodes[addr]->inputs, y);
			}
		}
		count++;
	}
}

