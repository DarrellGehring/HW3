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
		if (*args[1] == '4') {
			int pid = getpid();
			fseek(readF, 0, SEEK_END); //go to end of file
			long size = ftell(readF); //keep track of last byte in the file
			fseek(readF, 0, SEEK_SET); //go to beginning of file

			int num = (int)size / (int)sizeof(int); //How many total numbers in file

			int startOffset = 0;
			int endOffset = num - 1;
			int block = 0;

			if (*args[1] == '4') {
				printf("Using 4 fork version");
				block = atoi(args[3]);
				startOffset = (block)*((num + 3) / 4); //Will produce index to start at 

				if (block != 3) {
					endOffset = (((block + 1)*((num + 3) / 4)) - 1);
				}
				else {
					endOffset = num - 1;
				}
			}

			printf("Start index: %d\tEnd index: %d\t For block %d\n", startOffset, endOffset, block);
			printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);




			fseek(readF, (startOffset*(int)sizeof(int)), SEEK_SET);

			int min;
			int max;

			int i;

			for (i = startOffset; i <= endOffset; i++) {
				int temp = i;



				fread(&temp, sizeof(int), 1, readF);

				if (i == startOffset) {
					min = temp;
					max = temp;
					//printf("%i: %i : %d\t\n", pid, temp, i);

				}
				else if (i == endOffset) {
					//printf("%i: %i : %d\t\n", pid, temp, i);
				}

				if (temp < min) {
					min = temp;
				}

				if (temp > max) {
					max = temp;
				}
			}
			printf("Total numbers:%d Total Read: %d\n", num, i);
			printf("Min:%d Max: %d\n", min, max);
			fclose(readF);
		}
		else {
			int min;
			int max;
			const char * filename = "test.txt";
			FILE * ft = fopen(filename, "rb");
			if (ft) {
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
					//printf("%i: %i\t", pid, temp);

					if (i == 0) {
						min = temp;
						max = temp;
						//printf("%i: %i : %d\t\n", pid, temp, i);

					}

					if (temp < min) {
						min = temp;
					}

					if (temp > max) {
						max = temp;
					}
				}
				printf("Min: %d Max: %d\n", min, max);

				fclose(ft);
			}
			printf("\n");
		}
	}
	else {
		printf("Error: no file found with that name.\n");
	}
	printf("\n");
	return 0;
}