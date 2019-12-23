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

struct moving_sum {
	char * start;
	char * end;
	int sum;
};

int update_sum_start(struct moving_sum * m, int i, char * end)
{
	char * new_start = m->start + i + i + 1;
	int retval = 0;
	if (new_start >= end) {
		new_start = end - 1;
		retval = 1;
		m->sum = 0;
	}
	while (m->start < new_start) {
		m->sum -= m->start[0];
		m->start++;
	}
	return retval;
}

int update_sum_end(struct moving_sum * m, int i, char * end)
{
	char * new_end = m->end + i + i + 2;
	int retval = 0;
	if (new_end >= end) {
		new_end = end - 1;
		retval = 1;
	}
	while (m->end < new_end) {
		m->end++;
		m->sum += m->end[0];
	}
	return retval;
}

#define NSUMS 1000
struct moving_sum slask = { 0, 0, 0 };
struct moving_sum moving_sums[NSUMS] = { [0 ... NSUMS-1] = { 0, 0, 0 } };
struct moving_sum slbsk = { 0, 0, 0 };

void run_phase_x(char * input, char * output, int len)
{

	// NOTE using chars instead of ints reduces time by almost 50%. (probably because less memory to fetch into cache)
	// NOTE doing more additions, subtractions inside the loop barey increases the run time

	int nsums = len / 10000 + 1;
	int64_t sum = 0;

	for (int i = 0; i < nsums; i++) {
		moving_sums[i].start = &input[-1];
		moving_sums[i].end = &input[-2];
		moving_sums[i].sum = 0;
	}

	int last_start_end_sum = nsums - 1;
	int last_start_sum = nsums - 1;

	for (int i = 0; i < len; i++) { // for each output value
		sum = 0;
		for (int j = 0; j <= last_start_end_sum; j++) {
			if (update_sum_end(&moving_sums[j], j, &input[len])) {
				last_start_end_sum -= 1;
				//printf("SE-- %d %d %d\n", i, last_start_sum, last_start_end_sum);
			}
		}
		for (int j = 0; j <= last_start_sum; j++) {
			if (update_sum_start(&moving_sums[j], j, &input[len])) {
				last_start_sum -= 1;
				//printf("S-- %d %d %d\n", i, last_start_sum, last_start_end_sum);
				break;
			}
			if (j % 2 == 0) {
				sum += moving_sums[j].sum;
			} else {
				sum -= moving_sums[j].sum;
			}

		}
		/*
		if (i < 3) { 
			printf("%d update: %ld %ld %d\n", i, moving_sums[0].start - &input[0], moving_sums[0].end - &input[0], (int)moving_sums[0].sum);
			printf("           %ld %ld %d\n", moving_sums[1].start - &input[0], moving_sums[1].end - &input[0], (int)moving_sums[1].sum);
			printf("           %ld %ld %d\n", moving_sums[2].start - &input[0], moving_sums[2].end - &input[0], (int)moving_sums[2].sum);
		}
		*/
		if (last_start_end_sum < nsums - 1) {
			output[i] = abs(sum) % 10;
			continue;
		}

		int incr = i + 1;
		//printf("%d %d %ld\n", i, last_start_end_sum, moving_sums[last_start_end_sum].end - &input[0]);
		for (char * p = moving_sums[last_start_end_sum].end + 1 + incr; &p[0] < &input[len]; ) {
			char * end = &p[incr];
			int tempsum = 0;
			if (end > &input[len]) end = &input[len];
			for (; p < end; p++) {
				tempsum += *p;
			}
			if (last_start_end_sum % 2 == 0) {
				//printf("%d minus\n", i);
				sum -= tempsum;
			} else {
				//printf("%d plus\n", i);
				sum += tempsum;
			}

			p += incr;
			if (&p[0] >= &input[len]) break;

			end = &p[incr];
			tempsum = 0;
			if (end > &input[len]) end = &input[len];
			for (; p < end; p++) {
				tempsum += *p;
			}
			if (last_start_end_sum % 2 == 0) {
				sum += tempsum;
			} else {
				sum -= tempsum;
			}
			p += incr;
		}

		output[i] = abs(sum) % 10;
		//if (i % 1000 == 0) { write(STDOUT_FILENO, ".", 1); }
	}
	
}

int * part1(int * input, int phases, int len)
{
	int * temp1 = calloc(sizeof(int), len);
	int * temp2 = calloc(sizeof(int), len);

	int * inp = input;
	int * outp = temp1;
	for (int i = 0; i < phases; i++) {
		run_phase(inp, outp, len);
		if (outp == temp1) {
			inp = temp1;
			outp = temp2;
		} else {
			inp = temp2;
			outp = temp1;
		}
	}

	// 29956495
	printf("Part1: ");
	for (int i = 0; i < 8; i++)
		printf("%d", (int)inp[i]);
	printf("\n");

	return inp;
}

char * part2(int * input, int phases, int input_len, int multiplier)
{
	int len = input_len * multiplier;
	char * temp1 = calloc(1, len);
	char * temp2 = calloc(1, len);

	for (int i = 0; i < len; i++) {
		temp1[i] = (char)input[i % input_len];
	}

	char * inp = temp1;
	char * outp = temp2;
	int offset = 0;
	int slask = 1;
	for (int k = 6; k >= 0; k--) {
		offset += inp[k] * slask;
		slask = slask * 10;
	}

	printf("first seven? %d\n", offset);
	for (int i = 0; i < phases; i++) {
		run_phase_x(inp, outp, len);
		if (outp == temp1) {
			inp = temp1;
			outp = temp2;
		} else {
			inp = temp2;
			outp = temp1;
		}
		printf(">>> %d\n", i);
	}

	// read 7 at start. skip that many!
	//printf("Part2: ");
	//for (int i = 0; i < 8; i++)
		//printf("%d", inp[i]);
	//printf("\n");

	
	printf("Part2: ");
	for (int i = offset; i < 8 + offset; i++)
		printf("%d", inp[i]);
	printf("\n");
	return inp;
}

int main(int argc, char * argv[])
{
	char input[1024];
	read(STDIN_FILENO, input, 1000);

	int len = index(input, '\n') - &input[0];

	int * values = calloc(sizeof(int), len);
	for (int i = 0; i < len; i++) values[i] = input[i] - '0';

	int phases = 100;
	int * part1_out = part1(values, phases, len);
	(void) part1_out;

	char * part2_out = part2(values, phases, len, 10000);
	(void) part2_out;
	/*
	int matching = 2;
	for (int i = 0; i < len; i++) {
		if (part1_out[i] == part2_out[i]) {
			if (matching != 1) {
				printf("matching from %d\n", i);
			}
			matching = 1;
		} else {
			if (matching != 0) {
				printf("not matching from %d\n", i);
			}
			matching = 0;
		}
	}
	*/


	// output graphic representation of what to sum / subtract
	/*
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 230; j++) {
			printf("%c", cpattern(i + 1, j));
		}
		printf("\n");
	}
	*/


	return 0;
}
