#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <time.h>

#include "UDP_FUNC.h"


#define BUF_SIZE 20

char IP_ADDR[16] = "127.0.0.1";

uint16_t PORT = 7778;


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

    sockfd = udp_sock_create (IP_ADDR, PORT);

    if (sockfd == -1) return -1;

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hd\"\n"
             "====================================\n",
                                  sockfd, IP_ADDR, PORT );


    char BUF[BUF_SIZE];
    char ipaddr[16];

    uint16_t port;

    int r_bytes;
    char answ = 0x1;

    srand (time (NULL));

    while (1) {
        r_bytes = udp_recv_msg (sockfd, BUF, BUF_SIZE, ipaddr, &port);

        if (rand () % 10 < 2) continue;

        printf ("recv bytes: %d\n", r_bytes);       
        printf ("data: %s\n", BUF);
        printf ("from \"%s:%hd\"\n", ipaddr, port);

        udp_send_msg (sockfd, ipaddr, port, &answ, port);
    }

    udp_sock_remove (sockfd);

    return 0;
}
