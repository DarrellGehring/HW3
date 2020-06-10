#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int numArgs, char *args[]) {

	const char *filename = args[2];

	FILE * readF = fopen(filename, "rb");

	if (readF) {
		int pid = getpid();
		fseek(readF, 0, SEEK_END); //go to end of file
		long size = ftell(readF); //keep track of last byte in the file
		fseek(readF, 0, SEEK_SET); //go to beginning of file

		int num = (int)size / (int)sizeof(int); //How many total numbers in file


		printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);
		int i;
		for (i = 0; i < num; i++) {
			int temp = 0;
			fread(&temp, sizeof(int), 1, readF);
			printf("%i: %i\t", pid, temp);
		}
		fclose(readF);
	}
	else {
		printf("Error: no file found with that name.\n");
	}
	printf("\n");
	return 0;
}