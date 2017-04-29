OBJS = newton.o
LIBS = -lm -lpthread
CC = gcc
CFLAGS = -O3
.PHONY : all

all : newton

newton : $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

newton.o : newton.c

clean :
	rm -f newton newton.o *.ppm
