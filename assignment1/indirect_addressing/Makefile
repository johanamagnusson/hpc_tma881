OBJS   = mainO0.o mainO3.o
EXE    = $(OBJS:.o=)
SRC    = indirect_addressing.c
CC     = gcc -std=c99

.PHONY : all clean runall

all : $(EXE)

mainO0.o : $(SRC)
	$(CC) -O0 -c $(SRC) -o $@
mainO3.o : $(SRC)
	$(CC) -O3 -c $(SRC) -o $@

main% : main%.o
	$(CC) -o $@ $<

clean :
	rm -f $(OBJS) $(EXE)

runall :
	@echo "O0"
	@./mainO0
	@echo "O3"
	@./mainO3
	#@$(foreach exe, $(EXE), ./$(exe);)
