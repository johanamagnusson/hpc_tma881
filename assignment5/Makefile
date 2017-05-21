OBJS = dijkstra.o
LIBS = -lm
CC = gcc
CFLAGS = -O2
.PHONY : all

all : dijkstra

heat_diffusion : $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

dijkstra.o : dijkstra.c

clean :
	rm -f dijkstra dijkstra.o hello

