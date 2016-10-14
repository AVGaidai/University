CC=gcc
CFLAGS=-Wall -g -pthread -o

MAIN=main.out main.c
CHAT=send_msg.out send_msg.c

CLIENT=client_func.c client_func.h
MSGHANDLER=msghandler.c msghandler.h
ERROR=error.c error.h
USER_SOCK=user_socket.c user_socket.h
NETWORK=network.c network.h
SHARED=shared_data.h
TYPES=types.h

all:
	$(CC) $(CFLAGS) $(MAIN) $(ERROR)\
 $(MSGHANDLER) $(NETWORK) $(USER_SOCK) $(CLIENT) $(SHARED) $(TYPES)
	$(CC) $(CFLAGS) $(CHAT) $(SHARED)

clean_out:
	rm *.out
