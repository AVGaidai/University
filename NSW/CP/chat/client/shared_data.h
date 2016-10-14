#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <sys/un.h>
#include <resolv.h>


enum {
        NO_TYPE   , INFO      , CONNECT   , RECONNECT ,
        DISCONNECT, CHAT      , INFO_MSG 
     };


#define NUM_OF_SERVERS   5

int                      CUR_SERVER;

#define TIMEOUT_CONNECT  5
#define TIMEOUT_SEND     5
#define TIMEOUT_RECV     5
#define CONF_FNAME       "servers.conf"

#define MAX_MSG_SIZE     50

#define ADDR             "127.0.0.1"

int                      PID;


#define TRY_CONNECT      3
#define TRY_SEND         1
#define TRY_RECV         1


int                      CLIENT_SOCKET;

struct sockaddr_in       SERVERS[NUM_OF_SERVERS];


#endif
