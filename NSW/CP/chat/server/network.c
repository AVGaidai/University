/* 
 * Library containing user functions for working with network
 */
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "shared_data.h"
#include "user_socket.h"
#include "error.h"
#include "types.h"


/*
 * Send message
 * type    -- type message
 * content -- body message
 * size    -- size body
 */
int send_msg_nblck ( int sd   , int type     ,
                     void *buf, long int size, int timeout )
{
    struct __header   header;  // Header

    header.type = type;
    header.size = size;

    if (size == 0 && buf == NULL) {
        header.size = sizeof (header.type);
        buf = malloc (header.size);
        memcpy (buf, &header.type, header.size);
    } else if (size == 0 || buf == NULL) {
        return print_err ("Sending empty message!");
    }

    int  status;

    for (int i = 0; i < TRY_SEND; ++i) {
        status = send_nblck (sd, &header, sizeof (header), timeout);
        if (status > 0) {
            break;
        }
    }
    
    if (status <= 0) {
        return print_war ("send_msg_nblc ()!");
    }

/* print */
        printf ("send1 %d\n", status);


    for (int i = 0; i < TRY_SEND; ++i) {
        status = send_nblck (sd, buf, header.size, timeout);
        if (status > 0) {
            break;
        }
    }
    
    if (status <= 0) {
        return print_war ("send_msg_nblc ()!");
    }

/* print */
        printf ("send2 %d\n", status);

    if (size == 0) {
        free (buf);
    }
    
    return status;
}


/*
 * Receive message
 * type    -- type message
 * content -- body message
 */
int recv_msg_nblck (int sd, int *type, void *buf, int timeout)
{
    struct __header   header;  // Header of message
    int               status;


    for (int i = 0; i < TRY_RECV; ++i) {
        status = recv_nblck (sd, &header, sizeof (header), timeout);
        if (status > 0) {
            break;
        }
    }

    if (status == -4) {
        return -4;
    }    

    if (status <= 0) {
        return print_war ("recv_msg_nblck ()");
    }

    *type = header.type;   // Define type message
/* print */
        printf ("recv1 %d\n", status);


    for (int i = 0; i < TRY_RECV; ++i) {
        status = recv_nblck (sd, buf, header.size, timeout);
        if (status > 0) {
            break;
        }
    }
    
    if (status <= 0) {
        return print_war ("recv_msg_nblck ()!");
    }

/* print */
        printf ("recv2 %d\n", status);

    return status;
}

