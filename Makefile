CC=gcc
CFLAGS= -Wall -std=c99
SRC=$(wildcard *.c)
EXE = $(SRC:.c=)

all: $(EXE)

clean:
	 rm -f $(EXE) *~
