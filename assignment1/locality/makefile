CC     = gcc
CFLAGS = -O3

.PHONY : all

all : locality

locality : locality.o
	$(CC) -o $@ $(CFLAGS) locality.o

locality.o : locality.c

clean : 
	rm -f locality locality.o
