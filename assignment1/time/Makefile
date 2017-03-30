OBJS   = sum.o
CC     = gcc
CFLAGS = -O0 -O1 -O2 -O3 -Os -Og

.PHONY : clean

all : time assembler

time : $(OBJS) 
	$(CC) -o $@ $(OBJS)

assembler : sum.c
	$(CC) -S -o $@ sum.c

sum.o : sum.c

clean:
	rm -f $(OBJS) time assembler *.s
