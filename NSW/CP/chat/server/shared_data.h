#ifndef SHARED_DATA_H
#define SHARED_DATA_H


#include <resolv.h>


#include "list.h"


enum {
        NO_TYPE    , INFO       , CONNECT    , RECONNECT  ,
        DISCONNECT , CHAT       , INFO_MSG 
     };

enum {
        ADD_USER   , REMOVE_USER, EDIT_USER
     };


#define NUM_OF_SERVERS   5

#define CUR_SERVER       3 

#define TIMEOUT_ACCEPT   60
#define TIMEOUT_SEND     5
#define TIMEOUT_RECV     5
#define CONF_FNAME       "servers.conf"

#define MAX_MSG_SIZE     50
#define MAX_HISTORY_SIZE 10

#define MAX_USERS        20

#define TRY_SEND         1
#define TRY_RECV         1


struct list             *history;
struct list             *users;

int                      state_thr[MAX_USERS];

int                      SERVER_TO_CLIENT_SOCKET;
int                      SERVER_TO_SERVER_SOCKET;

struct sockaddr_in       SERVER;
struct sockaddr_in       SERVERS[NUM_OF_SERVERS - 1];


#endif
