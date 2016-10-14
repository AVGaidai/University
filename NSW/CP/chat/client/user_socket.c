/* 
 * Library containing user functions for working with network
 */
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/socket.h>

#include "error.h"


int nblck_sock_mode (int sd, int mode)
{
    int arg;

    arg = 1;

    if (setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof (int)) < 0) {
        return print_err ("SO_REUSEADDR!");
    }


    arg = fcntl (sd, F_GETFL, 0);
    
    if (mode % 2) {
        if (fcntl (sd, F_SETFL, arg | O_NONBLOCK) == -1) {
            return print_err ("fcntl ()!");
        }
    } else {
        if (fcntl (sd, F_SETFL, arg & ~O_NONBLOCK) == -1) {
            return print_err ("fcntl ()!");
        }
    }

    return 0;
}



int connect_nblck ( int sd        , struct sockaddr *server,
                    socklen_t size, int timeout            ,
                    int try                                  )
{
    struct timeval  tv;
    fd_set          fdset;
    int             status;
    
    status = connect (sd, server, size);
    if (status == -1 && errno == EINPROGRESS) {
        tv.tv_sec  = timeout;
        tv.tv_usec = 0;
        FD_ZERO (&fdset);
        FD_SET (sd, &fdset);
        status = select (sd + 1, NULL, &fdset, NULL, timeout ? &tv : NULL);
        if (status == 1) {
            if ( getsockopt ( sd      , SOL_SOCKET, 
                              SO_ERROR, &status   , &size ) == -1 ) {
                return print_err ("getsockopt ()!");
            }
        } else {
            return -1;
        }
    } else {
        return -1;
    }
    
    return status;
}



int accept_nblck ( int sd             , struct sockaddr *addr,
                   socklen_t *addr_len, int timeout            )
{
    int               status;
    struct timeval    tv;
    fd_set            fdset;

    tv.tv_sec  = timeout;
    tv.tv_usec = 0;
    FD_ZERO (&fdset);
    FD_SET  (sd, &fdset);

    status = select (sd + 1, &fdset, NULL, NULL, timeout ? &tv : NULL);

    if (status != 1) {
        return -2;
    }

    status = accept (sd, addr, addr_len);
    if (status == EWOULDBLOCK) {
        return -2;
    }

    return status;
}


/*
 * Send message
 * type    -- type message
 * content -- body message
 * size    -- size body
 */
int send_nblck (int sd, void *content, long int size, int timeout)
{
    if (size == 0 || content == NULL) {
        return print_err ("Sending empty message!");
    }

    int             status;
    struct timeval  tv;
    fd_set          fdset;

    tv.tv_sec  = timeout;
    tv.tv_usec = 0;
    FD_ZERO (&fdset);
    FD_SET  (sd, &fdset);

    status = select (sd + 1, NULL, &fdset, NULL, timeout ? &tv : NULL);
    
    if (status != 1) {
        return -2;
    }

    status = send (sd, content, size, 0);
    if (status == EWOULDBLOCK) {
        return -2;
    } else if (status != size) {
        return -4;
    }

    
    return status;
}


/*
 * Receive message
 * type    -- type message
 * content -- body message
 */
int recv_nblck (int sd, void *content, long int size, int timeout)
{
    int             status;
    struct timeval  tv;
    fd_set          fdset;

    tv.tv_sec  = timeout;
    tv.tv_usec = 0;
    FD_ZERO (&fdset);
    FD_SET  (sd, &fdset);

    status = select (sd + 1, &fdset, NULL, NULL, timeout ? &tv : NULL);
    
    if (status != 1) {
        return -2;
    }

    status = recv (sd, content, size, 0);
    if (status == EWOULDBLOCK) {
        return -2;
    } else if (status != size) {
        return -4;
    }

    return status;
}



int sendto_broadcast ( int sd, const void *buf, size_t len, int flags,
                       const struct sockaddr *dest_addr, socklen_t addrlen )
{
    int N = addrlen / sizeof (struct sockaddr);
    int ret = 0;

    for (int i = 0; i < N; ++i) {
        ret += sendto ( sd           , buf                  ,
                        len          , flags                ,
                        &dest_addr[i], sizeof (dest_addr[i])  );
    }

    return ret;
}

