#include <math.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	/*
	printf("%f\n", atan2(0.0, 5.0));
	printf("%f\n", atan2(1.0, 1.0));

	printf("%f\n", atan2(1.0, 0.0));
	printf("%f\n", atan2(1.0, -1.0));

	printf("---\n");
	printf("%f\n", atan2(0.0, -1.0));
	printf("%f\n", atan2(-1.0, -1.0));

	printf("%f\n", atan2(-1.0, 0.0));
	printf("%f\n", atan2(-1.0, 1.0));
	*/

	int dx, dy;
	// q1
	dx = 1; dy = -1;
	printf("Q1 %f\n", atan2(-dx, dy));

	dx = 1; dy = 1;
	printf("Q2 %f\n", atan2(-dx, dy));

	dx = -1; dy = 1;
	printf("Q3 %f\n", atan2(-dx, dy));

	dx = -1; dy = -1;
	printf("Q4 %f\n", atan2(-dx, dy));

	dx = 0; dy = -1;
	printf("U %f\n", atan2(dx, dy));

	dx = 1; dy = 0;
	printf("R %f\n", atan2(dx, dy));

	dx = 0; dy = 1;
	printf("D %f\n", atan2(dx, dy));

	dx = -1; dy = 0;
	printf("L %f\n", atan2(dx, dy));
}
