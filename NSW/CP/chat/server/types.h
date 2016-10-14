#ifndef USER_TYPES_H
#define USER_TYPES_H

#include <netinet/in.h>

#include "shared_data.h"

struct __header {

    int              type;
    long int         size;

};


struct __login {

    struct in_addr   addr;
    int              pid;

};


struct __chat {

    struct __login   login;

    char             buf[MAX_MSG_SIZE];

};

struct __serv_msg {

    struct __login   login;

    int              type;
    char             buf[MAX_MSG_SIZE];

};


struct __user {

    struct __login   login;

    int              sd;

};


struct __recv_msg {

    struct __user    user;

    int              ind;

};


#endif
