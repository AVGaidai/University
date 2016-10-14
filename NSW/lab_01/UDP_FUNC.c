#include <stdio.h>
#include <unistd.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>


#include "error.h"



int udp_sock_create (const char *ipaddr, uint16_t port)
{
    int sockfd;

    sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  
    if ( sockfd == -1 ) {
        return print_err ("udp_sock_create ()...");
    }

    struct sockaddr_in addr;

    bzero (&addr, sizeof (struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    
    if ( inet_aton (ipaddr, &addr.sin_addr) == 0 ) {
        return print_err ("udp_sock_create ()...");
    }

    int status;

    status = bind ( sockfd, (const struct sockaddr *) &addr, 
                    (socklen_t) sizeof (addr)                );

    if ( status == -1 ) {
        return print_err ("udp_sock_create ()...");
    }

    return sockfd;
}


int udp_sock_remove (int sockfd)
{
    return close (sockfd);
}



int udp_send_msg ( int sockfd, const char *ipaddr,
                   uint16_t port, const void *buf, size_t len )
{
    struct sockaddr_in addr;
 
    bzero (&addr, sizeof (struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);

    if ( inet_aton (ipaddr, &addr.sin_addr) == 0 ) {
        return print_err ("udp_send_msg ()...");
    }

    return sendto ( sockfd, buf, len, 0,
                    (const struct sockaddr *) &addr,
                    (socklen_t) sizeof (addr)        );
}


int udp_recv_msg ( int sockfd, void *buf, 
                   size_t len, char *ipaddr, uint16_t *port )
{
    struct sockaddr_in addr;

    socklen_t addrlen = sizeof (struct sockaddr_in);

    int r_bytes;

    bzero (&addr, addrlen);
    bzero (buf, len);

    r_bytes = recvfrom ( sockfd, buf, len, 0,
                         (struct sockaddr *) &addr, &addrlen );

    if (r_bytes <= 0) {
        return print_err ("udp_recv_msg ()...");
    }


    char *tmp;

    bzero (ipaddr, 16);
    tmp = inet_ntoa (addr.sin_addr);

    memcpy (ipaddr, tmp, 16);
    *port = ntohs (addr.sin_port);

    return r_bytes;
}

