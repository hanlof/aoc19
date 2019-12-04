#include <stdio.h>
#include <stdlib.h>

struct line {
	int x; int y;
	int len;
	int index; // for debugging
	int totsteps;
	int xdelta;
	int ydelta;
};

struct line line1[1024];
struct line line2[1024];

void read_line(struct line * line)
{
	char dir, slask;
	int len;
	int j = 0;
	int x = 0, y = 0;
	int totsteps = 0;

	while (scanf("%c%d%[,]", &dir, &len, &slask) >= 2) {
		line[j].x = x;
		line[j].y = y;
		line[j].len = len;
		line[j].index = j;
		line[j].totsteps = totsteps;
		switch (dir) {
			case 'L': x = x - len; line[j].xdelta = -len; break;
			case 'R': x = x + len; line[j].xdelta =  len; break;
			case 'U': y = y - len; line[j].ydelta = -len; break;
			case 'D': y = y + len; line[j].ydelta =  len; break;
		}
		totsteps += len;
		j++;
	}
	line[j].x = 0xFFFFFF;
	line[j].y = 0xFFFFFF;
}

struct line closest_man = { .len = 0xFFFFFF };
struct line closest_tot = { .len = 0xFFFFFF };

void match(struct line l1, struct line l2)
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
		closest_tot.x = x;
		closest_tot.y = y;
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
	if (p < min) return 0;
	return (p <= max);
}

void intersect(struct line l1, struct line l2)
{
	if (l1.ydelta == 0) {
		if (l2.ydelta == 0) return;
		if (within(l1.x, l1.x + l1.xdelta, l2.x) && within(l2.y, l2.y + l2.ydelta, l1.y)) {
			match(l1, l2);
		}
	} else { // if l1.ydelta != 0 then l1.xdelta must be 0, if input can be trusted to not have 0 length
		if (l2.xdelta == 0) return;
		if (within(l2.x, l2.x + l2.xdelta, l1.x) && within(l1.y, l1.y + l1.ydelta, l2.y)) {
			match(l1, l2);
		}
	}
}

int main(int argc, char * argv[])
{
	int w;
	int tot = 0;
	read_line(line1);
	scanf("\n");
	read_line(line2);

	int i = 0, j = 0;

	while (line1[i + 1].x != 0xFFFFFF) {
		j = 0;
		while (line2[j + 1].x != 0xFFFFFF) {
			intersect(line1[i], line2[j]);
			j++;
		}
		i++;
	}

	printf("Closest manhattan discance intersection: %d\n", abs(closest_man.x) + abs(closest_man.y));
	printf("Closest total line discance intersection %d\n", closest_tot.len);
}
