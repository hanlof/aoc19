
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <termios.h>

#include "intcode.h"
#include "printu.h"

int64_t program[] = {
#include "inputs/input17"
};

#define MAX_Y 31
#define MAX_X 51
char area[MAX_Y][MAX_X + 1]; // leave room for the newline

int is_x(char * c)
{
	int ok = (*c == '#');
	ok &= c[-1] == '#';
	ok &= c[1] == '#';
	ok &= c[-(MAX_X + 1)] == '#';
	ok &= c[(MAX_X + 1)] == '#';
	if (ok) *c = 'X';
	return ok;
}

void printmap() {
	for (int i = 0; i < MAX_Y; i++) {
		printf("%52.52s", &area[i][0]);
	}
}

int main(int argc, char * argv[])
{
	struct intcode_machine * m = NEW_INTCODE_MACHINE(program, _I());
	run_machine(m);
	char * t = &area[0][0];
	while (!EMPTY(m->outputs)) {
		char c = GET(m->outputs);
		switch (c) {
			case '#': break;
			case '.': break;
			case '\n': break;
			default: printf("XXX %d (%c)\n", c, c); break;
		}

		*(t++) = c;
	}

	//printmap();
	char L6_R12_R8[]     = "L,6,R,12,R,8\n";
	char R8_R12_L12[]    = "R,8,R,12,L,12\n";
	char R12_L12_L4_L4[] = "R,12,L,12,L,4,L,4\n";

	char * A = L6_R12_R8;
	char * B = R8_R12_L12;
	char * C = R12_L12_L4_L4;

	char sequences[] =  "A,B,B,A,C,A,C,A,C,B\n";

	//char movesinonestring[] = {L6_R12_R8  R8_R12_L12    R8_R12_L12     L6_R12_R8 R12_L12_L4_L4     L6_R12_R8     R12_L12_L4_L4     L6_R12_R8      R12_L12_L4_L4     R8_R12_L12 };
	//char movesinonestring[] = {"L,6,R,12,R,8,R,8,R,12,L,12,R,8,R,12,L,12,L,6,R,12,R,8,R,12,L,12,L,4,L,4,L,6,R,12,R,8,R,12,L,12,L,4,L,4,L,6,R,12,R,8,R,12,L,12,L,4,L,4,R,8,R,12,L,12"};
	//char allmoves[] = { L 6, R 12, R 8, R 8, R 12, L 12, R 8, R 12, L 12, L 6, R 12, R 8, R 12, L 12, L 4, L 4, L 6, R 12, R 8, R 12, L 12, L 4, L 4, L 6, R 12, R 8, R 12, L 12, L 4, L 4, R 8, R 12, L 12 };
	int part1 = 0;
	// skip top, bottom, leftmost and rightmost edge
	for (int i = 1; i < MAX_Y - 1; i++) {
		for (int j = 1; j < MAX_X; j++) {
			if (is_x(&area[i][j])) part1 += (i * j);
		}
	}
	printf("Part1: %d\n", part1);

	m = NEW_INTCODE_MACHINE(program, _I());
	m->mem[0] = 2;
	char * p = sequences;
	while (*p != '\0') PUT(m->inputs, *(p++));
	p = A;
	while (*p != '\0') PUT(m->inputs, *(p++));
	p = B;
	while (*p != '\0') PUT(m->inputs, *(p++));
	p = C;
	while (*p != '\0') PUT(m->inputs, *(p++));
	p = C;
	while (*p != '\0') PUT(m->inputs, *(p++));
	PUT(m->inputs, 'n');
	PUT(m->inputs, '\n');
	run_machine(m);
	while (!EMPTY(m->outputs))
		printf(">>> %ld\n", GET(m->outputs));

	return 0;
}

