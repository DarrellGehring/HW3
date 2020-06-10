#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(int numArgs, char *args[]) {

	const char *filename = args[2];
	
	if (*args[1] != '1' && *args[1] != '4') {
		printf("Error: first argument must be 1 or 4.\n");
		return -1;
	}

	FILE * readF = fopen(filename, "rb");

	if (readF) {
		int pid = getpid();
		fseek(readF, 0, SEEK_END); //go to end of file
		long size = ftell(readF); //keep track of last byte in the file
		fseek(readF, 0, SEEK_SET); //go to beginning of file

		int num = (int)size / (int)sizeof(int); //How many total numbers in file

		int startOffset = 0;
		int endOffset = num;

		if (*args[1] == '4') {
			printf("Using 4 fork version");
			int block = *args[3];
			startOffset = (block)*((num + 3) / 4); //Will produce index to start at 
			
			if (block != 3) {
				endOffset = (((block + 1)*((num + 3) / 4)) - 1);
			}
			else {
				endOffset = num;
			}
		}


		printf("Start index: %d\tEnd index: %d\n", startOffset, endOffset);
		printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);
		int i;
		/*for (i = startOffset; i < endOffset; i++) {
			int temp = i;
			fread(&temp, sizeof(int), 1, readF);
			printf("%i: %i : %d\t", pid, temp, i);
		}
		printf("Total numbers:%d Total Read: %d\n", num, i);*/
		fclose(readF);
	}
	else {
		printf("Error: no file found with that name.\n");
	}
	printf("\n");
	return 0;
}