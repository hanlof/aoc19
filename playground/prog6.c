#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

struct rusage rusage;

struct planet {
	int par;
	int parents;
	int parents_last;
	int toyou;
	int tosan;
};

struct planet5 {
	struct planet5 * par;
	int parents;
	int parents_last;
	int toyou;
	int tosan;
};

struct planet6 {
	struct planet5 * par;
	int parents;
	int parents_last;
	int toyou;
	int tosan;
};

int hash(char * s)
{
	int i = 0;
	i += (s[0] - '0');
	i += (s[1] - '0') * 45;
	i += (s[2] - '0') * 45 * 45;
	return i;
}

void impl1()
{
	int tot = 0;
	char s[8];
	char t[8];
	int san = 0;
	int you = 0;
	struct planet * planets = calloc(100000, sizeof(struct planet));
	while (scanf("%[^)])%s\n", s, t) == 2) {
		int par = hash(s);
		int sat = hash(t);
		planets[sat].par = par;
		planets[sat].parents = 1;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') { you = sat; planets[par].toyou = 1; }
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') { san = sat; planets[par].tosan = 1; }
	}

	do {
		tot = 0;
		for (int i = 0; i < 100000; i++) {
			if (planets[i].par == 0) continue;
			int parents_parents = planets[planets[i].par].parents;
			if (planets[i].parents_last != parents_parents) {
				planets[i].parents = parents_parents + 1;
				planets[i].parents_last = parents_parents;
				tot++;
			}
		}
	} while (tot > 0);

	tot = 0;
	for (int i = 0; i < 100000; i++) {
		if (planets[i].par == 0) continue;
		int parents_parents = planets[planets[i].par].parents;
		if (parents_parents != (planets[i].parents + 1)) {
		}
		tot += planets[i].parents;
	}
	printf("Part1: %d\n", tot);
	for (int i = you; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].toyou = planets[i].toyou + 1;
	}
	for (int i = san; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].tosan = planets[i].tosan + 1;
		if (planets[i].toyou != 0) {
			printf("Part2: %d\n", planets[i].tosan + planets[i].toyou - 2);
			break;
		}
	}
	/*
		$ ../../configs/c-programs/perf.py -i input6 -o /dev/null bin/prog6
		21 - S2.9 U170.7 R174.5 CPU99.5
	*/

}

void impl2()
{
	int tot = 0;
	char s[8];
	char t[8];
	int san = 0;
	int you = 0;
	struct planet * planets = calloc(92000, sizeof(struct planet));
	while (scanf("%[^)])%s\n", s, t) == 2) {
		int par = hash(s);
		int sat = hash(t);
		planets[sat].par = par;
		planets[sat].parents = 1;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') { you = sat; planets[par].toyou = 1; }
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') { san = sat; planets[par].tosan = 1; }
	}

	// try to reduce empty slots
	int last = 92000;
	for (int i = 0; i < 92000; i++) {
		while (planets[i].par != 0) {
			i++;
		}
		if (i > 92000) break;
		int j = i;
		while (planets[j].par == 0) {
			j++;
			if (j > 92000) break;
		}
		//printf("j:%d i:%d\n", j, i);
		if (j > 92000) { break; }
		// now we have i pointing at first empty and j pointing at smth to be moved

		planets[i] = planets[j];
		planets[j].par = 0;
		//printf("move %d to %d\n", j, i);
		for (int k = 0; k < 92000; k++) {
			last = i;
			if (planets[k].par == j) planets[k].par = i;
		}
	}

	do {
		tot = 0;
		for (int i = 0; i < last; i++) {
			if (planets[i].par == 0) continue;
			int parents_parents = planets[planets[i].par].parents;
			if (planets[i].parents_last != parents_parents) {
				planets[i].parents = parents_parents + 1;
				planets[i].parents_last = parents_parents;
				tot++;
			}
		}
	} while (tot > 0);

	tot = 0;
	for (int i = 0; i < 92000; i++) {
		if (planets[i].par == 0) continue;
		int parents_parents = planets[planets[i].par].parents;
		tot += planets[i].parents;
	}
	printf("Part1: %d\n", tot);
	for (int i = you; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].toyou = planets[i].toyou + 1;
	}
	for (int i = san; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].tosan = planets[i].tosan + 1;
		if (planets[i].toyou != 0) {
			printf("Part2: %d\n", planets[i].tosan + planets[i].toyou - 2);
			break;
		}
	}
	/*
		$ ../../configs/c-programs/perf.py -i input6 -o /dev/null bin/prog6
		138075 - S2.7 U19.0 R22.8 CPU95.3
	*/

}

void impl3()
{
	int tot = 0;
	char s[8];
	char t[8];
	int san = 0;
	int you = 0;
	struct planet * planets = calloc(100000, sizeof(struct planet));
	while (scanf("%[^)])%s\n", s, t) == 2) {
		int par = hash(s);
		int sat = hash(t);
		planets[sat].par = par;
		planets[sat].parents = 1;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') { you = sat; planets[par].toyou = 1; }
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') { san = sat; planets[par].tosan = 1; }
	}
	//getrusage(RUSAGE_SELF, &rusage);
	//printf("MARK1 %d %d\n", rusage.ru_utime.tv_usec, rusage.ru_stime.tv_usec);

	do {
		tot = 0;
		for (int i = 0; i < 100000; i++) {
			struct planet * this = &planets[i];
			if (this->par == 0) { continue; }
			struct planet * par = &planets[this->par];
			if (this->parents_last != par->parents) {
				this->parents = par->parents + 1;
				this->parents_last = par->parents;
				tot++;
			}
		}
	} while (tot > 0);

	//getrusage(RUSAGE_SELF, &rusage);
	//printf("MARK2 %d %d\n", rusage.ru_utime.tv_usec, rusage.ru_stime.tv_usec);

	tot = 0;
	for (int i = 0; i < 100000; i++) {
		struct planet * this = &planets[i];
		if (this->par == 0) continue;
		tot += this->parents;
	}
	printf("Part1: %d\n", tot);
	for (int i = you; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].toyou = planets[i].toyou + 1;
	}
	for (int i = san; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].tosan = planets[i].tosan + 1;
		if (planets[i].toyou != 0) {
			printf("Part2: %d\n", planets[i].tosan + planets[i].toyou - 2);
			break;
		}
	}
	/*
	*/

}

void impl4()
{
	int tot = 0;
	char s[8];
	char t[8];
	int san = 0;
	int you = 0;
	int entries[2000];
	int nentries;
	struct planet * planets = calloc(100000, sizeof(struct planet));
	while (scanf("%[^)])%s\n", s, t) == 2) {
		int par = hash(s);
		int sat = hash(t);
		planets[sat].par = par;
		planets[sat].parents = 1;
		entries[nentries++] = sat;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') { you = sat; planets[par].toyou = 1; }
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') { san = sat; planets[par].tosan = 1; }
	}

	do {
		tot = 0;
		for (int i = 0; i < nentries; i++) {
			struct planet * this = &planets[entries[i]];
			if (this->par == 0) { continue; }
			struct planet * par = &planets[this->par];
			if (this->parents_last != par->parents) {
				this->parents = par->parents + 1;
				this->parents_last = par->parents;
				tot++;
			}
		}
	} while (tot > 0);

	tot = 0;
	for (int i = 0; i < nentries; i++) {
		struct planet * this = &planets[entries[i]];
		if (this->par == 0) continue;
		tot += this->parents;
	}
	printf("Part1: %d\n", tot);
	for (int i = you; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].toyou = planets[i].toyou + 1;
	}
	for (int i = san; planets[i].par != 0; i = planets[i].par) {
		planets[planets[i].par].tosan = planets[i].tosan + 1;
		if (planets[i].toyou != 0) {
			printf("Part2: %d\n", planets[i].tosan + planets[i].toyou - 2);
			break;
		}
	}
}

void impl5()
{
	int tot = 0;
	char s[8];
	char t[8];
	int san = 0;
	int you = 0;
	int entries[2000];
	int nentries = 0;
	struct planet5 * planets = calloc(100000, sizeof(struct planet5));

	// read that input
	while (scanf("%[^)])%s\n", s, t) == 2) {
		int par = hash(s);
		int sat = hash(t);
		planets[sat].par = &planets[par];
		planets[sat].parents = 1;
		entries[nentries++] = sat;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') { you = sat; planets[par].toyou = 1; }
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') { san = sat; planets[par].tosan = 1; }
	}

	// traverse all planets. increase parents on those whose parent has been modified.
	// keep traversing all planets over and over until nothing needs updating
	do {
		tot = 0;
		for (int i = 0; i < nentries; i++) {
			struct planet5 * this = &planets[entries[i]];
			if (this->par == 0) { continue; }
			struct planet5 * par = this->par;
			if (this->parents_last != par->parents) {
				this->parents = par->parents + 1;
				this->parents_last = par->parents;
				tot++;
			}
		}
	} while (tot > 0);

	// count 'em
	tot = 0;
	for (int i = 0; i < nentries; i++) {
		struct planet5 * this = &planets[entries[i]];
		if (this->par == 0) continue;
		tot += this->parents;
	}
	printf("Part1: %d\n", tot);

	for (struct planet5 * i = planets[you].par; i->par != 0; i = i->par) {
		i->par->toyou = i->toyou + 1;
	}
	for (struct planet5 * i = planets[san].par; i->par != 0; i = i->par) {
		i->par->tosan = i->tosan + 1;
		if (i->toyou != 0) {
			printf("Part2: %d\n", i->tosan + i->toyou - 2);
			break;
		}
	}
}

void impl6()
{
	int tot = 0;
	char s[8];
	char t[8];
	int san = 0;
	int you = 0;
	int entries[2000];
	int nentries;
	struct planet5 * planets = calloc(100000, sizeof(struct planet5));
	while (scanf("%[^)])%s\n", s, t) == 2) {
		int par = hash(s);
		int sat = hash(t);
		planets[sat].par = &planets[par];
		planets[sat].parents = 1;
		entries[nentries++] = sat;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') { you = sat; planets[par].toyou = 1; }
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') { san = sat; planets[par].tosan = 1; }
	}

	do {
		tot = 0;
		for (int i = 0; i < nentries; i++) {
			struct planet5 * this = &planets[entries[i]];
			if (this->par == 0) { continue; }
			struct planet5 * par = this->par;
			if (this->parents_last != par->parents) {
				this->parents = par->parents + 1;
				this->parents_last = par->parents;
				tot++;
			}
		}
	} while (tot > 0);

	tot = 0;
	for (int i = 0; i < nentries; i++) {
		struct planet5 * this = &planets[entries[i]];
		if (this->par == 0) continue;
		tot += this->parents;
	}
	printf("Part1: %d\n", tot);
	for (struct planet5 * i = &planets[you]; i->par != 0; i = i->par) {
		i->par->toyou = i->toyou + 1;
	}
	for (struct planet5 * i = &planets[san]; i->par != 0; i = i->par) {
		i->par->tosan = i->tosan + 1;
		if (i->toyou != 0) {
			printf("Part2: %d\n", i->tosan + i->toyou - 2);
			break;
		}
	}
}

int main(int argc, char * argv[])
{
	// * no planet (including root) has more than 2 children/"orbiters"
	// * there is only one "root" planet. not multiple.

	// impl1()  ~22ms   - keep big holes in the hash table and iterate every entry every time anyways
	// impl2()  ~150+ms - squeeze input data from 91k to 1232 entries (XXX CURRENTLY BUGGY, wrong output)
	// impl3()  ~21.8ms - use struct pointers instead of referencing planets[i] so much
	// impl4()  ~4.7ms  - keep a table of valid entries. better way of avoiding traversing so much data
	// impl5()  ~4.5ms  - same as impl4 with parent as a pointer

	impl5();
}
