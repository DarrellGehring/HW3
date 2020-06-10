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

	int cp[2];

	if (pipe(cp) < 0) {
		printf("didn't work, couldn't not establish pipe.\n");
		return -1;
	}

	if (*args[1] == '1') {
		printf("Exec:: \n");
		close(1); //close stdout
		dup2(cp[1], 1); //move stdout to pipe of cp[1]
		close(0); //close stdin
		close(cp[0]); //close pipe in
		close(cp[1]);
		execl("minMax", "minMax", "1", *args[2], (char *)0);
		printf("Exec|| \n");
	}

	printf("\n");
	return 0;
}