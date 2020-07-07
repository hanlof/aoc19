#include <stdio.h>
#include <stdint.h>

#define FIFO_SIZE 8
struct fifo {
	int write;
	int read;
	int64_t values[FIFO_SIZE];
};
#define PUT(f, v) ((f).values[((f).write < (FIFO_SIZE)) ? (f).write++ : ((f).write = 0)] = v)
#define GET(f) ((f).values[((f).read < (FIFO_SIZE)) ? (f).read++ : ((f).read = 0)])
#define EMPTY(f) ((f).write == (f).read)
#define NEW_FIFO(name) struct fifo name = { 0, 0, { [0 ... (FIFO_SIZE - 1)] = 0 } };

NEW_FIFO(name);
int main(int argc, char * argv[])
{
	PUT(name, 0);
	PUT(name, 1);
	PUT(name, 2);
	PUT(name, 3);
	PUT(name, 4);
	PUT(name, 5);
	for(int i = 0; i < 100; i++) {
		PUT(name, i);
		printf("put %d\n", i);
		printf("get %ld\n", GET(name));
		printf("%d %d\n", name.read, name.write);
	}
	while (!EMPTY(name)) {
		printf("get %ld\n", GET(name));
	}


	return 0;
}
