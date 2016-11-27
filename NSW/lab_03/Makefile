CC=gcc
CFLAGS=-Wall -g

CLIENT=client.out client.c
SERVER=server.out server.c
SUP_F=support_func.c support_func.h

all:
	$(CC) $(CFLAGS) -o $(CLIENT) -lsctp
	$(CC) $(CFLAGS) -o $(SERVER) $(SUP_F) -lsctp

clean_out:
	rm *.out
