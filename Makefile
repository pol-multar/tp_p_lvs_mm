CC=gcc
CFLAGS= -Wall -std=c99
SRC=$(wildcard *.c)
EXE = $(SRC:.c=)

test.o: test.c stdio.h
	$(CC) -c $(CFLAGS) -o test.o test.c

stdio.o: stdio.c stdio.h
	$(CC) -c $(CFLAGS) -o stdio.o stdio.c

clean:
	 rm -f $(EXE) *~
