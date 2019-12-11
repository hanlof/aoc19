#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


#define SIZE_X 26
#define SIZE_Y 26
#define SIZE_B (SIZE_Y * (SIZE_X + 1))

struct p {
	int x;
	int y;
	double angle;
};

int comp(const void * x, const void * y)
{
	struct p * a = *((struct p **)x);
	struct p * b = *((struct p **)y);
	return a->angle < b->angle;
}


int zap(int ox, int oy, char map[SIZE_Y][SIZE_X + 1])
{
	map[25][26] = '\0';
	struct p points[300];
	struct p * ptrs[300];
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
	map[oy][ox] = 'O';
	int count = 0;
	int index = 0;
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (map[y][x] == '#') {
				points[index].x = x;
				points[index].y = y;
				int dx = x - ox;
				int dy = y - oy;
				points[index].angle = atan2(dx, dy);
				ptrs[index] = &points[index];

				index++;
				//printf("%f %d,%d\n", atan2(dx, dy), x, y);
			}
		}
	}
	qsort(ptrs, index, sizeof(struct p *), comp);
	for (int i = i; i < index; i++) {
		printf("%d: %d, %f\n", i, ptrs[i]->x * 100 + ptrs[i]->y, ptrs[i]->angle);
	}

	return count;
}

int check(int ox, int oy, char map[SIZE_Y][SIZE_X + 1])
{
	map[25][26] = '\0';
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
	map[oy][ox] = 'O';
	int count = 0;
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (map[y][x] == '#') {
				count++;
			}
		}
	}

//	printf("%s\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", &map[0][0]);
	return count;
}

int main(int argc, char * argv[])
{
	char omap[SIZE_Y][SIZE_X + 1];
	char tmap[SIZE_Y][SIZE_X + 1];

	int ret = read(STDIN_FILENO, omap, SIZE_B);
	(void) ret;
	memcpy(tmap, omap, SIZE_B);
	int count = 0;
	int e = 0;
	for (int i = 0; i < SIZE_B; i++) {
		if (((char*)omap)[i] == '#') count++;
		if (((char*)omap)[i] == '.') e++;
	}


	int detectable = 0;
	int best_x = 0; int best_y = 0;
	for (int y = 0; y < SIZE_Y; y++) {
		for (int x = 0; x < SIZE_X; x++) {
			if (omap[y][x] != '#') continue;
			memcpy(tmap, omap, SIZE_B);
			int c = check(x, y, tmap);
			if (c > detectable) {
				detectable = c;
				best_x = x;
				best_y = y;
			}
		}
	}
	printf(">> %d %d,%d\n", detectable, best_x, best_y);
	memcpy(tmap, omap, SIZE_B);
	printf("%d\n", zap(best_x, best_y, tmap));
	printf("%s\nYYYYYYYYYYYYYY\n", &tmap[0][0]);
	return 0;
}
