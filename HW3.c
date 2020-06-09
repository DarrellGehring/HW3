#include <stdio.h>
#include <ctype.h>

int main(int numArgs, char *args[]) {
	if (numArgs != 3) {
		printf("Error: requires two arguments.\n");
		return -1;
	}

	if (args[1] != '1' && args[1] != '4') {
		printf("Error: first argument must be 1 or 4.\n");
		return -1;
	}

	printf("%s", args[2]);

	return 0;
}