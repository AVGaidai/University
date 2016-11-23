#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "error.h"
#include "TCP_FUNC.h"
#include "network.h"



char     SERV_IPADDR[16] = "127.0.0.1\0";
uint16_t SERV_PORT = 8888;

char     HOST_IPADDR[16] = "127.0.0.1\0";
uint16_t HOST_PORT;


int main (int argc, char *argv[])
{
    if (argc >= 2) {
        memmove (SERV_IPADDR, argv[1], 16);
        printf ("%s\n", SERV_IPADDR);
    }

    if (argc == 3) {
        SERV_PORT = (uint16_t) atoi (argv[2]);
    }

    int sockfd;

    sockfd = tcp_sock_create (HOST_IPADDR, &HOST_PORT);

    if (sockfd == -1) return -1;

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hu\"\n"
             "====================================\n", 
                        sockfd, HOST_IPADDR, HOST_PORT );

    if (tcp_connect (sockfd, SERV_IPADDR, SERV_PORT)) {
        tcp_sock_remove (sockfd);
        return -1;
    }


    char fname[20];
    int i;

    while(1) {
        i = 0;
        printf ("Input fname: ");    
        do {
            scanf ("%c", &fname[i++]);
        } while (i < 19 && fname[i - 1] != '\n');
        fname[i - 1] = '\0';

        if (!strcmp (fname, "//exit//")) break;
        send_file (sockfd, fname);
    }

    tcp_sock_remove (sockfd);

    return 0;
}
