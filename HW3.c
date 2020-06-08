#include <stdio.h>
int main(int numForks, char *fileName[]) {

	if (!numForks || (numForks != 1 && numForks != 4)) {
		printf("Invalid Parameter: only takes 1 or 4");
		return 1;
	}

	printf("Hello, World!");
	return 0;
}