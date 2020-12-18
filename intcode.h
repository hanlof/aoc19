#include <stdint.h>
#include <string.h>

#define PROG_SIZE(x) (sizeof(x) / sizeof(int64_t))

#define MAX_OPERANDS 4

#define STACK_SIZE 3200

struct stack {
	int top;
	int64_t values[STACK_SIZE];
};
#define POP(x) ((x).values[--(x).top])
#define PUSH(x, y) ((x).values[(x).top++] = y)

#define FIFO_SIZE 4096
struct fifo {
	int write;
	int read;
	int64_t values[FIFO_SIZE];
};

#define PUT(f, v) ((f).values[((f).write < (FIFO_SIZE)) ? (f).write++ : ((f).write = 0)] = v)
#define GET(f) ((f).values[((f).read < (FIFO_SIZE)) ? (f).read++ : ((f).read = 0)])
#define EMPTY(f) ((f).write == (f).read)

struct intcode_machine {
	int64_t * mem;
	int64_t * prog_ctr;
	struct fifo inputs;
	struct fifo outputs;
	struct fifo * inputs_ptr;
	struct fifo * outputs_ptr;
	int64_t relative_base;
};

// Forward declaration
int64_t run_prog(int64_t * mem, int64_t ** prog_ctr, struct fifo * inputs, struct fifo * outputs, struct intcode_machine * m);

struct intcode_machine * new_intcode_machine(int64_t * original_intcode, int size, int64_t * inputs, int ninputs, int extra_mem)
{
	struct intcode_machine * m = calloc(sizeof(struct intcode_machine), 1);
//	printf("Alloc %ld\n", (size * sizeof(int64_t)) * (1 + extra_mem));
	m->mem = calloc(size * sizeof(int64_t), 1 + extra_mem);
	memcpy(m->mem, original_intcode, size * sizeof(int64_t));
	m->prog_ctr = &m->mem[0];
	m->inputs.read = 0;
	m->inputs.write = 0;
	m->outputs.read = 0;
	m->outputs.write = 0;
	m->inputs_ptr = &m->inputs;
	m->outputs_ptr = &m->outputs;
	m->relative_base = 0;

	for (int i = 0; i < ninputs; i++) {
		PUT(m->inputs, inputs[i]);
	}

	return m;
}

#define _I(...) {__VA_ARGS__}
#define NEW_INTCODE_MACHINE(m, i) new_intcode_machine(m, PROG_SIZE(m), ((int64_t[])i), (sizeof((int64_t[])i) / sizeof(int64_t)), 0)
#define NEW_INTCODE_MACHINE_EMEM(m, i, emem) new_intcode_machine(m, PROG_SIZE(m), ((int64_t[])i), (sizeof((int64_t[])i) / sizeof(int64_t)), emem)

int64_t run_machine(struct intcode_machine * m)
{
	return run_prog(m->mem, &m->prog_ctr, m->inputs_ptr, m->outputs_ptr, m);
}

int64_t run_prog(int64_t * mem, int64_t ** prog_ctr, struct fifo * inputs, struct fifo * outputs, struct intcode_machine * m)
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
		//printf("%ld - ", *p);
		sprintf(modestring, "%0*ld", MAX_OPERANDS, *p / 100);
		//printf("%s\n", modestring);
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
				case '2': { // relative baseptr
					opptrs[idx] = &mem[p[idx] + m->relative_base];
					if (4 == opcode && *opptrs[1] == 5185) {
						/*
						printf("base %ld\n", m->relative_base);
						printf("idx %ld\n", idx);
						printf("mem %p\n", mem);
						*/
					}
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
					//printf("exit becuse missing indata\n");
					*prog_ctr = p;
					return 3;
				}
				*opptrs[1] = GET(*inputs);
				//printf("in %ld\n", *opptrs[1]);
				p += 2;
				break;
			}
			case 4: { // output
				//printf("out %ld (%p)\n", *opptrs[1], p);
				//
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
			case 9: { // adjust base offset
				m->relative_base += *opptrs[1];
				//printf("adjust base %ld (%ld)\n", *opptrs[1], m->relative_base);
				p += 2;
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
