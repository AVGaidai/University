CC=gcc
MPICC=mpicc
CFLAGS1=-Wall
CFLAGS2=-Wall -std=c99

PROG1=main
PROG2=Mul_Matrix
PROG3=verification

all:
	$(MPICC) $(CFLAGS1) -o $(PROG1).out $(PROG1).c
	$(MPICC) $(CFLAGS2) -o $(PROG2).out $(PROG2).c
	$(CC) $(CFLAGS1) -o $(PROG3).out $(PROG3).c

clean:
	rm -f *.o *.out *.i out_file* set_*.txt cachegrind.out.*

