#include <stdio.h>
int main(int numForks, char *fileName[]) {

	if ((*numForks == NULL) || (numForks != 1 && numForks != 4)) {
		printf("Invalid Parameter: only takes 1 or 4\n");
		return 1;
	}

	printf("Hello, World!\n");
	return 0;
}