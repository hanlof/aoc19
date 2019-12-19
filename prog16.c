#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PATLEN 4
int base_pattern[PATLEN] = { 0, 1, 0, -1 };
inline int pattern(int len, int index)
{
	index++; // "skip the very first..."

	int b = (index / len) % PATLEN;

	return base_pattern[b];
}

char base_chars[PATLEN] = { ' ', '+', ' ', '-' };
inline char cpattern(int len, int index)
{
	index++; // "skip the very first..."

	int b = (index / len) % PATLEN;

	return base_chars[b];
}

void run_phase(int * input, int * output, int len)
{
	for (int i = 0; i < len; i++) {
		int sum = 0;

		for (int j = 0; j < len; j++) {
			sum += input[j] * pattern(i + 1, j);
		}
		output[i] = abs(sum) % 10;
		//if ((i % 100) == 0) write(STDOUT_FILENO, ".", 1);
	}
}

int * part1(int * input, int len)
{
	int * temp1 = calloc(sizeof(int), len);
	int * temp2 = calloc(sizeof(int), len);

	int * inp = input;
	int * outp = temp1;
	for (int i = 0; i < 100; i++) {
		run_phase(inp, outp, len);
		if (outp == temp1) {
			inp = temp1;
			outp = temp2;
		} else {
			inp = temp2;
			outp = temp1;
		}
	}

	return inp;
}

void run_phase_x(int * input, int * output, int len)
{
	int64_t sum = 0;
	int64_t sum2 = 0;
	int * end = &input[len - 2];
	//printf("%ld %d\n", end - &input[0], len);
	// XXX opportunity for cache hit/miss experiments. one loop instead of two!
	int * p;
	for (p = &input[0]; p < end; p += 4) {
		sum += p[0];
		sum -= p[2];
		//sum2 += p[0];
	}
	if (&p[2] < &input[len]) sum += p[2];

	output[1] = sum2;
	output[0] = abs(sum) % 10;
}

int * part2(int * input, int input_len)
{
	int len = input_len * 10000;
	int * temp1 = calloc(sizeof(int), len);
	int * temp2 = calloc(sizeof(int), len);

	printf("X\n");
	for (int i = 0; i < len; i++) temp1[i] = input[i % input_len];
	printf("X\n");

	int * inp = temp1;
	int * outp = temp2;
	for (int i = 0; i < 1000; i++) {
		run_phase_x(inp, outp, len);
		if (outp == temp1) {
			inp = temp1;
			outp = temp2;
		} else {
			inp = temp2;
			outp = temp1;
		}
	}

	return inp;
}

int main(int argc, char * argv[])
{
	char input[1024];
	read(STDIN_FILENO, input, 1000);

	int len = index(input, '\n') - &input[0];

	int * values = calloc(sizeof(int), len);
	for (int i = 0; i < len; i++) values[i] = input[i] - '0';

	//int * part1_out = part1(values, len);

	//for (int i = 0; i < 8; i++)
		//printf("%d", part1_out[i]);
	//printf("\n");

	/*
	int inputx[] = { 0, 1, 2, 3 };
	int outputx[sizeof(inputx) / sizeof(int)];
	run_phase(inputx, outputx, sizeof(inputx) / sizeof(int));
	for (int i = 0; i < sizeof(inputx) / sizeof(int); i++)
		printf("%d ", outputx[i]);
	printf("\n");
	*/

	// output graphic representation of what to sum / subtract
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 230; j++) {
			printf("%c", cpattern(i + 1, j));
		}
		printf("\n");
	}

	int * part2_out = part2(values, len);
	for (int i = 0; i < 8; i++)
		printf("%d ", part2_out[i]);
	printf("\n");

	return 0;
}
