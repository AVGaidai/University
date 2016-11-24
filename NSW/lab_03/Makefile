CC=gcc
CFLAGS=-Wall -g

CLIENT=client.out client.c
SERVER=server.out server.c

all:
	$(CC) $(CFLAGS) -o $(CLIENT) -lsctp
	$(CC) $(CFLAGS) -o $(SERVER) -lsctp

clean_out:
	rm *.out
