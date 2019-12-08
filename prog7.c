#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input7"
};

int64_t highest = 0;
int64_t highest_feedback = 0;

void run_feedbackloop(struct stack * order) {
	int64_t ret = 0;
	int64_t * progs[5];
	int64_t * progctrs[5];
	struct fifo infifos[5];
	for (int i = 0; i < 5; i++) {
		infifos[i].read = 0;
		infifos[i].write = 0;
		PUT(infifos[i], order->values[i] + 5); // set phase here
		progs[i] = malloc(PROG_SIZE(intcode_prog) * sizeof(int64_t));
		memcpy(progs[i], intcode_prog, PROG_SIZE(intcode_prog) * sizeof(int64_t));
		progctrs[i] = &progs[i][0];
	}
	PUT(infifos[0], 0);
	do {
		run_prog(progs[0], &progctrs[0], &infifos[0], &infifos[1]);
		run_prog(progs[1], &progctrs[1], &infifos[1], &infifos[2]);
		run_prog(progs[2], &progctrs[2], &infifos[2], &infifos[3]);
		run_prog(progs[3], &progctrs[3], &infifos[3], &infifos[4]);
		ret = run_prog(progs[4], &progctrs[4], &infifos[4], &infifos[0]);
	} while (ret != 99);

	int res = GET(infifos[0]);
	if (res > highest_feedback) {
		highest_feedback = res;
	}
}

void runall(int64_t * set, int64_t firstinput, int nelements, struct stack * perms) {
	struct fifo inputs = { 0, 0 };
	struct fifo outputs = { 0, 0 };
	int64_t ret = 0;
	int64_t * tmpset = set;
	while (*tmpset != -1) {
		if (*tmpset == -2) { tmpset++; continue; }
		PUT(inputs, *tmpset);
		PUT(inputs, firstinput);
		run_prog(intcode_prog, NULL, &inputs, &outputs);
		ret = GET(outputs);
		int64_t o = *tmpset;
		*tmpset = -2;
		PUSH(*perms, o);
		if (nelements == 1) { // we have a permutation of 4 in perms
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
	int64_t comb[6] = { 0, 1, 2, 3, 4, -1 };
	struct stack s = { 0, { 0 }};
	runall(comb, 0, 5, &s);
	printf("Part 1: %ld\n", highest);
	printf("Part 2: %ld\n", highest_feedback);


}
