CC = gcc
CFLAGS = -Wall -ansi -pedantic
CFLAGSEX =
EXE = main

all: $(EXE)

clean:
	rm -f *.o $(EXE)

$(EXE): %: %.o afnd.o transforma.o int_list.o
	$(CC) $(CFLAGS) -o $@ $@.o afnd.o transforma.o int_list.o

afnd.o: afnd.c afnd.h
	$(CC) $(CFLAGSEX) -c $<

transforma.o: transforma.c transforma.h
	$(CC) $(CFLAGS) -c $<

int_list.o: int_list.c int_list.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c
	$(CC) $(CFLAGS) -c $<
