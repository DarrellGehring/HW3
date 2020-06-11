#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(int numArgs, char *args[]) {
	printf("Here0");

	const char *filename = args[2];
	
	if (*args[1] != '1' && *args[1] != '4') {
		printf("Error: first argument must be 1 or 4 in minMax. Was: %s\n", *args[1]);
		return -1;
	}

	FILE * readF = fopen(filename, "rb");

	if (readF) {
		if (*args[1] == '4') {
			int pipes[8][2]; //Make 8 pipes with in and out, each for path of communication to or from
			pid_t subpid, parentpid;
			int startOffset = 0, endOffset, num, min, max, sentLen;

			int j;
			for (j = 0; j < 4; j++) {
				pipe(pipes[j]);
			}

			int k;
			for (k = 0; k < 4; k++) {

				//printf("Start index: %d\tEnd index: %d\t For block %d\n", startOffset, endOffset, block);
				//printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);
				if ((subpid = fork()) == 0) {
					printf("Here1");
					sentLen = read(pipes[k][0], &startOffset, sizeof(startOffset));
					printf("Here2");

					if (sentLen > 0) {

						subpid = getpid();

						fseek(readF, (startOffset*(int)sizeof(int)), SEEK_SET);

						printf("Child(%d): Recieved position: %d\n", subpid, startOffset);
						
						int i;
						for (i = startOffset; i <= endOffset; i++) {
							int temp = i;

							fread(&temp, sizeof(int), 1, readF);

							if (i == startOffset) {
								min = temp;
								max = temp;
								printf("%i: %i : %d\t\n", pid, temp, i);

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

							printf("%i: %i : %d\t\n", subpid, temp, i);
						}
					}

					write(pipes[k + 4][1], &min, sizeof(min));
					write(pipes[k + 4][1], &max, sizeof(max));

					printf("Subprocess: %d gave %d as min and %d as max\n", subpid, min, max);
					printf("Min:%d Max: %d\n", min, max);
					_exit;
				}
				
				printf("A\n");


				parentpid = getpid();
				fseek(readF, 0, SEEK_END); //go to end of file

				printf("B\n");

				long size = ftell(readF); //keep track of last byte in the file

				printf("C\n");

				fseek(readF, 0, SEEK_SET); //go to beginning of file

				printf("D\n");

				num = (int)size / (int)sizeof(int); //How many total numbers in file

				printf("E\n");

				printf("Using 4 fork version");

				printf("F\n");

				startOffset = (k)*((num + 3) / 4); //Will produce index to start at 

				printf("G\n");

				printf("Determined startOffset %d", startOffset);

				printf("H\n");

				if (k != 3) {
					endOffset = (((k + 1)*((num + 3) / 4)) - 1);
				}
				else {
					endOffset = num - 1;
				}

				printf("Parent(%d): Sending file position to child\n", parentpid);
				write(pipes[k][1], &startOffset, sizeof(startOffset));

				int len = read(pipes[k + 4][0], &min, sizeof(min));
				if (len > 0) {
					printf("Parent(%d): Recieved %d from child as min.\n", parentpid, min);
					printf("Parent(%d): Recieved %d from child as max.\n", parentpid, max);
				}
				else
				{
					printf("Parent(%d): Error with len\n", parentpid);
				}

				fclose(readF);
			}
		}
		else {
			int min;
			int max;
			if (readF) {
				int pid = getpid();
				fseek(readF, 0, SEEK_END); //go to end of file
				long size = ftell(readF); //what byte in file am I at?
				fseek(readF, 0, SEEK_SET); //go to beginning of file
				int num = (int)size / (int)sizeof(int);
				printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);
				int i;
				for (i = 0; i < num; i++) {
					int temp = 0;
					fread(&temp, sizeof(int), 1, readF);
					//printf("%i: %i\t", pid, temp);

					if (i == 0) {
						min = temp;
						max = temp;
						//printf("%i: %i : %d\t\n", pid, temp, i);

					}

					if (temp < min) {
						printf("Min was: %d Is now: %d\n", min, temp);
						min = temp;
					}

					if (temp > max) {
						printf("Max was: %d Is now: %d\n", max, temp);
						max = temp;
					}
				}
				printf("Min: %d Max: %d\n", min, max);

				fclose(readF);
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