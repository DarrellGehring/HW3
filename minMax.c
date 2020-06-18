#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int numArgs, char *args[]) {
	// printf("Starting Main\n");

	if (*args[1] != '1' && *args[1] != '4') {
		printf("Error: first argument must be 1 or 4 in minMax. Was: %s\n", args[1]);
		return -1;
	}

	const char *filename = args[2];
	// printf("Opening file:\n");
	FILE * file = fopen(filename, "r");
	int minMin, maxMax;

	if (file) {
		if (*args[1] == '4') {
			// printf("Here!\n");
			// printf("Finished.\n");

			int fd[8][2]; //parent+child pipe
			// printf("Here2!\n");
			int i, len, fpos = 0, count = 0, min = -1, max = -1;
			// printf("Here3!\n");
			pid_t pid;

			// printf("Here4!\n");

			fseek(file, 0, SEEK_END); //go to end of file
			long size = ftell(file); //what byte in file am I at?
			fseek(file, 0, SEEK_SET); //go to beginning of file

			int nums = (int)size / (int)sizeof(int);
			// printf("Here5!\n");

			// create all pipes
			// printf("Hopefully will make 4 pipes.\n");


			for (i = 0; i < 8; i++) {
				pipe(fd[i]);
				// printf("Opening pipes for %d!\n", i);
			}

			// printf("After Pipes!\n");

			for (i = 0; i < 4; i++)
			{
				if ((pid = fork()) == 0) // child process
				{
					pid = getpid();

					len = read(fd[i][0], &fpos, sizeof(fpos));
					if (len > 0)
					{
						fseek(file, fpos*(int)sizeof(int), SEEK_SET);
						count = 0;

						while (count < (nums / 4))
						{
							int temp = 0;
							fread(&temp, sizeof(int), 1, file);

							if (count == 0) {
								min = temp;
								max = temp;
							}
							else {
								if (max < temp) {
									max = temp;
								}

								if (min > temp) {
									min = temp;
								}
							}

							count++;
						}
						//write to parent
						write(fd[i + 4][1], &min, sizeof(min));
						write(fd[i + 4][1], &max, sizeof(max));
					}
					else
					{
					}

					exit(0);
				}

				// parent process
				pid = getpid();

				fpos = ((i*nums) / 4);

				write(fd[i][1], &fpos, sizeof(fpos));

				// wait for child responce
				len = read(fd[i + 4][0], &min, sizeof(min));

				if (len > 0)
				{

					if (i == 0) {
						minMin = min;
					}
					else {
						if (minMin > min) {
							minMin = min;
						}
					}
					//total += total;
				}
				else
				{
					printf("Parent(%d): Error with len\n", pid);
				}

				len = read(fd[i + 4][0], &max, sizeof(max));

				if (len > 0)
				{
					if (i == 0) {
						maxMax = max;
					}
					else {
						if (maxMax < max) {
							maxMax = max;
						}
					}
					//total += total;
				}
				else
				{
					printf("Parent(%d): Error with len\n", pid);
				}
			}
		}
		else {
			fseek(file, 0, SEEK_END); //go to end of file
			long size = ftell(file); //what byte in file am I at?
			fseek(file, 0, SEEK_SET); //go to beginning of file
			int num = (int)size / (int)sizeof(int);
			int i;
			for (i = 0; i < num; i++) {
				int temp = 0;
				fread(&temp, sizeof(int), 1, file);
				//printf("%i: %i\t", pid, temp);

				if (i == 0) {
					minMin = temp;
					maxMax = temp;
					//printf("%i: %i : %d\t\n", pid, temp, i);
				}

				if (temp < minMin) {
					//printf("Min was: %d Is now: %d\n", min, temp);
					minMin = temp;
				}

				if (temp > maxMax) {
					//printf("Max was: %d Is now: %d\n", max, temp);
					maxMax = temp;
				}
			}
		}
	} else {
		printf("Failed to read given file!");
	}
	fclose(file);
	printf("Minimum: %d\t Maximum: %d\n", minMin, maxMax);

	return 0;
}