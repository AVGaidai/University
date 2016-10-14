#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#include "UDP_FUNC.h"

char IP_ADDR[16] = "127.0.0.1";

uint16_t PORT = 7777;


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

    int s_bytes;

    for (int i = 0; i < 10; ++i) {
        s_bytes = udp_send_msg ( sockfd, "127.0.0.1", 7778,
                                 &sockfd, sizeof (int)      );
    
        printf ("send bytes: %d\n", s_bytes);
    }

    udp_sock_remove (sockfd);

    return 0;
}
