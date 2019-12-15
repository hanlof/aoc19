#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>

#include "printu.h"
#include "intcode.h"

int64_t program[] = {
#include "inputs/input13"
};

int part1_count = 0;
int max_y = 0;

#define printfU(s, ...) { \
	int n = 1 + snprintf(NULL, 0, s, __VA_ARGS__); \
	char * c = alloca(n); \
	snprintf(c, n, s, __VA_ARGS__); \
	write(STDOUT_FILENO, c, n); }


void draw(int x, int y, int col) {
	if (x == -1 && y == 0) {
		printfU("\e[1;1H\e[0m\e[KScore: %d\n", col);
	} else {
		printfU("\e[%d;%dH", y + 2, (x * 2) + 1);
		printfU("\e[4%dm  ", col);
		if (col == 2) part1_count++;
		if (y > max_y) max_y = y;
	}

}

int main(int argc, char * argv[])
{

	struct intcode_machine * m = NEW_INTCODE_MACHINE(program, _I());
	run_machine(m);
	printf("\e[s");
	while (!EMPTY(m->outputs)) {
			int x = (int)GET(m->outputs);
			int y = (int)GET(m->outputs);
			int col = (int)GET(m->outputs);
			draw(x, y, col);
	}
	//printf("\e[u\e[0mPart1: %d\n", part1_count);
	draw(-1, 0, part1_count);
	printf("\e[%d;1H\e[0m", max_y + 3);
	m = NEW_INTCODE_MACHINE(program, _I());
	m->mem[0] = 2;
	int ball_x = 0;
	int paddle_x = 0;
	(void)run_machine(m);
	while (!EMPTY(m->outputs)) {
		int x = (int)GET(m->outputs);
		int y = (int)GET(m->outputs);
		int col = (int)GET(m->outputs);
		if (col == 4) { // ball
			ball_x = x;
		}
		if (col == 3) { // paddle
			paddle_x = x;
		}
		draw(x, y, col);
	}
	while (1) {
		int ret = run_machine(m);
		while (!EMPTY(m->outputs)) {
				int x = (int)GET(m->outputs);
				int y = (int)GET(m->outputs);
				int col = (int)GET(m->outputs);
				if (col == 4) { // ball
					ball_x = x;
				}
				if (col == 3) { // paddle
					paddle_x = x;
				}
				draw(x, y, col);
				usleep(20 * 1000);
		}

		if (ball_x < paddle_x) {
			PUT(m->inputs, -1);
		} else if (ball_x > paddle_x) {
			PUT(m->inputs, 1);
		} else {
			PUT(m->inputs, 0);
		}
		if (ret == 99) break;
	}
	return 0;
}

