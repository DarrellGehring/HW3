#include <stdio.h>
#include <ctype.h>

int main(int numArgs, char *args[]) {
	if (numArgs != 2) {
		printf("Error: requires two arguments.");
		return -1;
	}

	printf("Files: " + args[1] + "\n");

	return 0;
}