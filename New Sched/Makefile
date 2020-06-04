all: complibs compmain compstats

complibs:
	clang -c Scheduler.c PIP.c tests.c aux1.c

compmain:
	clang Main.c Scheduler.o PIP.o tests.o aux1.o -lm -o main

compstats:
	clang statistics.c -o stats