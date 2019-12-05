#define PROG_SIZE(x) (sizeof(x) / sizeof(int))

#define MAX_OPERANDS 4

int run_prog_output_in_arg(int * prog, int size, int input, int * output)
{
	int * mem = malloc(size * sizeof(int));
	memcpy(mem, prog, size * sizeof(int));
	int * p = &mem[0];
	int opvals[MAX_OPERANDS] = { [0 ... (MAX_OPERANDS - 1)] = 0 };
	int * opptrs[MAX_OPERANDS] = { [0 ... (MAX_OPERANDS - 1)] = 0 };
	int intcode_output = 0;
	int opcode;
	while (1) {
		opcode = *p % 100;
		char modestring[MAX_OPERANDS * 2];
		sprintf(modestring, "%0*d", MAX_OPERANDS, *p / 100);
		for (int i = 0; modestring[i] != '\0'; i++) {
			int idx = MAX_OPERANDS - i; // modestring[0] is mode for operand 8, [1] is op 7 and so on...
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
				*opptrs[1] = input;
				p += 2;
				break;
			}
			case 4: { // output
				intcode_output = *opptrs[1];
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
				if (output) *output = mem[0];
				return intcode_output; break;
			}
			default: printf("Illegal instruction %d @ %d\n", opcode, p - mem); *p = 99; break;
		}
	}
}

// add this to keep compatibility with day 2 :-)
int run_prog_with_input(int * prog, int size, int input)
{
	return run_prog_output_in_arg(prog, size, input, NULL);
}

// this is for day 2
int run_prog(int * prog, int size)
{
	int ret;
	run_prog_output_in_arg(prog, size, 0, &ret);
	return ret;
}

