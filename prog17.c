
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
/*
 * This is the map
 *
 * ..............#####................................
 * ..............#...#................................
 * ....#############.#................................
 * ....#.........#.#.#................................
 * #############.#######..............................
 * #...#.......#...#.#.#..............................
 * #...#.......#...#.#.#.....................#########
 * #...#.......#...#.#.#.....................#.......#
 * #####.......#...#.#.#.....................#.......#
 * ............#...#.#.#.....................#.......#
 * ....#############.#.#.....................#.......#
 * ....#.......#.....#.#.....................#.......#
 * ....#.#############.#.....................#.......#
 * ....#.#.....#.......#.....................#.......#
 * ....#.#.....#.....#############.......#############
 * ....#.#.....#.....#.#.........#.......#...#........
 * ....#.#.....#########.........#.......#...#........
 * ....#.#...........#...........#.......#...#........
 * ....#.#.#########.#...........#.......#...######^..
 * ....#.#.#.......#.#...........#.......#............
 * ....#.#############...........#.......#............
 * ....#...#.......#.............#.......#............
 * ....#####.......#.............#.......#............
 * ................#.............#.......#............
 * ................#.............#.......#............
 * ................#.............#.......#............
 * ................#.............#########............
 * ................#..................................
 * ................#..................................
 * ................#..................................
 * ................#############......................
 */

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
			case '^': break;
			default: printf("XXX %d (%c)\n", c, c); break;
		}

		*(t++) = c;
	}

	//printmap();

	int part1 = 0;
	for (int i = 1; i < MAX_Y - 1; i++) {     // skip top and bottom edge
		for (int j = 1; j < MAX_X; j++) { // skip leftmost and rightmost edge
			if (is_x(&area[i][j])) part1 += (i * j);
		}
	}
	printf("Part1: %d\n", part1);

	// manually created this move list: "L,6,R,12,R,8,R,8,R,12,L,12,R,8,R,12,L,12,L,6,R,12,R,8,R,12,L,12,L,4,L,4,L,6,R,12,R,8,R,12,L,12,L,4,L,4,L,6,R,12,R,8,R,12,L,12,L,4,L,4,R,8,R,12,L,12"

	// it is also quite easy to split it into the following manually!

	char L6_R12_R8[]     = "L,6,R,12,R,8\n";
	char R8_R12_L12[]    = "R,8,R,12,L,12\n";
	char R12_L12_L4_L4[] = "R,12,L,12,L,4,L,4\n";

	char * A = L6_R12_R8;
	char * B = R8_R12_L12;
	char * C = R12_L12_L4_L4;

	char sequences[] =  "A,B,B,A,C,A,C,A,C,B\n";

	m = NEW_INTCODE_MACHINE(program, _I());
	m->mem[0] = 2;

	for (char * p = sequences; *p != '\0'; p++) PUT(m->inputs, *p);
	for (char * p = A; *p != '\0'; p++) PUT(m->inputs, *p);
	for (char * p = B; *p != '\0'; p++) PUT(m->inputs, *p);
	for (char * p = C; *p != '\0'; p++) PUT(m->inputs, *p);
	for (char * p = C; *p != '\0'; p++) PUT(m->inputs, *p); // NOTE! sequence D is not needed
	for (char * p = "n\n"; *p != '\0'; p++) PUT(m->inputs, *p); // No camera feed!

	run_machine(m);

	int part2 = 0;
	while (!EMPTY(m->outputs)) {
		part2 = GET(m->outputs);
	}
	printf("Part2: %d\n", part2);

	return 0;
}

