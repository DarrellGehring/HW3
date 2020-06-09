#include <stdio.h>
#include <ctype.h>

int main(int numArgs, char *args[]) {
	if (numArgs != 3) {
		printf("Error: requires two arguments.\n");
		return -1;
	}

	printf("%s", args[2]);

	return 0;
}