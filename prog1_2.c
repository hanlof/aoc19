#include <stdio.h>


int main(int argc, char * argv[])
{
	int w;
	int tot = 0;
	while (scanf("%d\n", &w) == 1) {
		int tmp = (w / 3) - 2;
		while (tmp > 0) {
			tot += tmp;
			tmp = (tmp / 3) - 2;
		}
	}
	printf("%d\n", tot);
}

