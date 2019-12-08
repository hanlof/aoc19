#include <stdio.h>
#include <stdlib.h>

struct planet {
	struct planet * par;
	int parents;
	int parents_last;        // used as marker when we update all total orbit counts
	int to_my_planet;        // only used for part2
	int to_santas_planet;    // only used for part2
};

#define HASH(x, y, z) ( 45 * 45 * (x - '0') + 45 * (y - '0') + (z - '0') )

int main(int argc, char * argv[])
{
	struct planet * santas_planet = 0; // for part 2. stores where santa is
	struct planet * my_planet = 0;     // for part 2. stores where I (YOU) am
	struct planet * entries[1300];     // there are 1233 valid entries. 1300 cause why not?!
	int nentries = 0;
	struct planet * planets = calloc(100000, sizeof(struct planet));
	int tmp = 0;
	char s[8];
	char t[8];

	// get that input, put all planets in the big table (planets) that has space for the whole hash space
	// also make a compact table (entries) that holds pointers to all used entries in the big table
	while (scanf("%[^)])%s\n", s, t) == 2) {
		struct planet * par = &planets[HASH(s[0], s[1], s[2])];
		struct planet * sat = &planets[HASH(t[0], t[1], t[2])];
		sat->par = par;
		sat->parents = 1;
		entries[nentries++] = sat;
		if (t[0] == 'Y' && t[1] == 'O' && t[2] == 'U') {
			my_planet = sat;
			par->to_my_planet = 1;
		}
		if (t[0] == 'S' && t[1] == 'A' && t[2] == 'N') {
			santas_planet = sat;
			par->to_santas_planet = 1;
		}
	}

	// traverse all planets. increase parents on those whose parent has been modified.
	// keep traversing all planets over and over until nothing needs updating
	do {
		tmp = 0;
		for (int i = 0; i < nentries; i++) {
			struct planet * this = entries[i];
			struct planet * par = this->par;
			if (this->parents_last != par->parents) {
				this->parents_last = par->parents;
				this->parents = par->parents + 1;
				tmp++;
			}
		}
	} while (tmp > 0);

	// sum up the parents/"orbits"
	tmp = 0;
	for (int i = 0; i < nentries; i++) {
		struct planet * this = entries[i];
		tmp += this->parents;
	}
	printf("Part1: %d\n", tmp);

	// add a count to everything from my_planet to "root"
	for (struct planet * i = my_planet->par; i->par != 0; i = i->par) {
		i->par->to_my_planet = i->to_my_planet + 1;
	}
	// traverse from santas_planet through all parents until we find a path to my_planet
	for (struct planet * i = santas_planet->par; i->par != 0; i = i->par) {
		i->par->to_santas_planet = i->to_santas_planet + 1;
		if (i->to_my_planet != 0) {
			printf("Part2: %d\n", i->to_santas_planet + i->to_my_planet - 2);
			break;
		}
	}
}
