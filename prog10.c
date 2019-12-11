#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


#define SIZE_X 26
#define SIZE_Y 26
#define SIZE_B (SIZE_Y * (SIZE_X + 1))

struct asteroid {
	int x;
	int y;
	double angle;
};

int angle_comparator(const void * ap, const void * bp)
{
	return ( *((struct asteroid **)ap) )->angle < ( *((struct asteroid **)bp) )->angle;
}


void mark_visible(int ox, int oy, char map[SIZE_Y][SIZE_X + 1]) {
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			int dx = x - ox;
			int dy = y - oy;
			if (!dx && !dy) continue;
			int tx = x;
			int ty = y;
			int b = 0;
			while (tx >= 0 && tx < SIZE_X && ty >= 0 && ty < SIZE_Y) {
				if (!b && map[ty][tx] == '#') {
					b = 1;
					tx += dx;
					ty += dy;
					continue;
				}
				if (b) {
					map[ty][tx] = 'b';
				}
				tx += dx;
				ty += dy;
			}
		}
	}
	map[oy][ox] = 'O'; // also "unmark" the one we're counting from
}
int zap(int ox, int oy, char map[SIZE_Y][SIZE_X + 1], int which)
{
	struct asteroid asteroids[300]; // we happen to know 300 is enough... :D
	struct asteroid * ptrs[300];
	which--; // cause we input a one-indexed number. just because...

	mark_visible(ox, oy, map);

	// collect all visible asteroids  in a list
	// make a separate list if pointers to each asteroid, so we can sort the pointers and not the point structs
	int index = 0;
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (map[y][x] == '#') {
				int dx = x - ox;
				int dy = y - oy;
				asteroids[index].x = x;
				asteroids[index].y = y;
				asteroids[index].angle = atan2(dx, dy);
				ptrs[index] = &asteroids[index];

				index++;
			}
		}
	}
	// sort list by angle
	qsort(ptrs, index, sizeof(struct asteroid *), angle_comparator);

	// return whatever index the caller asked for (with 100*x+y encoding)
	return 100 * ptrs[which]->x + ptrs[which]->y;
}

int count_visible(int ox, int oy, char map[SIZE_Y][SIZE_X + 1])
{
	int count = 0;

	mark_visible(ox, oy, map);

	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (map[y][x] == '#') {
				count++;
			}
		}
	}

	return count;
}

int main(int argc, char * argv[])
{
	char omap[SIZE_Y][SIZE_X + 1];
	char tmap[SIZE_Y][SIZE_X + 1];

	(void) read(STDIN_FILENO, omap, SIZE_B);
	memcpy(tmap, omap, SIZE_B);

	int highest_visible = 0;
	int best_x = 0; int best_y = 0;
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (omap[y][x] != '#') continue;
			memcpy(tmap, omap, SIZE_B);
			int c = count_visible(x, y, tmap);
			if (c > highest_visible) {
				highest_visible = c;
				best_x = x;
				best_y = y;
			}
		}
	}
	printf("Part1: %d\n", highest_visible);

	memcpy(tmap, omap, SIZE_B);
	int twohundredth = zap(best_x, best_y, tmap, 200);
	printf("Part2: %d\n", twohundredth);
	return 0;
}
