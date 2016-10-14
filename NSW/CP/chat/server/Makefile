CC=gcc
CFLAGS=-Wall -g -pthread -o

MAIN=main.out main.c

SERVER=server_func.c server_func.h
MSGHANDLER=msghandler.c msghandler.h
ERROR=error.c error.h
USER_SOCK=user_socket.c user_socket.h
NETWORK=network.c network.h
LIST=list.c list.h
SHARED=shared_data.h
TYPES=types.h

all:
	$(CC) $(CFLAGS) $(MAIN) $(ERROR) $(LIST)\
 $(MSGHANDLER) $(NETWORK) $(USER_SOCK) $(SERVER) $(SHARED) $(TYPES)

clean_out:
	rm *.out
