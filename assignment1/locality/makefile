CC     = gcc
CFLAGS = -O0

.PHONY : all

all : locality

locality : locality.o
	$(CC) -o $@ $(CFLAGS) locality.o

locality.o : locality.c
