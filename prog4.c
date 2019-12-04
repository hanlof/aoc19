#include <stdio.h>

#define START 134792
#define END 675810

int increasing(char * s)
{
	while (s[1] != '\0') {
		if (s[0] > s[1]) {
			return 0;
		}
		s++;
	}
	return 1;
}

int hasdouble(char *s, int isolated)
{
	char * s2 = s;
	int ok = 0;
	while (s[1] != '\0') {
		if (s[0] == s[1]) {
			ok = 1;
			if (isolated) { // must disqualify if more than two match
				if ((s[-1] != s[0] && s[2] != s[0])) {
					return 1;
				}
				ok = 0;
			}
		}
		s++;
	}
	return ok;
}

int main(int argc, char * argv[])
{
	int i = START;
	int count1 = 0;
	int count2 = 0;
	char s[1024] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char * s2 = &s[4]; // leave a margin before where we store the string

	for (int i = START; i <= END; i++) {
		sprintf(s2, "%d", i);
		if (increasing(s2)) {
			if (hasdouble(s2, 0)) count1++;
			if (hasdouble(s2, 1)) {
				count2++;
			}
		}
	}
	printf("Count 1: %d\n", count1);
	printf("Count 2: %d\n", count2);
	return 0;
}
