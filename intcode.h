#define PROG_SIZE(x) (sizeof(x) / sizeof(int64_t))

#define MAX_OPERANDS 4

#define STACK_SIZE 16

struct stack {
	int top;
	int64_t values[STACK_SIZE];
};

#define POP(x) ((x).values[--(x).top])
#define PUSH(x, y) ((x).values[(x).top++] = y)

struct fifo {
	int write;
	int read;
	int64_t values[STACK_SIZE];
};

#define PUT(f, v) ((f).values[(f).write++] = v)
#define GET(f) ((f).values[(f).read++])
#define EMPTY(f) ((f).write == (f).read)

int64_t run_prog(int64_t * mem, int64_t ** prog_ctr, struct fifo * inputs, struct fifo * outputs)
{
	int64_t * p = &mem[0];
	if (prog_ctr) {
	  p = *prog_ctr;
	}
	int64_t opvals[MAX_OPERANDS + 1] = { [0 ... (MAX_OPERANDS - 1)] = 0 };
	int64_t * opptrs[MAX_OPERANDS + 1] = { [0 ... (MAX_OPERANDS - 1)] = 0 };
	int64_t opcode;
	while (1) {
		opcode = *p % 100;
		char modestring[MAX_OPERANDS * 2];
		sprintf(modestring, "%0*ld", MAX_OPERANDS, *p / 100);
		for (int64_t i = 0; modestring[i] != '\0'; i++) {
			int64_t idx = MAX_OPERANDS - i; // modestring[0] is mode for operand 8, [1] is op 7 and so on...
			switch (modestring[i]) {
				case '0': { // normal (mem address)
					opptrs[idx] = &mem[p[idx]];
					break;
				}
				case '1': { // immediate
					opvals[idx] = p[idx];
					opptrs[idx] = &opvals[idx];
					break;
				}
				default: {
					printf("Unknown parameter mode: %c\n", modestring[i]);
					return -1;
				}
			}
		}

		switch (opcode) {
			case 1: { // add
				*opptrs[3] = *opptrs[1] + *opptrs[2];
				p += 4;
				break;
			}
			case 2: { // multiply
				*opptrs[3] = *opptrs[1] * *opptrs[2];
				p += 4;
				break;
			}
			case 3: { // read input
				if (EMPTY(*inputs)) {
					*prog_ctr = p;
					return 3;
				} else {
				}
				*opptrs[1] = GET(*inputs);
				//printf("in %ld\n", *opptrs[1]);
				p += 2;
				break;
			}
			case 4: { // output
				//printf("out %ld (%p)\n", *opptrs[1], p);
				PUT(*outputs, *opptrs[1]);
				p += 2;
				break;
			}
			case 5: { // jump if non-zero
				if (*opptrs[1]) {
					p = &mem[*opptrs[2]];
				} else {
					p += 3;
				}
				break;
			}
			case 6: { // jump if zero
				if (!*opptrs[1]) {
					p = &mem[*opptrs[2]];
				} else {
					p += 3;
				}

				break;
			}
			case 7: { // if 1 < 2 then 3 = 1
				if (*opptrs[1] < *opptrs[2]) {
					*opptrs[3] = 1;
				} else {
					*opptrs[3] = 0;
				}
				p += 4;
				break;
			}
			case 8: { // if 1 == 2 then 3 = 1
				if (*opptrs[1] == *opptrs[2]) {
					*opptrs[3] = 1;
				} else {
					*opptrs[3] = 0;
				}
				p += 4;
				break;
			}

			case 99: { // exit
				//printf("exit\n");
				return 99; break;
			}
			default: printf("Illegal instruction %li @ %ld\n", opcode, p - mem); *p = 99; break;
		}
	}
}
