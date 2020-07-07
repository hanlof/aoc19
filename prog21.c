#include <stdio.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input21"
};

void put_str(struct intcode_machine * m, char * s)
{
	while (*s) PUT(m->inputs, *(s++));
	PUT(m->inputs, '\n');
}

// @................
// #####.#.#...#####

// @
// #####...###..####

// ....@............
// #####.#.#.##.####
//
// #####.#.#@##.####

#define S(a, b) #a " " #b
#define NOT(a,b) put_str(m, "NOT " S(a, b))
#define OR(a,b) put_str(m, "OR " S(a, b))
#define AND(a,b) put_str(m, "AND " S(a, b))
int main(int argc, char * argv[])
{
	struct intcode_machine * m;
	m = NEW_INTCODE_MACHINE(intcode_prog, _I());
	run_machine(m);
	while (!EMPTY(m->outputs)) {
		printf("%c", (char)GET(m->outputs));
	}

	// J = 1 if smth to land on!
//	put_str(m, "NOT D J");
//	put_str(m, "NOT J J");
//
//	// T = 1
//	put_str(m, "NOT D T");
//	put_str(m, "OR D T");
//
//	// IF ANY HOLE A-C SET T TO 0
//	put_str(m, "AND A T");
//	put_str(m, "AND B T");
//	put_str(m, "AND C T");
//
//	// IF ANY HOLE A-C SET T TO 1
//	put_str(m, "NOT T T");
//
//	put_str(m, "AND T J");

	// HOLE AT C, JUMP if we can land
	NOT(C, J);
	AND(D, J);

	// BUT WAIT. SKIP JUMP IF E AND H are holes

	NOT(E, T); // T is 1 if E is hole
	NOT(T, T); // T is 0 if E is hole
	OR(H, T); // 
	AND(T, J);


	// HOLE AT A, JUMP
	NOT(A, T);
	OR(T, J);

        // HOLE AT B, JUMP IF we can land?
	NOT(B, T);
	AND(D, T);
	OR(T, J);

	put_str(m, "RUN");
	printf("ret: %ld\n", run_machine(m));

	printf("%d\n", m->outputs.write - m->outputs.read);
	if (m->outputs.read - m->outputs.write == -1) {
		while (!EMPTY(m->outputs)) {
			printf("%ld", GET(m->outputs));
		}
	}
	while (!EMPTY(m->outputs)) {
		printf("%ld\n", GET(m->outputs));

	}
	return 0;
}
