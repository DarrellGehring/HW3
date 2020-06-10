#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int numArgs, char *args[]) {
	if (numArgs != 3) {
		printf("Error: requires two arguments.\n");
		return -1;
	}

	if (*args[1] != '1' && *args[1] != '4') {
		printf("Error: first argument must be 1 or 4.\n");
		return -1;
	}

	if (*args[1] == '1') {
		printf("Exec:: \n");
		execl("minMax", "minMax", "1", *args[2], (char *)0);
		printf("Exec|| \n");
	}

	printf("\n");
	return 0;
}