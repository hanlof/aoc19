#include <stdio.h>
#include <stdlib.h>

#define END_MARKER 0xFFFFFF

struct segment {
	int x; int y;
	int len;
	int index; // for debugging
	int totsteps;
	int xdelta;
	int ydelta;
};

void read_line(struct segment * seg)
{
	char dir, slask;
	int len;
	int j = 0;
	int x = 0, y = 0;
	int totsteps = 0;

	while (scanf("%c%d%[,]", &dir, &len, &slask) >= 2) {
		seg[j].x = x;
		seg[j].y = y;
		seg[j].len = len;
		seg[j].index = j;
		seg[j].totsteps = totsteps;
		switch (dir) {
			case 'L': x = x - len; seg[j].xdelta = -len; break;
			case 'R': x = x + len; seg[j].xdelta =  len; break;
			case 'U': y = y - len; seg[j].ydelta = -len; break;
			case 'D': y = y + len; seg[j].ydelta =  len; break;
		}
		totsteps += len;
		j++;
	}
	seg[j].x = END_MARKER;
	seg[j].y = END_MARKER;
}

struct segment closest_man = { .len = END_MARKER };
struct segment closest_tot = { .len = END_MARKER };

void check_closest(struct segment l1, struct segment l2)
{
	int x, y;
	int totsteps = l1.totsteps + l2.totsteps;
	if (l1.ydelta == 0) {
		x = l2.x;
		y = l1.y;
		totsteps += abs(x - l1.x);
		totsteps += abs(y - l2.y);
	} else {
		x = l1.x;
		y = l2.y;
		totsteps += abs(x - l2.x);
		totsteps += abs(y - l1.y);
	}

	int dist = abs(x) + abs(y);
	if (dist < closest_man.len) {
		closest_man.x = x;
		closest_man.y = y;
		closest_man.len = dist;
		//printf("man! %d,%d/%d\t%d,%d/%d\t@%d,%d [%d][%d]\n", l1.x, l1.y, l1.len, l2.x, l2.y, l2.len, x, y, l1.index, l2.index);
	}
	if (totsteps < closest_tot.len) {
		closest_tot.len = totsteps;
		//printf("tot! %d,%d/%d\t%d,%d/%d\t@%d,%d [%d][%d]\n", l1.x, l1.y, l1.len, l2.x, l2.y, l2.len, x, y, l1.index, l2.index);
	}
}

int within(int p1, int p2, int p)
{
	int min, max;
	if (p1 < p2) {
		min = p1;
		max = p2;
	} else {
		min = p2;
		max = p1;
	}
	if (p < min) {
		return 0;
	}
	return (p <= max);
}

void check_intersection(struct segment l1, struct segment l2)
{
	if (l1.ydelta == 0) {
		if (l2.ydelta == 0) return;
		if (within(l1.x, l1.x + l1.xdelta, l2.x) && within(l2.y, l2.y + l2.ydelta, l1.y)) {
			check_closest(l1, l2);
		}
	} else { // if l1.ydelta != 0 then l1.xdelta must be 0, if input can be trusted to not have 0 length
		if (l2.xdelta == 0) return;
		if (within(l2.x, l2.x + l2.xdelta, l1.x) && within(l1.y, l1.y + l1.ydelta, l2.y)) {
			check_closest(l1, l2);
		}
	}
}

int main(int argc, char * argv[])
{
	int w;
	int tot = 0;
	struct segment line1[1024];
	struct segment line2[1024];

	read_line(line1);
	scanf("\n");
	read_line(line2);

	for (int i = 0; line1[i].x != END_MARKER; i++) {
		for (int j = 0; line2[j].x != END_MARKER; j++) {
			check_intersection(line1[i], line2[j]);
		}
	}

	printf("Closest manhattan discance intersection: %d\n", abs(closest_man.x) + abs(closest_man.y));
	printf("Closest total line discance intersection %d\n", closest_tot.len);
}
