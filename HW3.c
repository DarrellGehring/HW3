#include <stdio.h>
#include <ctype.h>

int main(int numArgs, char *args[]) {
	if (numArgs != 3) {
		printf("Error: requires two arguments.");
		return -1;
	}

	printf("%s", args[1]);

	return 0;
}