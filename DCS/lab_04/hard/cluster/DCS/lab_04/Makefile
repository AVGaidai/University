CC=gcc
MPICC=mpicc
CFLAGS1=-Wall
CFLAGS2=-Wall -std=c99 -fopenmp

PROG1=Mul_Matrix
PROG2=verification

all:
	$(MPICC) $(CFLAGS2) -o $(PROG1).out $(PROG1).c
	$(CC) $(CFLAGS1) -o $(PROG2).out $(PROG2).c

clean:
	rm -f *.o *.out *.i out_file* set_*.txt cachegrind.out.*

