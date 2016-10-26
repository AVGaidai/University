CC=gcc
CFLAGS=-Wall -g

CLIENT=client.out client.c
SERVER=server.out server.c

ERROR=error.c error.h
UDP_F=UDP_FUNC.c UDP_FUNC.h
SUP_F=support_func.c support_func.h


all:
	$(CC) $(CFLAGS) -o $(CLIENT) $(UDP_F) $(SUP_F) $(ERROR)
	$(CC) $(CFLAGS) -o $(SERVER) $(UDP_F) $(SUP_F) $(ERROR)

clean_out:
	rm *.out
