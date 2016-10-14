#ifndef UDP_FUNL
#define UDP_FUNC

#include <unistd.h>
#include <stdint.h>

int udp_sock_create (const char *, uint16_t);
int udp_sock_remove (int);
int udp_send_msg (int, const char *, uint16_t, const void *, size_t);
int udp_recv_msg (int, void *, size_t, char *, uint16_t *);

#endif
