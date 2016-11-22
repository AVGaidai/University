#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "error.h"
#include "TCP_FUNC.h"


#define SERVER_IP      "127.0.0.1"
#define SERVER_PORT    8888


char IP_ADDR[16] = "127.0.0.1";
uint16_t PORT = 8887;


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

    if (tcp_connect (sockfd, SERVER_IP, SERVER_PORT)) {
        tcp_sock_remove (sockfd);
        return -1;
    }

    int s_bytes;
    int r_bytes;

    FILE *fp;

    fp = fopen ("msg.bin", "rb");
    if (fp <= 0) {
        return tcp_sock_remove (sockfd);
    }

    int  fbuf;
    
    r_bytes = fread (&fbuf, sizeof (int), 1, fp);

    while (r_bytes > 0) {
        s_bytes = tcp_send_msg (sockfd, &fbuf, sizeof (int));
        printf ("send bytes: %d\n", s_bytes);

        if (s_bytes <= 0) {
            r_bytes = 1;
            print_info ("transmission failure!");
            continue;
        }

        r_bytes = fread (&fbuf, sizeof (int), 1, fp);
        sleep (1);
    }

    fclose (fp);
    tcp_sock_remove (sockfd);

    return 0;
}
