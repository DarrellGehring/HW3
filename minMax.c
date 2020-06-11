#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(int numArgs, char *args[]) {
	//printf("Here0");

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
			int startOffset = 0, endOffset = 0, num, min, max;

			int j;
			for (j = 0; j < 8; j++) {
				pipe(pipes[j]);
			}

			int minMin = -1, maxMax = -1;

			int k;
			for (k = 0; k < 4; k++) {

				if ((subpid = fork()) == 0) {
					//printf("[k=%d] (CHILD) Reading startOffset", k);
					int bytesRead = read(pipes[k][0], &startOffset, sizeof(startOffset));
					if (bytesRead <= 0) {
						//printf("[k=%d] (CHILD) Failed to read startOffset", k);
						_exit;
					}
					else {
						//printf("[k=%d] (CHILD) Read startOffset = %d", k, startOffset);
					}
										
					//printf("[k=%d] (CHILD) Reading endOffset", k);
					bytesRead = read(pipes[k][0], &endOffset, sizeof(endOffset));
					if (bytesRead <= 0) {
						//printf("[k=%d] (CHILD) Failed to read endOffset", k);
						_exit;
					}
					else {
						//printf("[k=%d] (CHILD) Read endOffset = %d", k, endOffset);
					}
					
					if (bytesRead > 0) {
						subpid = getpid();

						fseek(readF, (startOffset*(int)sizeof(int)), SEEK_SET);

						//printf("[k=%d] Child(%d): Recieved start position: %d\n", k, subpid, startOffset);
						//printf("[k=%d] Child(%d): Recieved end position: %d\n", k, subpid, endOffset);
						
						int i;
						for (i = startOffset; i <= endOffset; i++) {
							int temp = 0;
							fread(&temp, sizeof(int), 1, readF);

							if (i == startOffset) {
								min = temp;
								max = temp;
								//printf("[k=%d] %i: %i : %d\t\n", k, subpid, temp, i);
							}
							else if (i == endOffset) {
								//printf("[k=%d] %i: %i : %d\t\n", k, subpid, temp, i);
							}

							if (temp < min) {
								min = temp;
							}

							if (temp > max) {
								max = temp;
							}

							//printf("[k=%d] (CHILD) %i: %i : %d\t\n", k, subpid, temp, i);
						}
					}

					write(pipes[k + 4][1], &min, sizeof(min));
					write(pipes[k + 4][1], &max, sizeof(max));

					//printf("[k=%d] (CHILD) Subprocess: %d gave %d as min and %d as max\n", k, subpid, min, max);
					//printf("[k=%d] Min:%d Max: %d\n", k, min, max);
					_exit;
				}
				
				//printf("A\n");


				parentpid = getpid();
				fseek(readF, 0, SEEK_END); //go to end of file

				//printf("B\n");

				long size = ftell(readF); //keep track of last byte in the file

				//printf("C\n");

				fseek(readF, 0, SEEK_SET); //go to beginning of file

				//printf("D\n");

				num = (int)size / (int)sizeof(int); //How many total numbers in file

				//printf("E\n");

				startOffset = (k)*((num + 3) / 4); //Will produce index to start at 

				//printf("F\n");

				//printf("[k=%d] Determined startOffset %d\n", k, startOffset);

				//printf("G\n");

				if (k != 3) {
					endOffset = (((k + 1)*((num + 3) / 4)) - 1);
				}
				else {
					endOffset = num - 1;
				}

				//printf("[k=%d] Determined endoffset %d\n", k, endOffset);

				//printf("[k=%d] Parent(%d): Sending startOffset to child\n", k, parentpid);
				if (write(pipes[k][1], &startOffset, sizeof(startOffset)) < 0) {
					//printf("[k=%d] Send of startOffset Failed!", k);
					_exit;
				}
				
				//printf("[k=%d] Parent(%d): Sending endOffset to child\n", k, parentpid);
				if (write(pipes[k][1], &endOffset, sizeof(endOffset)) < 0) {
					//printf("[k=%d] Send of endOffset Failed!", k);
					_exit;
				}
								
				//printf("[k=%d] Waiting for child read", k);

				int bytesRead = read(pipes[k + 4][0], &min, sizeof(min));
				if (bytesRead > 0) {
					//printf("[k=%d] Parent(%d): Recieved %d from child as min.\n", k, parentpid, min);
				}
				else {
					//printf("[k=%d] Parent(%d): Error Reading Min from child.\n", k, parentpid);
				}

				bytesRead = read(pipes[k + 4][0], &max, sizeof(max));
				if (bytesRead > 0) {
					//printf("[k=%d] Parent(%d): Recieved %d from child as max.\n", k, parentpid, max);
				} else {
					//printf("[k=%d] Parent(%d): Error Reading Max from child.\n", k, parentpid);
				}

				if (min < minMin || minMin == -1) {
					//printf("[k=%d] Set minMin = %d", k, min);
					minMin = min;
				}
				if (max < maxMax || maxMax == -1) {
					//printf("[k=%d] Set maxMax = %d", k, max);
					maxMax = max;
				}
			}
			printf("Min: %d Max: %d\n", min, max);
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
				//printf("size of the file: %li ,sizeof(int) = %i\n, the number of numbers = %i\n\n", size, (int) sizeof(int), num);
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
						//printf("Min was: %d Is now: %d\n", min, temp);
						min = temp;
					}

					if (temp > max) {
						//printf("Max was: %d Is now: %d\n", max, temp);
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