#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct moon {
	int x;
	int y;
	int z;
	int dx;
	int dy;
	int dz;
};

void gravity(struct moon * a, struct moon * b)
{
	int diff = a->x - b->x;
	if (diff) {
		a->dx += diff < 0 ? 1 : -1;
		b->dx += diff > 0 ? 1 : -1;
	}
	diff = a->y - b->y;
	if (diff) {
		a->dy += diff < 0 ? 1 : -1;
		b->dy += diff > 0 ? 1 : -1;
	}
	diff = a->z - b->z;
	if (diff) {
		a->dz += diff < 0 ? 1 : -1;
		b->dz += diff > 0 ? 1 : -1;
	}
}

int energy(struct moon * m)
{
	int pot = abs(m->x) + abs(m->y) + abs(m->z);
	int kin = abs(m->dx) + abs(m->dy) + abs(m->dz);
	return pot * kin;
}
void step(struct moon * moons)
{
		gravity(&moons[0], &moons[1]);
		gravity(&moons[0], &moons[2]);
		gravity(&moons[0], &moons[3]);
		gravity(&moons[1], &moons[2]);
		gravity(&moons[1], &moons[3]);
		gravity(&moons[2], &moons[3]);
		for (int i = 0; i < 4; i++) {
			moons[i].x += moons[i].dx;
			moons[i].y += moons[i].dy;
			moons[i].z += moons[i].dz;
		}
}

int compare(struct moon * a, struct moon * b)
{
	/*
	printf(">>> %d %d %d - %d %d %d ---- ", a[0].x, a[0].y, a[0].z, a[0].dx, a[0].dy, a[0].dz);
	printf("%d %d %d - %d %d %d\n",     b[0].x, b[0].y, b[0].z, b[0].dx, b[0].dy, b[0].dz);
	printf("    %d %d %d - %d %d %d ---- ", a[1].x, a[1].y, a[1].z, a[1].dx, a[1].dy, a[1].dz);
	printf("%d %d %d - %d %d %d\n",     b[1].x, b[1].y, b[1].z, b[1].dx, b[1].dy, b[1].dz);
	printf("    %d %d %d - %d %d %d ---- ", a[2].x, a[2].y, a[2].z, a[2].dx, a[2].dy, a[2].dz);
	printf("%d %d %d - %d %d %d\n",     b[2].x, b[2].y, b[2].z, b[2].dx, b[2].dy, b[2].dz);
	printf("    %d %d %d - %d %d %d ---- ", a[3].x, a[3].y, a[3].z, a[3].dx, a[3].dy, a[3].dz);
	printf("%d %d %d - %d %d %d\n",     b[3].x, b[3].y, b[3].z, b[3].dx, b[3].dy, b[3].dz);
	*/
	for (int i = 0; i < 4; i++) {
		if (a[i].x != b[i].x) goto not_equal;
		if (a[i].y != b[i].y) goto not_equal;
		if (a[i].z != b[i].z) goto not_equal;
		printf("HIT %i!\n", i);
		if (a[i].dx != b[i].dx) goto not_equal;
		if (a[i].dy != b[i].dy) goto not_equal;
		if (a[i].dz != b[i].dz) goto not_equal;
	}
	return 1;
not_equal:
	return 0;
}

void easy()
{
	struct moon easy_moons[4] = {
		{ .x=-1,.y=0,  .z=2 },
		{ .x=2, .y=-10,.z=-7 },
		{ .x=4, .y=-8, .z=8 },
		{ .x=3, .y=5,  .z=-1 }
	};
	struct moon o_easy_moons[4];
	memcpy(o_easy_moons, easy_moons, sizeof(easy_moons)); // save original state

	printf("compare moons to omoons %d\n", compare(o_easy_moons, easy_moons));
	step(&easy_moons[0]);
	printf("compare moons to omoons %d\n", compare(o_easy_moons, easy_moons));
	int64_t j = 1;
	//struct timeval tv;
	for (; !compare(o_easy_moons, easy_moons); step(&easy_moons[0]), j++) {
		if (0 == (j % 10000000)) {
			printf("Iter %ld\n", j);
		}
	}
	printf("%ld\n", j);
}

struct moon old_moons[1000000][4];

int main(int argc, char * argv[])
{
	struct moon moons[4] = {
		{ .x =  1,  .y = -4, .z =  3,  .dx = 0, .dy = 0, .dz = 0 },
		{ .x = -14, .y =  9, .z = -4,  .dx = 0, .dy = 0, .dz = 0 },
		{ .x = -4,  .y = -6, .z =  7 , .dx = 0, .dy = 0, .dz = 0 },
		{ .x =  6,  .y = -9, .z = -11, .dx = 0, .dy = 0, .dz = 0  }
	};
	struct moon o_moons[4];
	memcpy(o_moons, moons, sizeof(moons)); // save original state
	for (int i = 1000; i; i--) {
		step(moons);
	}
	printf("Part1: %d\n", energy(&moons[0]) + energy(&moons[1]) + energy(&moons[2]) + energy(&moons[3]));

	memcpy(moons, o_moons, sizeof(moons)); // restore original state
	memcpy(&old_moons[0][0], &moons[0], sizeof(moons));
	//struct timeval tv;

	int64_t i = 1;
	int max_x = 10;
	int min_x = -10;
	int max_y = 10;
	int min_y = -10;
	//int max_y = 0;
	for (; i < 50l * 1000l * 1000l * 1000l; i++) {
		if (0 == (i % (100l * 1000l * 1000l))) {
			printf("%ld\n", i);
		}
		step(&moons[0]);
	}
	for ( ; /* !compare(o_moons, moons)*/ ; ) {
		step(&moons[0]);
		//memcpy(&old_moons[i][0], &moons[0], sizeof(moons));
		//printf("\e[0m\e[2J");
		for (int j = 0; j < 4; j++) {
			if (moons[j].x > max_x) max_x = moons[j].x;
			if (moons[j].x < min_x) min_x = moons[j].x;
			if (moons[j].y > max_y) max_y = moons[j].y;
			if (moons[j].y < min_y) min_y = moons[j].y;
			double xpos = 100.0 + (200.0 * moons[j].x) / (double)(max_x - min_x);
			double ypos = 30.0 + (100.0 * moons[j].y) / (double)(max_y - min_y);
			printf("\e[%i;%iH\e[4%im ", (int)ypos, (int)xpos, j+2);
		}
		printf("\e[1;1H%ld: %d, %d\n", i, min_x, max_x);
		//usleep(1000 * 15);
		int equal = 0;
	/*	for (int j = i - 1; j >= 0; j--) {
			if (compare(&moons[0], &old_moons[j][0])) {
				equal = 1;
				break;
			}
		}
	*/
		if (equal) break;
		if (0 == (i % 10000)) {
		//	printf("Iter %ld\n", i);
		printf(">>>>> %d %d %d\n", min_x, max_x, moons[0].x);
		}
		i++;
		//printf(">>>>>>\n");
	}
	printf("%ld\n", i);

	return 0;
}
