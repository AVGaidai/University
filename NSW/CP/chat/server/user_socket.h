#ifndef NBLCK_SOCKET_H
#define NBLCK_SOCKET_H

#include <sys/socket.h>


int  nblck_sock_mode (int, int);
int  connect_nblck (int, struct sockaddr *, socklen_t, int);
int  accept_nblck ( int sd, struct sockaddr *, socklen_t *, int);
int  send_nblck (int, void *, long int, int);
int  recv_nblck (int, void *, long int, int);

int  sendto_broadcast ( int, const void *, size_t, int ,
                        const struct sockaddr *, socklen_t );


#endif
