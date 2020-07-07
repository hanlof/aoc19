#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//#define DECK_SIZE (10007)
#define PART2_DECK_SIZE     (119315717514047L)
#define DECK_SIZE PART2_DECK_SIZE
//#define PART2_DECK_SIZE (10007)

#define PART2_SHUFFLE_TIMES (101741582076661L)

//#define PART2_DECK_SIZE (100)
//#define DECK_SIZE (100)

int64_t (*flist[200])(int64_t, int64_t) = { [0 ... 199] = NULL };
int64_t alist[200] = { [0 ... 199] = 0 };

// return position of card m after cut n
int64_t cut(int64_t n, int64_t m)
{
	//printf("cut %ld %ld\n", n, m);
	if (n < 0) n = DECK_SIZE + n;
	if (n < 0 || n > DECK_SIZE) { printf("*** cut %ld %ld\n", n, m); exit(1); }

	//printf(" ==> %ld\n", (DECK_SIZE + m - n) % DECK_SIZE);

	return (DECK_SIZE + m - n) % DECK_SIZE;
}

// return position of card m after order is reversed
int64_t dealinto(int64_t n, int64_t m)
{
	return (DECK_SIZE - 1 - m);
}

// return position of card m after incremental deal n
int64_t dealinc(int64_t n, int64_t m)
{

	//printf("deal incr %ld %ld\n", n, m);
	//return dealinto(0, (n * m) % DECK_SIZE);
	//printf(" ==> %ld\n", (n * m) % DECK_SIZE);
	return (n * m) % DECK_SIZE;
}

// what was the previous position of the card that is at m after cut n
int64_t cut_X(int64_t n, int64_t m)
{
	//printf("cut %ld %ld\n", n, m);
	if (n < 0) n = PART2_DECK_SIZE + n;
	//printf("... %ld %ld\n", n, m);

	return (PART2_DECK_SIZE + m + n) % PART2_DECK_SIZE;
}

// what position is put into position n after reverse order
int64_t dealinto_X(int64_t n, int64_t m)
{
	//printf("dealinto %ld %ld\n", n, m);
	return (PART2_DECK_SIZE - 1 - m);
}

long long moduloMultiplication(long long a,
                            long long b,
                            long long mod)
{
    long long res = 0; // Initialize result

    // Update a if it is more than
    // or equal to mod
    a %= mod;

    while (b)
    {
        // If b is odd, add a with result
        if (b & 1)
            res = (res + a) % mod;

        // Here we assume that doing 2*a
        // doesn't cause overflow
        a = (2 * a) % mod;

        b >>= 1; // b = b / 2
    }

    return res;
}

// what position is put into position n after deal incremental
int64_t dealinc_X(int64_t n, int64_t m)
{
	// numbers below 77000 can be (signed) multiplied by PART2_DECK_SIZE without overflow

	// lets just make a table.... highest n is 69.
	int64_t lookup[n * 2];
	int64_t step = n - (PART2_DECK_SIZE % n);

	for (int i = 0; i < n; i++) {
		lookup[((i * step) % n)] = i;
	}
	int64_t pos_before_mod = lookup[m % n] * PART2_DECK_SIZE;
	/*
	if (0x7fffffffffffffff / PART2_DECK_SIZE < which_lap) {
		printf("overflow %ld %ld\n", which_lap, PART2_DECK_SIZE);
		exit(0);
	}
	*/
	pos_before_mod += m;
	int64_t res = pos_before_mod / n;
//	if (pos_before_mod % n != 0) { printf("XXXXXX\n"); exit(0); }
	return res;
}

#define INITIAL_STEPS 420000L
//#define INITIAL_STEPS 1L

int main(int argc, char * argv[])
{

	/*
	for (int i = 0; i < PART2_DECK_SIZE; i++)
	{
		printf("cut_X((cut(%d)): %ld\n", i, dealinc_X(3, dealinc(3, i)));
		printf("cut((cut_X(%d)): %ld\n", i, dealinc(3, dealinc_X(3, i)));
	//	dealinc_X(3, i);
	}
	return 0;
	*/
	char s[100];
	int64_t t;
	int n_funcs = 0;
	while (1 == scanf("%[^\n]\n", s)) {
		if      (    !strcmp(s, "deal into new stack"))         { alist[n_funcs] = 0; flist[n_funcs] = dealinto; }
		else if (1 == sscanf(s, "deal with increment %ld", &t)) { alist[n_funcs] = t; flist[n_funcs] = dealinc; }
		else if (1 == sscanf(s, "cut %ld", &t))                 { alist[n_funcs] = t; flist[n_funcs] = cut; }
		n_funcs++;
	}
	n_funcs--;

	int64_t n = 2019;
	for (int64_t j = 0; j <= n_funcs; j++) {
	//	printf("%ld => ", n);
		n = flist[j](alist[j], n);
	//	printf("%ld (%ld)\n", n, alist[j]);
	}
	printf("Part1: %ld\n", n);

	for (int i = 0; i <= n_funcs; i++) {
		if (flist[i] == cut) { flist[i] = cut_X; }
		if (flist[i] == dealinc) flist[i] = dealinc_X;
		if (flist[i] == dealinto) flist[i] = dealinto_X;
	}

	for (int i = n_funcs; i >= 0; i--) {
	//	printf("%ld => ", n);
		n = flist[i](alist[i], n);
	//	printf("%ld (%ld)\n", n, alist[i]);
	}
	printf("n: %ld\n", n);
//	return 0;


	n = 2020;
	uint64_t prev = n;
	uint64_t prevprev = prev;
	for (int j = 0; j < 10000000; j++) {
		for (int i = n_funcs; i >= 0; i--) {
			n = flist[i](alist[i], n);
		}

	/*	if (n < 10000000) {
			printf("<<<<< %ld %ld\n", n, j);
			exit(0);
		} */
		if (n > (PART2_DECK_SIZE - 10000000)) {
			printf("<<<<< %ld %ld\n", n, j);
			exit(0);
		}
		//printf("n:%15ld step:%15ld stepdelta:%ld\n", n, n-prev, (n- prev)-(prev -prevprev));
		prevprev = prev;
		prev = n;
	}

	exit(0);
	n = 2020;
	for (int j = 0; j < INITIAL_STEPS; j++) {
		for (int i = n_funcs; i >= 0; i--) {
			n = flist[i](alist[i], n);
		}
	}

	int64_t lookfor = n;
	int64_t cycle_steps = 0;
	lookfor = 2020;
	printf("Got %ld after %ld steps.\n", lookfor, INITIAL_STEPS);
	for (int64_t j = 0; j < PART2_SHUFFLE_TIMES; j++) {
		for (int i = n_funcs; i >= 0; i--) {
			n = flist[i](alist[i], n);
		}
		//printf(">>> %ld\n", n);
		if (n == lookfor) {
			cycle_steps = j + 1;
			printf("found repeat after %ld steps\n", cycle_steps);
			break;
		}
	}
	// 82569066636552 is too low
	int64_t times_remaining = (PART2_SHUFFLE_TIMES - INITIAL_STEPS - cycle_steps) % cycle_steps;
	printf("Times remaining after skipping %ld cycles: %ld\n",
			(PART2_SHUFFLE_TIMES - INITIAL_STEPS) / times_remaining, times_remaining);
	for (int j = 0; j < times_remaining; j++) {
		for (int i = n_funcs; i >= 0; i--) {
			n = flist[i](alist[i], n);
		}
	}
	printf("Part2: %ld\n", n);
	return 0;
}
