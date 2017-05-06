OBJS = cell_distances.o
LIBS = -lm -fopenmp
CC = gcc
CFLAGS = -O3
.PHONY : all

all : cell_distances

cell_distances : $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

cell_distances.o : cell_distances.c

clean :
	rm -f cell_distances cell_distances.o
