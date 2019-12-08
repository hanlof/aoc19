#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

char pixels[100][25 * 6];

int main(int argc, char * argv[])
{
	int fd = open("inputs/input8", O_RDONLY);
	(void)read(fd, pixels, sizeof(pixels));
	int lowest = 200000;
	int lowest_layer = 0;
	for (int i = 0; i < 100; i++) {
		int count = 0;
		for (int j = 0; j < 150; j++) {
			if (pixels[i][j] == '0') {
				count++;
			}
		}
		if (count < lowest) {
			lowest = count;
			lowest_layer = i;
		}
	}

	// XXX TODO: no foldcolumn and no matchadd in :Trun window!!
	int twos = 0;
	int ones = 0;
	for (int i = 0; i < 150; i++) {
		//printf("%c", pixels[lowest_layer][i]);
		if (pixels[lowest_layer][i] == '1') ones++;
		if (pixels[lowest_layer][i] == '2') twos++;
	}
	printf("Part1: %d\n", ones * twos);

	// part 2
	printf("Part2: ...\n");
	char final[150];
	for (int i = 99; i >= 0; i--) { // last layer is the lowest!
		for (int j = 0; j < 150; j++) {
			char p = pixels[i][j];
			if (p != '2') { // '2' is transparent
				final[j] = p;
			}
			if (final[j] == '1') {
				printf("\e[47m ");
			} else {
				printf("\e[40m ");
			}
			if (!((j + 1) % 25)) printf("\n");
		}
		printf("\e[0m\e[6A");
		usleep(10000);
	}
	printf("\e[7B\n");
	return 0;
}
