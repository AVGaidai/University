#ifndef TCP_FUNC
#define TCP_FUNC

#include <unistd.h>
#include <stdint.h>

int tcp_sock_create (const char *, uint16_t);
int tcp_sock_remove (int);
int tcp_connect (int, const char *, uint16_t);
int tcp_listen (int, int);
int tcp_accept (int, char *, uint16_t *);
int tcp_send_msg (int, const void *, size_t);
int tcp_recv_msg (int, void *, size_t);

#endif
