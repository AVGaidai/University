#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#include "UDP_FUNC.h"
#include "support_func.h"
#include "error.h"


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
    int r_bytes;

    FILE *fp;

    fp = fopen ("msg.bin", "rb");
    if (fp <= 0) {
        return udp_sock_remove (sockfd);
    }

    set_recv_timer (sockfd, 2);

    int  fbuf;
    char answ;
    
    char ipaddr[16];
    uint16_t port;

    r_bytes = fread (&fbuf, sizeof (int), 1, fp);

    while (r_bytes > 0) {
        s_bytes = udp_send_msg ( sockfd, "127.0.0.1", 7778,
                                 &fbuf, sizeof (int)        );
        printf ("send bytes: %d\n", s_bytes);

        r_bytes = udp_recv_msg ( sockfd, &answ, sizeof (char), ipaddr, &port);
        if (r_bytes <= 0) {
            r_bytes = 1;
            print_info ("transmission failure!");
            continue;
        }

        r_bytes = fread (&fbuf, sizeof (int), 1, fp);
        sleep (1);
    }

    fclose (fp);
    udp_sock_remove (sockfd);

    return 0;
}
