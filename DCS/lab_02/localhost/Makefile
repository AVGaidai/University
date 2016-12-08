CC=gcc
CFLAGS=-Wall

TASK_01=task_01
TASK_02=sqrts

all:
	make task_01 ; make task_02

task_01:
	$(CC) $(CFLAGS) -o $(TASK_01).out $(TASK_01).c

task_02:
	$(CC) $(CFLAGS) --save-temps -o $(TASK_02).out $(TASK_02).c -lm

clean:
	rm -f *.o *.out *.i
