#include <stdio.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input7"
};

int64_t highest = 0;
int64_t highest_feedback = 0;

void run_feedbackloop(struct stack * order) {
	int64_t ret = 0;
	struct intcode_machine * m[5];

	for (int i = 0; i < 5; i++) {
		m[i] = NEW_INTCODE_MACHINE(intcode_prog, _I(order->values[i] + 5));
	}
	for (int i = 0; i < 5; i++) {
		m[i]->outputs_ptr = m[(i + 1) % 5]->inputs_ptr; // connect outputs to inputs
	}

	PUT(m[0]->inputs, 0); // initial value 0 in first machine
	do {
		run_machine(m[0]);
		run_machine(m[1]);
		run_machine(m[2]);
		run_machine(m[3]);
		ret = run_machine(m[4]);
	} while (ret != 99);

	ret = GET(m[0]->inputs);
	if (ret > highest_feedback) {
		highest_feedback = ret;
	}
}

struct intcode_machine slask;
void runall(int64_t * set, int64_t firstinput, int nelements, struct stack * perms) {
	struct fifo inputs = { 0, 0 };
	struct fifo outputs = { 0, 0 };
	int64_t ret = 0;
	int64_t * tmpset = set;
	while (*tmpset != -1) {
		if (*tmpset == -2) { tmpset++; continue; }
		PUT(inputs, *tmpset);
		PUT(inputs, firstinput);
		run_prog(intcode_prog, NULL, &inputs, &outputs, &slask);
		ret = GET(outputs);
		int64_t o = *tmpset;
		*tmpset = -2;
		PUSH(*perms, o);
		if (nelements == 1) { // runall() was called with 4 permutations in perms and we just pushed the final one
			run_feedbackloop(perms);
		} else { // we don't have a complete permutation in perms, keep recursing!
			runall(set, ret, nelements - 1, perms);
		}
		(void)POP(*perms);
		*tmpset = o;
		tmpset++;
	}
	if (ret > highest) {
		highest = ret;
	}
}

int main(int argc, char * argv[])
{
	slask.relative_base = 0;
	int64_t comb[6] = { 0, 1, 2, 3, 4, -1 };
	struct stack s = { 0, { 0 }};
	runall(comb, 0, 5, &s);
	printf("Part 1: %ld\n", highest);
	printf("Part 2: %ld\n", highest_feedback);


}
