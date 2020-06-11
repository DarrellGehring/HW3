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

	int cp[2];

	if (*args[1] == '1') {
		if (pipe(cp) < 0)
		{
			printf("didn't work, couldn't not establish pipe.\n");
			return -1;
		}

		char* filePath = *args[2];
		printf(*args[2]);

		int pid = fork();
		if (pid == 0)
		{
			printf("this is the child. not the original !!!!\n");
			printf(filePath);
			close(1); //close stdout
			dup2(cp[1], 1); //move stdout to pipe of cp[1]
			close(0); //close stdin
			close(cp[0]); //close pipe in
			int retVal = execl("minMax", "minMax", "1", (char *)0);  //note: All the arguments in exec have to be strings.

			if (retVal == -1) {
				printf("exec was unsuccessful");
			}
		}
		else
		{

			close(cp[1]); //if you don't close this part of the pipe then the while loop (three lines down) will never return
			printf("this is the parent. the 'original.'\n");
			char ch;
			while (read(cp[0], &ch, 1) == 1)
			{
				printf("%c", ch);
				//write(1, &ch, 1);
				//outcount++;
			}
			printf("all done.\n");
			//execl("printStuff","printStuff", "10",(char *) 0);
		}
	}

	printf("\n");
	return 0;
}