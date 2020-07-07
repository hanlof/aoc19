#include <stdio.h>
#include <unistd.h>
#include <alloca.h>

#define printfU(s, ...) { \
	int n = 1 + snprintf(NULL, 0, s, __VA_ARGS__); \
	char * c = alloca(n); \
	snprintf(c, n, s, __VA_ARGS__); \
	write(STDOUT_FILENO, c, n); }

#define printU(s) { \
	int n = 1 + snprintf(NULL, 0, s); \
	char * c = alloca(n); \
	snprintf(c, n, s); \
	write(STDOUT_FILENO, c, n); }
