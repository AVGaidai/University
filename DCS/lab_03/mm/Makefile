CC=gcc
CFLAGS=-Wall -std=c99 -g -fopenmp

PROG=Mul_Matrix

all:
	$(CC) $(CFLAGS) -o $(PROG).out $(PROG).c

verification:
	$(CC) -Wall -o verification.out verification.c

clean:
	rm -f *.out out_file* set_*.txt cachegrind.out.*
