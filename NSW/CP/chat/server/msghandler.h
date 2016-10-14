#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "types.h"

int handler_client_msg (struct __user, int, void *);
int handler_server_msg (struct __user, int, void *);


#endif
