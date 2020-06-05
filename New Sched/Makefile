all: complibs compmain compstats

complibs:
	gcc -c Scheduler.c PIP.c tests.c aux1.c

compmain:
	gcc -c Main.c Scheduler.o PIP.o tests.o aux1.o -lm -o main

compstats:
	gcc -c statistics.c -o stats
