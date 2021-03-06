#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main(int numArgs, char *args[]) {
	
	if (numArgs != 3) {
		printf("Error: requires two arguments.\n");
		return -1;
	}

	if (*args[1] != '1' && *args[1] != '4') {
		printf("Error: first argument must be 1 or 4.\n");
		return -1;
	}

	printf("Darrell Gehring\n");

	int cp[2];

	clock_t start = clock();

	if (*args[1] == '1') {
		if (pipe(cp) < 0)
		{
			printf("didn't work, couldn't not establish pipe.\n");
			return -1;
		}

		char* filePath = args[2];
		int pid = fork();
		if (pid == 0) {
			close(1); //close stdout
			dup2(cp[1], 1); //move stdout to pipe of cp[1]
			close(0); //close stdin
			close(cp[0]); //close pipe in

			int retVal = execl("minMax", "minMax", "1", filePath, NULL);  //note: All the arguments in exec have to be strings.

			if (retVal == -1) {
				printf("exec was unsuccessful");
			}
			exit(0);
		}

		close(cp[1]); //if you don't close this part of the pipe then the while loop (three lines down) will never return

		char ch;
		while (read(cp[0], &ch, 1) == 1)
		{
			printf("%c", ch);
		}

	} else if (*args[1] == '4') {
		if (pipe(cp) < 0)
		{
			printf("didn't work, couldn't not establish pipe.\n");
			return -1;
		}

		char* filePath = args[2];

		int pid = fork();
		if (pid == 0)
		{
			close(1); //close stdout
			dup2(cp[1], 1); //move stdout to pipe of cp[1]
			close(0); //close stdin
			close(cp[0]); //close pipe in

			int retVal = execl("minMax", "minMax", "4", filePath, NULL);  //note: All the arguments in exec have to be strings.

			if (retVal == -1) {
				printf("exec was unsuccessful");
			}
		}
		else
		{

			close(cp[1]); //if you don't close this part of the pipe then the while loop (three lines down) will never return
			char ch;
			while (read(cp[0], &ch, 1) == 1)
			{
				printf("%c", ch);
			}
		}
	}

	clock_t finish = clock();

	printf("Took %.9f seconds for %s forks.\n", 1.0*(finish - start) / CLOCKS_PER_SEC, args[1]);

	return 0;
}