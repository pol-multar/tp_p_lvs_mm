CC=gcc
CFLAGS= -Wall -std=c99
SRC=$(wildcard *.c)
EXE = $(SRC:.c=)

test: test.o stdio.o stdio.h
	$(CC) -o test test.o stdio.o

test.o: test.c stdio.h
	$(CC) $(CFLAGS) -c test.c

stdio.o: stdio.c stdio.h
	$(CC) $(CFLAGS) -c stdio.c

clean:
	 rm -f $(EXE) *~
