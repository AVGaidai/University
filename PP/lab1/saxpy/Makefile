CC := gcc
CFLAGS := -Wall -O2 -g
LDFLAGS := -lm

.PHONY: all clean

all: saxpy daxpy

saxpy: saxpy.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

saxpy.o: saxpy.c

daxpy: daxpy.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

daxpy.o: daxpy.c

clean:
	@rm -rf *.o saxpy daxpy
