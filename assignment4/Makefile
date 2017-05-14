OBJS = heat_diffusion.o
OBJS2 = make_cell.o
LIBS = -lm -lOpenCL
CC = gcc -I /usr/local/cuda-7.5/targets/x86_64-linux/include -L /usr/local/cuda-7.5/targets/x86_64-linux/lib
CFLAGS = -O2
.PHONY : all

all : heat_diffusion

heat_diffusion : $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

hello : hello.c
	$(CC) -o $@ $< -lOpenCL

heat_diffusion.o : heat_diffusion.c

clean :
	rm -f heat_diffusion heat_diffusion.o hello

