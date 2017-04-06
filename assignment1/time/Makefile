OBJS       = timeO0.o timeO1.o timeO2.o timeO3.o timeOs.o timeOg.o
EXE        = $(OBJS:.o=)
ASS	       = $(OBJS:.o=.s)
CC         = gcc
CFLAGSLIST = -O0 -O1 -O2 -O3 -Os -Og

.PHONY: all clean assembler time

all : $(EXE)

assembler : $(ASS)

timeO0.o : sum.c
	$(CC) -O0 -c $< -o $@
timeO1.o : sum.c
	$(CC) -O1 -c $< -o $@
timeO2.o : sum.c
	$(CC) -O2 -c $< -o $@
timeO3.o : sum.c
	$(CC) -O3 -c $< -o $@
timeOs.o : sum.c
	$(CC) -Os -c $< -o $@
timeOg.o : sum.c
	$(CC) -Og -c $< -o $@

time% : time%.o
	$(CC) -o $@ $<

timeO0.s : sum.c
	$(CC) -S -O0 -c $< -o $@
timeO1.s : sum.c
	$(CC) -S -O1 -c $< -o $@
timeO2.s : sum.c
	$(CC) -S -O2 -c $< -o $@
timeO3.s : sum.c
	$(CC) -S -O3 -c $< -o $@
timeOs.s : sum.c
	$(CC) -S -Os -c $< -o $@
timeOg.s : sum.c
	$(CC) -S -Og -c $< -o $@

clean :
	rm -f  *.o assembler *.s $(EXE)

time :
	@echo "All benchmarks run in order O0, O1, O2, O3, Og, Os"
	@$(foreach exe, $(EXE),  ./$(exe);)
