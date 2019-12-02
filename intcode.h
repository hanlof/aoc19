#define PROG_SIZE(x) (sizeof(x) / sizeof(int))

int run_prog(int * prog, int size)
{
	int * mem = (int *) malloc(size * sizeof(int));
	memcpy(mem, prog, size * sizeof(int));
	int * p = &mem[0];
	while (1) {
		switch (*p) {
			case 1: {
				mem[p[3]] = mem[p[1]] + mem[p[2]];
				p += 4;
				break;
			}
			case 2: {
				mem[p[3]] = mem[p[1]] * mem[p[2]];
				p += 4;
				break;
			}
			case 99: return mem[0]; break;
			default: printf("HOF\n"); *p = 99; break;
		}
	}
}
