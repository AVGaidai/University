#include <stdio.h>
#include <unistd.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>


#include "error.h"



int tcp_sock_create (const char *ipaddr, uint16_t port)
{
    int sockfd;

    sockfd = socket (PF_INET, SOCK_STREAM, 0);
  
    if ( sockfd == -1 ) {
        return print_err ("tcp_sock_create ()...");
    }

    struct sockaddr_in addr;

    bzero (&addr, sizeof (struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    
    if ( inet_aton (ipaddr, &addr.sin_addr) == 0 ) {
        return print_err ("tcp_sock_create ()...");
    }

    int status;

    status = bind ( sockfd, (const struct sockaddr *) &addr, 
                    (socklen_t) sizeof (addr)                );

    if ( status == -1 ) {
        return print_err ("tcp_sock_create ()...");
    }

    return sockfd;
}


int tcp_sock_remove (int sockfd)
{
    return close (sockfd);
}


int tcp_connect (int sockfd, const char *ipaddr, uint16_t port)
{
    struct sockaddr_in addr;

    bzero (&addr, sizeof (struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    
    if ( inet_aton (ipaddr, &addr.sin_addr) == 0 ) {
        return print_err ("tcp_connect ()...");
    }

    int status;

    status = connect ( sockfd, (const struct sockaddr *) &addr,
                       (socklen_t) sizeof (addr)                );

    return (status) ? print_err ("tcp_connect ()...") : 0;
}

int tcp_listen (int sockfd, int backlog)
{
    int status;

    status = listen (sockfd, backlog);

    return (status) ? print_err ("tcp_listen ()...") : 0;
}

int tcp_accept (int sockfd, char *ipaddr, uint16_t *port)
{
    struct sockaddr_in addr;

    socklen_t addrlen;

    bero (&addr, addrlen);

    int status;

    status = accept (sockfd, &addr, &addrlen);

    if (status) return print_err ("tcp_accept ()...");

    char *tmp;

    bzero (ipaddr, 16);
    tmp = inet_ntoa (addr.sin_addr);

    memcpy (ipaddr, tmp, 16);
    *port = ntohs (addr.sin_port);

    return 0;
}


int tcp_send_msg (int sockfd, const void *buf, size_t len)
{
    int s_bytes;

    s_bytes = send (sockfd, buf, len, 0);

    return (s_bytes <= 0) ? print_err ("tcp_send_msg ()...") : s_bytes;
}


int tcp_recv_msg (int sockfd, void *buf, size_t len)
{
    int r_bytes;

    r_bytes = recv (sockfd, buf, len, 0);

    return (r_bytes <= 0) ? print_err ("tcp_recv_msg ()...") : r_bytes;
}

