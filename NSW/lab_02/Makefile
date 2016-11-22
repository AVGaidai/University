CC=gcc
CFLAGS=-Wall -g

CLIENT=client.out client.c
SERVER=server.out server.c

ERROR=error.c error.h
TCP_F=TCP_FUNC.c TCP_FUNC.h
SUP_F=support_func.c support_func.h

all:
	$(CC) $(CFLAGS) -o $(CLIENT) $(TCP_F) $(SUP_F) $(ERROR)
	$(CC) $(CFLAGS) -o $(SERVER) $(TCP_F) $(SUP_F) $(ERROR)

clean_out:
	rm *.out
