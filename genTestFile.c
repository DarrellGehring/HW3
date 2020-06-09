#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char * argv[])
{
	const char * filename = "test.txt";
	FILE * ft = fopen(filename, "wb");
	srand(time(NULL));
	if (ft) {
		int i;
		for (i = 0; i < 1000000; i++) {
			int randomnum = rand() % 1000000;
			fwrite(&randomnum, sizeof(int), 1, ft);
		}
		fclose(ft);
	}
	return 0;
}