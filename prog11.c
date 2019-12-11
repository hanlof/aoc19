#include <stdio.h>
#include <unistd.h>
#include "intcode.h"

int64_t intcode_prog[] = {
#include "inputs/input11"
};

#define MAX_SIZE
char area[200][200];
int robot_dir = 0;
int robot_x = 110;
int robot_y = 110;

void draw(int min_x, int min_y, int max_x, int max_y)
{
	char c;
	for (int i = min_y; i <= max_y; i++) {
		for (int j = min_x; j <= max_x; j++) {
			if (robot_y == i && robot_x == j) {
				if (robot_dir == 0) c = '^';
				if (robot_dir == 1) c = '>';
				if (robot_dir == 2) c = 'v';
				if (robot_dir == 3) c = '<';
				printf("\e[31m%c%c\e[0m", c, c);
			} else {
				if (area[i][j] == 1) {
					printf("\e[47m  ");
				} else {
					printf("\e[0m  ");
				}
			}
		}
		printf("\n");
	}
}

#define UNTOUCHED 5
int main(int argc, char * argv[])
{
	struct intcode_machine * m;
	m = NEW_INTCODE_MACHINE_EMEM(intcode_prog, _I(), 0);
	int ret = 0;
	int count = 0;
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 200; j++) {
			area[i][j] = UNTOUCHED;
		}
	}
	int min_x = robot_x;
	int min_y = robot_y;
	int max_x = robot_x;
	int max_y = robot_y;
	area[robot_y][robot_x] = 1;
	printf("\e[s"); // save cursor position
	while (1) {
		if (area[robot_y][robot_x] == UNTOUCHED) {
			PUT(m->inputs, 0);
		} else {
			PUT(m->inputs, area[robot_y][robot_x]);
		}

		ret = run_machine(m);

		int64_t col = GET(m->outputs);
		int64_t dir = GET(m->outputs);
		if (m->outputs.write != m->outputs.read) {
			printf("Outputs not empty! read:%d write:%d", m->outputs.write, m->outputs.read);
		}

		if (area[robot_y][robot_x] == UNTOUCHED) count++;
		if (col == 0) {
			area[robot_y][robot_x] = 0;
		} else if (col == 1) {
			area[robot_y][robot_x] = 1;
		} else {

			printf("Wierd col! %ld\n", col);
			area[robot_y][robot_x] = 1;
		}

		if (dir == 1) {
			robot_dir++;
			if (robot_dir > 3) {
				robot_dir = 0;
			}
		} else if (dir == 0) {
			robot_dir--;
			if (robot_dir < 0) {
				robot_dir = 3;
			}
		} else {
			printf("Illegal turn %ld\n", dir);
		}
		if (robot_dir == 0) robot_y--;
		if (robot_dir == 1) robot_x++;
		if (robot_dir == 2) robot_y++;
		if (robot_dir == 3) robot_x--;

		if (robot_x < min_x) min_x = robot_x;
		if (robot_x > max_x) max_x = robot_x;
		if (robot_y < min_y) min_y = robot_y;
		if (robot_y > max_y) max_y = robot_y;

		printf("\e[u");
		draw(min_x, min_y, max_x + 1, max_y + 1);
		usleep(20000);
		if (ret == 99) {
			printf("done!\n");
			break;
		}
	}
	printf("@ %d %d c:%d\n", robot_x, robot_y, count);
	//printf("min:%d,%d max:%d,%d\n", min_x, min_y, max_x, max_y);
//	draw(min_x, min_y, max_x + 1, max_y + 1);

}
