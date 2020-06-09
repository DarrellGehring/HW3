#include <stdio.h>
int main(int numForks, char *fileName[]) {

	if ((numForks == 1)
		printf("Hello, World 1\n");
		return 0;
	} else if (numForks == 4) {
		printf("Hello, World 2\n");
		return 0;
	} else {
		return -1;
	}
}