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


char     SERV_IPADDR[16];
uint16_t SERV_PORT;

char FNAME[20];


int main (int argc, char *argv[])
{
    if (argc < 4) {
        return -1;
    }


    memmove (SERV_IPADDR, argv[1], strlen (argv[1]));
    SERV_IPADDR[strlen (argv[1])] = '\0';
    SERV_PORT = (uint16_t) atoi (argv[2]);
    memmove (FNAME, argv[3], strlen (argv[3]));

    int sockfd;

    sockfd = socket (PF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sockfd <= 0) {
        return -1;
    }

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof (struct sockaddr_in);

    bzero (&addr, sizeof (addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (SERV_PORT);

    int status;

    status = inet_aton (SERV_IPADDR, &addr.sin_addr);
    if (!status) {
        close (sockfd);
        return -1;
    }

    status = sctp_connectx (sockfd, (struct sockaddr *) &addr, 1, 0);
    if (status) {
        close (sockfd);
        return -1;
    }

    FILE *fp = fopen (FNAME, "rb");
    
    if (fp == NULL) {
        close (sockfd);
        return -1;
    }

    struct sctp_event_subscribe events;

    bzero (&events, sizeof (events));
    events.sctp_data_io_event = 1;
    setsockopt (sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof (events));

    struct sctp_sndrcvinfo sinfo;
    socklen_t optlen;

    getsockopt ( sockfd, IPPROTO_SCTP, SCTP_SNDRCV, 
                 &sinfo, &optlen                    );

    printf ("%ld\n", sizeof (sinfo.sinfo_stream));

    int r_bytes, s_bytes;
    char ch;

    while ((r_bytes = fread (&ch, 1, 1, fp))) {
        s_bytes = sctp_sendmsg ( sockfd, &ch, 1, (struct sockaddr *) &addr,
                                 addrlen, sinfo.sinfo_ppid, 
                                 sinfo.sinfo_flags, 0, 0, 0                 );
    }

    close (sockfd);

    return 0;
}
