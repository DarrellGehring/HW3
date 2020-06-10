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

	const char * filename = *args[2];

	FILE * readF = fopen(filename, "rb");

	if (readF) {
		int pid = getpid();
		fseek(ft, 0, SEEK_END); //go to end of file
		long size = ftell(ft); //what byte in file am I at?
		fseek(ft, 0, SEEK_SET); //go to beginning of file
		int num = (int)size / (int)sizeof(int);
		printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);
		int i;
		for (i = 0; i < num; i++) {
			int temp = 0;
			fread(&temp, sizeof(int), 1, ft);
			printf("%i: %i\t", pid, temp);
		}
		fclose(ft);
	}
	else {
		printf("Error: no file found with that name.\n");
	}
	printf("\n");
	return 0;
}