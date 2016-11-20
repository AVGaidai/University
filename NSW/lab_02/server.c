#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "UDP_FUNC.h"


#define BUF_SIZE 20

char IP_ADDR[16] = "127.0.0.1";

uint16_t PORT = 8888;


int main (int argc, char *argv[])
{
    if (argc >= 2) {
        memmove (IP_ADDR, argv[1], 16);
        printf ("%s\n", IP_ADDR);
    }

    if (argc == 3) {
        PORT = (uint16_t) atoi (argv[2]);
    }


    int sockfd;

    sockfd = tcp_sock_create (IP_ADDR, PORT);

    if (sockfd == -1) return -1;

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hd\"\n"
             "====================================\n",
                                  sockfd, IP_ADDR, PORT );

    pid_t main_pid = getpid ();

    char ipaddr[16];

    uint16_t port;

    while (tcp_listen (sockfd, 20)) {
        int cfd = accept (sockfd, ipaddr, &port);
        fork ();
        if (getpid () != main_pid) break;
    }

    char BUF[BUF_SIZE];

    int r_bytes;

    while (1) {
        r_bytes = udp_recv_msg (sockfd, BUF, BUF_SIZE, ipaddr, &port);

        printf ("recv bytes: %d\n", r_bytes);       
        printf ("data: %s\n", BUF);
        printf ("from \"%s:%hd\"\n", ipaddr, port);

        udp_send_msg (sockfd, ipaddr, port, &answ, port);
    }

    udp_sock_remove (sockfd);

    return 0;
}
