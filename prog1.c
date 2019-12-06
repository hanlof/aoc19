#include <stdio.h>

int main(int argc, char * argv[])
{
	int w;
	int tot1 = 0;
	int tot2 = 0;
	while (scanf("%d\n", &w) == 1) {
		int tmp = (w / 3) - 2;
		tot1 += tmp;
		while (tmp > 0) {
			tot2 += tmp;
			tmp = (tmp / 3) - 2;
		}
	}
	printf("Part1: %d\n", tot1);
	printf("Part2: %d\n", tot2);
}

