CC = gcc
CFLAGS = -Wall -ansi -pedantic
CFLAGSEX =
EXE = main

all: $(EXE)

run:
	./main
	
debug:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main --debug

clean:
	rm -f *.o $(EXE) *.txt *.dot

$(EXE): %: %.o afnd.o minimiza.o int_list.o state_list.o
	$(CC) $(CFLAGS) -o $@ $@.o afnd.o minimiza.o int_list.o state_list.o

afnd.o: afnd.c afnd.h
	$(CC) $(CFLAGSEX) -c $<

minimiza.o: minimiza.c minimiza.h
	$(CC) $(CFLAGS) -c $<

int_list.o: int_list.c int_list.h
	$(CC) $(CFLAGS) -c $<

state_list.o: state_list.c state_list.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c
	$(CC) $(CFLAGS) -c $<
