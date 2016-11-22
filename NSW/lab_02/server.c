#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include "TCP_FUNC.h"


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

    int client_sock;

    tcp_listen (sockfd, 20);

    while (1) {
        client_sock = tcp_accept (sockfd, ipaddr, &port);
        printf ("Client %s:%hd connected!\n", ipaddr, port);
        fork ();
        if (getpid () != main_pid) break;
    }

    char BUF[BUF_SIZE];

    int r_bytes;

    while (1) {
        r_bytes = tcp_recv_msg (client_sock, &BUF, BUF_SIZE);
        sleep (1);

        if (r_bytes == 0) {
            tcp_sock_remove (client_sock);
            return 0;
        }

        BUF[r_bytes] = '\0';

        printf ("recv bytes: %d\n", r_bytes);       
        printf ("data: %s\n", BUF);
        printf ("from \"%s:%hd\"\n", ipaddr, port);
    }

    return 0;
}
