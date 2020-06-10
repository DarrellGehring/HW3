all: HW3.c 
	gcc -g -Wall -o HW3 HW3.c
	gcc -g -Wall -o genTestFile genTestFile.c
	gcc -g -Wall -o minMax minMax.c

clean: 
	$(RM) HW3
	$(RM) genTestFile
	$(RM) minMax