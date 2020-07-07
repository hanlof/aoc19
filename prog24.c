#include <stdio.h>

char input[5][5] = {
	"###..",
	".##..",
	"#....",
	"##..#",
	".###."
};

char mark_or_not(char * p)
{
	int count = 0;
	if (p[1] == '#') count++;
	if (p[-1] == '#') count++;
	if (p[7] == '#') count++;
	if (p[-7] == '#') count++;

	if (p[0] == '#' && count != 1) return '.';
	if (p[0] == '.' && count > 0 && count < 3) return '#';
	return p[0];
}

void step(char * in, char * out)
{
	for (int i = 1; i < 6; i++) {
		for (int j = 1; j < 6; j++) {
			out[j * 7 + i] = mark_or_not(&in[j * 7 + i]);
		}
	}
}

void draw(char * p)
{
	for (int i = 0; i < 5; i++) {
		printf("|%5.5s|\n", &p[i * 7]);
	}
}

void draw2(char * p)
{
	for (int i = 0; i < 5; i++) {
		printf("%c%c%c%c%c\n", (char)p[i * 5 + 0] + 0x30, p[i * 5 + 1] + 0x30,
				       (char)p[i * 5 + 2] + 0x30, p[i * 5 + 3] + 0x30, p[i * 5 + 4] + 0x30);
	}
}
char saved_areas[1000][25];
void save_area(char * where, char * p)
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			where[i * 5 + j] = p[(i + 1) * 7 + j + 1];
		}
	}
}

int compare(char * a, char * b)
{
	for (int i = 0; i < 25; i++) {
		if (a[i] != b[i]) return 0;
	}
	return 1;
}

int calc(char * a)
{
	int val = 1;
	int sum = 0;
	for (int i = 0; i < 25; i++) {
		if (a[i] == '#') sum += val;
		val *= 2;
	}
	return sum;
}

void part1()
{
	char area1[7][7] = { [0 ... 6] = "......\n" };
	char area2[7][7] = { [0 ... 6] = "......\n" };

	char * in = &area1[0][0];
	char * out = &area2[0][0];
	char slask[25];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			area1[i + 1][j + 1] = input[i][j];
		}
	}
	for (int i = 0; i < 1000; i++) {
		step(in, out);
		save_area(slask, out);
		for (int j = 0; j < i; j++) {
			if (compare(slask, &saved_areas[j][0])) {
				printf(">>>> %i %i\n", i, j);
				draw(slask);
				printf("-----\n");
				draw(&saved_areas[j][0]);
				printf("sum: %d\n", calc(slask));
				return;
			}
		}
		save_area(&saved_areas[i][0], out);
		if (in == &area1[0][0]) {
			in = &area2[0][0];
			out = &area1[0][0];
		} else {
			in = &area1[0][0];
			out = &area2[0][0];
		}
	}

}

#define VAL(z, y, x) (in[((z) * 25) + ((y) * 5) + (x)])
char evaluate(char * in, int z, int y, int x)
{
	// center square. always zero.
	if (y == 2 && x == 2) return 0;

	int count = 0;

	// count above
	if (y == 0) { // top row. we must look "outside". z - 1
		// XXX why the #@$! do we look at 3, 2 in the outside box here?
		// XXX and in 1, 2 for counting "below"
		// XXX it gives the correct result but WHY?!!11
		count += VAL(z - 1, 3, 2);
	} else if (y == 3 && x == 2) { // square above is center. must look "inside". z + 1
		// count top row of the area one level deeper
		count += VAL(z + 1, 0, 0);
		count += VAL(z + 1, 0, 1);
		count += VAL(z + 1, 0, 2);
		count += VAL(z + 1, 0, 3);
		count += VAL(z + 1, 0, 4);
	} else {
		count += VAL(z, y - 1, x);
	}

	// count below
	if (y == 4) { // bottom row. we must look "outside". z - 1
		count += VAL(z - 1, 1, 2);
	} else if (y == 1 && x == 2) { // square to count is the center. must look "inside". z + 1
		// count bottom row of the area one level deeper
		count += VAL(z + 1, 4, 0);
		count += VAL(z + 1, 4, 1);
		count += VAL(z + 1, 4, 2);
		count += VAL(z + 1, 4, 3);
		count += VAL(z + 1, 4, 4);
	} else {
		count += VAL(z, y + 1, x);
	}

	// count west
	if (x == 0) { // leftmost column, we must look "outside". z - 1
		count += VAL(z - 1, 2, 1);
	} else if (y == 2 && x == 3) { // square to count is the center. must look "inside". z + 1
		// count rightmost row of the area one level deeper
		count += VAL(z + 1, 0, 4);
		count += VAL(z + 1, 1, 4);
		count += VAL(z + 1, 2, 4);
		count += VAL(z + 1, 3, 4);
		count += VAL(z + 1, 4, 4);

	} else {
		count += VAL(z, y, x - 1);
	}

	// count east
	if (x == 4) { // rightmost column, we must look "outside". z - 1
		count += VAL(z - 1, 2, 3);
	} else if (y == 2 && x == 1) { // square to count is the center. must look "inside". z + 1
		// count leftmost row of the area one level deeper
		count += VAL(z + 1, 0, 0);
		count += VAL(z + 1, 1, 0);
		count += VAL(z + 1, 2, 0);
		count += VAL(z + 1, 3, 0);
		count += VAL(z + 1, 4, 0);

	} else {
		count += VAL(z, y, x + 1);
	}

	if (VAL(z, y, x) == 1 && count != 1) return 0;
	if (VAL(z, y, x) == 0 && count > 0 && count < 3) return 1;
	return VAL(z, y, x);
}

void step2(char * in, char * out)
{
	for (int z = 40; z < 460; z++) {
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				out[z * 25 + y * 5 + x] = evaluate(in, z, y, x);
			}
		}
	}
}

char input_example[5][5] = {
	"....#",
	"#..#.",
	"#..##",
	"..#..",
	"#....",
};

int main(int argc, char * argv[])
{
	part1();

	// Part 2
	char areas1[500][25] = { [0 ... 499] = { [0 ... 24] = 0 } };
	char areas2[500][25] = { [0 ... 499] = { [0 ... 24] = 0 } };

	// Set up according to input data
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (input[i][j] == '#') {
				areas1[250][i * 5 + j] = 1;
			} else {
				areas1[250][i * 5 + j] = 0;
			}
		}
	}
	char * in = &areas1[0][0];
	char * out = &areas2[0][0];
	for (int i = 0; i < 200; i++) {
		step2(in, out);
		if (in == &areas1[0][0]) {
			in = &areas2[0][0];
			out = &areas1[0][0];
		} else {
			in = &areas1[0][0];
			out = &areas2[0][0];
		}
		/*
		draw2(&in[248 * 25]);
		printf("-----\n");
		draw2(&in[249 * 25]);
		printf("-----\n");
		draw2(&in[250 * 25]);
		printf("-----\n");
		draw2(&in[251 * 25]);
		printf("-----\n");
		draw2(&in[252 * 25]);
		printf("\n\n");

		(void)getc(stdin);
		*/

	}
	draw2(&in[245 * 25]);
	printf("\n-----\n");
	draw2(&in[246 * 25]);
	printf("\n-----\n");
	draw2(&in[247 * 25]);
	printf("\n-----\n");
	draw2(&in[248 * 25]);
	printf("\n-----\n");
	draw2(&in[249 * 25]);
	printf("\n-----\n");
	draw2(&in[250 * 25]);
	printf("\n-----\n");
	draw2(&in[251 * 25]);
	printf("\n-----\n");
	draw2(&in[252 * 25]);
	printf("\n-----\n");
	draw2(&in[253 * 25]);
	printf("\n-----\n");
	draw2(&in[254 * 25]);
	printf("\n-----\n");
	draw2(&in[255 * 25]);
	printf("\n-----\n");
	draw2(&in[256 * 25]);
	printf("\n-----\n");
	draw2(&in[257 * 25]);

	int part2 = 0;
	for (int i = 0; i < 500 * 25; i++) {
		part2 += in[i];
	//	if (in[i] != 0) { printf("XXX %d\n", i / 25); break; }
	}
	printf("Part2: %d\n", part2);

	return 0;
}
