.PHONY: all clean run

all: leak

leak: leak.o
	gcc -o leak leak.o

leak.o : leak.c

clean :
	rm -f leak leak.o

run :
	valgrind --leak-check=yes --log-file=valgrindout.txt ./leak
