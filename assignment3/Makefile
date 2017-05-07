OBJS = cell_distances.o
OBJS2 = make_cell.o
LIBS = -lgomp -fopenmp -lm
CC = gcc
CFLAGS = -O3
.PHONY : all

all : cell_distances

cell : make_cell

cell_distances : $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)
cellscript : $(OBJS2)
	$(CC) -o $@ $(CFLAGS) $(OBJS2)

cell_distances.o : cell_distances.c
make_cell.o : make_cell.c

clean :
	rm -f cell_distances cell_distances.o

cleanall :
	rm -f cell_distances cell_distances.o make_cell make_cell.o
