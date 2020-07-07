#include <stdio.h>
#include <fcntl.h>

void main()
{
	printf("11111\n %d ", 1);
	printf("22222222222 %d\n", 2);
	int ret = snprintf(NULL, 0, "%s %d\n", "apa", 9991);
	printf("%s %d\n", "apa", 9991);
	printf("ret %d\n", ret);


}
