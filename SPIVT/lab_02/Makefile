CC=gcc
CFLAGS=-Wall -g
FI=fi_0.c fi_1.c fi_2.c fi.h
SF=support_func.c support_func.h

MAIN=MTF

all:
	$(CC) $(CFLAGS) -o $(MAIN).out $(MAIN).c $(FI) $(SF)

clean:
	rm -f *.out *.bin
