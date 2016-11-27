#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/sctp.h>

#include <arpa/inet.h>


#include "support_func.h"


char     IPADDR[16] = "127.0.0.1\0";
uint16_t PORT;


struct msg {

    char buf[100];
    int  len;

};

int main (int argc, char *argv[])
{
    int sockfd;

    sockfd = socket (PF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (sockfd <= 0) return -1;

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof (struct sockaddr_in);

    bzero (&addr, sizeof (addr));

    addr.sin_family = AF_INET;
    addr.sin_port = 0;

    int status;

    status = inet_aton (IPADDR, &addr.sin_addr);
    if (!status) {
        close (sockfd);
        return -1;
    }

    status = bind (sockfd, (const struct sockaddr *) &addr, addrlen);

    if (status) {
        close (sockfd);
        return -1;
    }

    status = getsockname (sockfd, (struct sockaddr *) &addr, &addrlen);
    if (status) {
        close (sockfd);
        return -1;
    }
    PORT = ntohs (addr.sin_port);

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hu\"\n"
             "====================================\n",
                                  sockfd, IPADDR, PORT );


    status = listen (sockfd, 100);
    if (status) {
        close (sockfd);
        return -1;
    }

    struct sctp_event_subscribe events;

    bzero (&events, sizeof (events));
    events.sctp_data_io_event = 1;
    setsockopt (sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof (events));

    struct sctp_sndrcvinfo sinfo;

    int r_bytes = 1, msg_flags;
    struct msg msg;

    FILE *fp;
    uint16_t client_port;
    char sport[6];

    while (r_bytes > 0) {
        r_bytes = sctp_recvmsg ( sockfd, &msg, sizeof (msg),
                                 (struct sockaddr *) &addr,
                                 &addrlen, &sinfo, &msg_flags );
        client_port = ntohs (addr.sin_port);
        
        if (sinfo.sinfo_stream == 1) {
            printf ("%hu >> %s\n", client_port, msg.buf);
            continue;
        }

        if (sinfo.sinfo_stream == 0) {
            itoa (client_port, sport);
            int len = strlen (sport);
            sport[len] = '.';
            sport[len + 1] = 'j';
            sport[len + 2] = 'p';
            sport[len + 3] = 'g';
            sport[len + 4] = '\0';
            fp = fopen (sport, "a+b");
        //printf ("context %u\n", sinfo.sinfo_context);
//        printf ("msg.len = %d\n", msg.len);
//        printf ("r_bytes: %d\n", r_bytes);
            fwrite ((const char *) msg.buf, 1, msg.len, fp);
            fclose (fp);
        }
    }

    close (sockfd);

    return 0;
}
