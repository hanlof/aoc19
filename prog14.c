#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct conv {
	char * name;
	int n;
	struct conv * inp[16];
	int64_t cost[16];

	struct conv * outp[16];
	int64_t requested[16];
	int64_t leftover;
};

int maxconv = 0;
struct conv o_convs[100];
struct conv convs[100];
struct conv * findconv(char * s)
{
	for (int i = 0; i < maxconv; i++) {
		if (!strcmp(s, convs[i].name)) {
			return &convs[i];
		}
	}
	struct conv * c = &convs[maxconv++];
	c->name = s;
	c->inp[0] = NULL;
	c->outp[0] = NULL;
	c->leftover = 0;
	c->requested[0] = -2;
	return c;

}

void printconv(struct conv * c)
{
	printf("XXX %s/%d\n  ", c->name, c->n);
	for (int j = 0; c->inp[j] != NULL; j++) {
		printf("%s/%ld, ", c->inp[j]->name, c->cost[j]);
	}
	printf("\n");
}

void addinput(struct conv * c, char * name, int cost)
{
	int i = 0;
	struct conv * c2 = findconv(name);
	while (c->inp[i] != NULL) i++;
	c->inp[i] = c2;
	c->cost[i] = cost;
	c->inp[++i] = NULL;
	i = 0;
	while (c2->outp[i] != NULL) i++;
	c2->outp[i] = c;
	c2->requested[i] = -1;
	c2->outp[++i] = NULL;
	c2->requested[i] = -2;
}

int64_t calculate_for(int amount_of_fuel)
{
	struct conv * fuel = findconv("FUEL");
	struct conv * ore = findconv("ORE");
	fuel->requested[0] = amount_of_fuel;
	fuel->requested[1] = -2;
	ore->n = 1;
	int found_smth = 1;
	while (found_smth == 1) {
		found_smth = 0;
		//printf("==========\n");
		for (int i = 0; i < maxconv; i++) {
			int undetermined_reqs = 0;

			int j = 0;
			while (convs[i].outp[j] != NULL) {
				if (convs[i].requested[j] == -1) undetermined_reqs += 1;
				j++;
			}
			//printf("%s %d\n", convs[i].name, undetermined_reqs);
			if (undetermined_reqs != 0) { found_smth = 1; continue; }

			// find total requested from this node
			j = 0;
			int64_t tot_requested = 0;
			while (convs[i].requested[j] != -2) {
				tot_requested += convs[i].requested[j];
				j++;
			}

			int64_t conversions = tot_requested / convs[i].n;
			while (conversions * convs[i].n < tot_requested) { conversions++; }

			if (!strcmp(convs[i].name, "ORE")) return conversions;

			j = 0;
			while (convs[i].inp[j] != NULL) {
				struct conv * in = convs[i].inp[j];
				int k = 0;
				while (in->outp[k] != &convs[i]) k++;
				in->requested[k] = conversions * convs[i].cost[j];
				j++;
			}

		}
		//printf("updates %d\n", found_smth);
	}
	return -1;
}

int main(int argc, char * argv[])
{
	// read input
	char * s1;
	char * s2;
	int i;
	printf("digraph g {\n");
	while (scanf("%m[^=]=> %d %ms\n", &s1, &i, &s2) > 0) {
		//printf("|%s|%d %s|\n", s1, i, s2);
		int j;
		char * s3;
		char slask[10];
		int n;
		//printf("|%s|%d:%s\n", s1, i, s2);

		struct conv * c = findconv(s2);
		c->n = i;
		printf("%s [label=\"%s %d\"];\n", s2, s2, i);
		while (sscanf(s1, "%d %m[^, ]%[,]%n", &j, &s3, slask, &n) == 3) {
			addinput(c, s3, j);
			s1 += n;
			printf("%s -> %s [label=%d];\n", c->name,  s3, j);
		}
		addinput(c, s3, j); // when loop exits we still got j and s3
		printf("%s -> %s [label=%d];\n", c->name,  s3, j);
		//printf("%d *>> %d:%s\n", tmp, j, s3);
	}
	printf("}\n");
	memcpy(o_convs, convs, sizeof(convs));


	/*
	for (int i = 0; i < maxconv; i++) {
		printconv(&convs[i]);
	}
	*/

	printf("Part1: %ld\n", calculate_for(1));
	memcpy(convs, o_convs, sizeof(convs));
	printf("Part1: %ld\n", calculate_for(1));
	memcpy(convs, o_convs, sizeof(convs));
	printf("Part1: %ld\n", calculate_for(1));
	memcpy(convs, o_convs, sizeof(convs));

	// Part 2: just try numbers by hand... :-D
	// 4906797 this is the lowest number that gives > 1 trillion

	printf("Part2: %ld\n", calculate_for(4906796)); 
	printf("     : 1000000000000\n");

	return 0;
}
