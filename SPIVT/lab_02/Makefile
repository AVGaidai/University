CC=gcc
CFLAGS=-Wall -g
FI=fi_0.c fi_1.c fi_2.c fi.h
SF=support_func.c support_func.h
MTF=MTF.c MTF.h

MAIN=main

all:
	$(CC) $(CFLAGS) -o $(MAIN).out $(MAIN).c $(MTF) $(FI) $(SF)

clean:
	rm -f *.out *.bin
