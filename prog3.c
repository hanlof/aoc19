#include <stdio.h>
#include <stdlib.h>

struct line {
	int x; int y;
	char dir; int len;
	int index;
	int totsteps;
};

struct line line1[1024];
struct line line2[1024];

void printline(struct line l)
{
	printf("%d,%d %c%d\n", l.x, l.y, l.dir, l.len);
}

void read_line(struct line * line)
{
	char c;
	int i;
	int j = 0;
	int x = 0, y = 0;
	int totsteps = 0;
	scanf("%c%d", &c, &i);
	line[j].x = x;
	line[j].y = y;
	line[j].dir = c;
	line[j].len = i;
	line[j].index = j;
	line[j].totsteps = totsteps;
	switch (c) {
		case 'L': x = x - i; break;
		case 'R': x = x + i; break;
		case 'U': y = y - i; break;
		case 'D': y = y + i; break;
	}
	totsteps += i;
	j++;
	while (scanf(",%c%d", &c, &i) == 2) {
		line[j].x = x;
		line[j].y = y;
		line[j].dir = c;
		line[j].len = i;
		line[j].index = j;
		line[j].totsteps = totsteps;
		switch (c) {
			case 'L': x = x - i; break;
			case 'R': x = x + i; break;
			case 'U': y = y - i; break;
			case 'D': y = y + i; break;
		}
		totsteps += i;
		j++;
	}
	line[j].x = 0xFFFFFF;
	line[j].y = 0xFFFFFF;
}

int within(int min, int max, int p)
{
	if (p < min) return 0;
	return (p <= max);
}

struct line closest_man = { .len = 0xFFFFFF, .dir = 'X' };
struct line closest_tot = { .len = 0xFFFFFF, .dir = 'X' };

void match(struct line l1, struct line l2)
{
	int x, y;
	int totsteps = l1.totsteps + l2.totsteps;
	if (l1.dir == 'L' || l1.dir == 'R') {
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
		printf("man! %d,%d/%c%d\t%d,%d/%c%d\t@%d,%d [%d][%d]\n", l1.x, l1.y, l1.dir, l1.len, l2.x, l2.y, l2.dir, l2.len, x, y, l1.index, l2.index);
	}
	if (totsteps < closest_tot.len) {
		closest_tot.x = x;
		closest_tot.y = y;
		closest_tot.len = totsteps;
		printf("tot! %d,%d/%c%d\t%d,%d/%c%d\t@%d,%d [%d][%d]\n", l1.x, l1.y, l1.dir, l1.len, l2.x, l2.y, l2.dir, l2.len, x, y, l1.index, l2.index);
	}

}

void intersect(struct line l1, struct line l2)
{
	if ((l1.dir == 'L' || l1.dir == 'R') && (l2.dir == 'L' || l2.dir == 'R')) {
		return;
	}
	if ((l1.dir == 'U' || l1.dir == 'D') && (l2.dir == 'U' || l2.dir == 'D')) {
		return;
	}
	if (l1.dir == 'L') {
		if (within(l1.x - l1.len, l1.x, l2.x)) {
			if (l2.dir == 'D') {
				if (within(l2.y, l2.y + l2.len, l1.y)) {
					match(l1, l2);
				}
			}
			if (l2.dir == 'U') {
				if (within(l2.y - l2.len, l2.y, l1.y)) {
					match(l1, l2);
				}
			}
		}
	}
	if (l1.dir == 'R') {
		if (within(l1.x, l1.x + l1.len, l2.x)) {
			if (l2.dir == 'D') {
				if (within(l2.y, l2.y + l2.len, l1.y)) {
					match(l1, l2);
				}
			}
			if (l2.dir == 'U') {
				if (within(l2.y - l2.len, l2.y, l1.y)) {
					match(l1, l2);
				}
			}
		}

	}
	if (l1.dir == 'D') {
		if (within(l1.y, l1.y + l1.len, l2.y)) {
			if (l2.dir == 'L') {
				if (within(l2.x, l2.x - l2.len, l1.x)) {
					match(l1, l2);
				}
			}
			if (l2.dir == 'R') {
				if (within(l2.x, l2.x + l2.len, l1.x)) {
					match(l1, l2);
				}
			}
		}

	}
	if (l1.dir == 'U') {
		if (within(l1.y - l1.len, l1.y, l2.y)) {
			if (l2.dir == 'L') {
				if (within(l2.x, l2.x - l2.len, l1.x)) {
					match(l1, l2);
				}
			}
			if (l2.dir == 'R') {
				if (within(l2.x, l2.x + l2.len, l1.x)) {
					match(l1, l2);
				}
			}
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
			//printf("%d %d\n", i, j);
			intersect(line1[i], line2[j]);
			j++;
		}
		i++;
	}
	printline(closest_man);
	printline(closest_tot);
}
