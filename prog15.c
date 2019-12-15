#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <alloca.h>
#include <termios.h>

#include "intcode.h"
#include "printu.h"

int64_t program[] = {
#include "inputs/input15"
};

#define UNKNOWN 0xFFFF
#define GOAL 0xFFFE
#define WALL 0xFFFD
#define OXYGEN 0xFFFC
#define OXYGEN_STEP 0xFFFB
#define LAST_TYPE 0xFFFA

int part1_count = 0;
int max_y = 0;

int bot_x = 50;
int bot_y = 50;
int area[100][100];

int goal_dist = 0;
void draw() {
	printU("\e[1;1H");
	for (int i = bot_y - 20; i < bot_y + 20; i++) {
		for (int j = 0; j < 100; j++) {
			switch (area[i][j]) {
				case UNKNOWN: printf(" "); break;
				case WALL: printf("#"); break;
				case GOAL: printf("@"); break;
				case OXYGEN: printf("O"); break;
				default: printf("%d", area[i][j] % 10); break;
			}
		}
		printf("\n");
	}
	int d = area[bot_y][bot_x];
	if (d == GOAL) d = goal_dist;
	printf("%d, %d %d\n", bot_x, bot_y, d);
	printfU("\e[%d;%dH", 21, bot_x + 1);
}

struct termios stored_settings;
void setup_term()
{
	tcgetattr(0, &stored_settings);

	struct termios new_settings = stored_settings;

	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ECHO); // don't echo the character

	//new_settings.c_cc[VTIME] = 1; // timeout (tenths of a second)
	//new_settings.c_cc[VMIN] = 0; // minimum number of characters

	tcsetattr(0, TCSANOW, &new_settings);
}

void update_square(int64_t out, int dx, int dy)
{
	int * old_pos = &area[bot_y][bot_x];
	if (out == 0) {
		area[bot_y + dy][bot_x + dx] = WALL;
	}
	if (out == 1) {
		bot_x += dx;
		bot_y += dy;
		if (area[bot_y][bot_x] == UNKNOWN) {
			area[bot_y][bot_x] = *old_pos + 1;
		}
	}
	if (out == 2) {
		if (area[bot_y][bot_x] == UNKNOWN) {
			goal_dist = *old_pos + 1;
		}
		bot_x += dx;
		bot_y += dy;
		area[bot_y][bot_x] = GOAL;
	}
}

void interactive_mode(struct intcode_machine * m)
{
	setup_term();
	while (1) {
		char c;
		draw();
		read(STDIN_FILENO, &c, 1);
		int64_t move = 0;
		int dx = 0, dy = 0;
		if (c == 'k') { move = 1; dx = 0; dy = -1; }
		if (c == 'j') { move = 2; dx = 0; dy = 1; }
		if (c == 'h') { move = 3; dx = -1; dy = 0; }
		if (c == 'l') { move = 4; dx = 1; dy = 0; }
		if (c == 'q') { break; }
		PUT(m->inputs, move);
		run_machine(m);
		int64_t out = GET(m->outputs);

		update_square(out, dx, dy);
	}
}

int update_oxygen(int x, int y)
{
	int updates = 0;
	if (area[y - 1][x] < LAST_TYPE) {
		area[y - 1][x] = OXYGEN_STEP;
		updates++;
	}
	if (area[y + 1][x] < LAST_TYPE) {
		area[y + 1][x] = OXYGEN_STEP;
		updates++;
	}
	if (area[y][x - 1] < LAST_TYPE) {
		area[y][x - 1] = OXYGEN_STEP;
		updates++;
	}
	if (area[y][x + 1] < LAST_TYPE) {
		area[y][x + 1] = OXYGEN_STEP;
		updates++;
	}
	printf("\e[40;1HUPDATES %d\n", updates);
	return updates;
}
int main(int argc, char * argv[])
{
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			area[i][j] = UNKNOWN;
		}
	}
	area[bot_y][bot_x] = 0;
	struct intcode_machine * m = NEW_INTCODE_MACHINE(program, _I());

	int move = 1;
	int64_t out;
	int dx = 0;
	int dy = 0;
	int * first_wall = NULL;
	int * first_pos = &area[bot_y][bot_x];
	int * goal_pos = NULL;
	while (1) {
		switch (move) {
			case 1: dx =  0; dy = -1; break;
			case 2: dx =  0; dy =  1; break;
			case 3: dx = -1; dy =  0; break;
			case 4: dx =  1; dy =  0; break;
		}

		PUT(m->inputs, move);
		run_machine(m);
		out = GET(m->outputs);
		update_square(out, dx, dy);
		draw();

		if (out == 2) {
			goal_pos = &area[bot_y][bot_x];
		}
		if (out != 0) {
			switch (move) {
				case 1: move = 4; break;
				case 2: move = 3; break;
				case 3: move = 1; break;
				case 4: move = 2; break;
			}
		} else {
			int * this_wall = &area[bot_y + dy][bot_x + dx];
			int * this_pos = &area[bot_y][bot_x];
			if (first_wall == NULL) {
				first_wall = this_wall;
				first_pos = &area[bot_y][bot_x];
			} else {
				if (this_wall == first_wall && this_pos == first_pos) {
					break;
				}
			}
			switch (move) {
				case 1: move = 3; break;
				case 2: move = 4; break;
				case 3: move = 2; break;
				case 4: move = 1; break;
			}
		}
		// if we moved
		usleep(1 * 1000);
	}

	*goal_pos = OXYGEN;
	int steps = 0;
	int updates = 1;
	while (updates != 0) {
		updates = 0;
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				if (area[i][j] == OXYGEN) {
					updates += update_oxygen(j, i);
				}
			}
		}
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				if (area[i][j] == OXYGEN_STEP) {
					area[i][j] = OXYGEN;
				}
			}
		}
		steps++;
		draw();
		usleep(50 * 1000);
	}
	printf("\n\nSTEPS!!! %d\n\n", steps - 1); // subtract 1 for the last step
	return 0;
}

