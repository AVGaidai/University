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

struct msg {

    char buf[100];
    int  len;

};


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

    sockfd = socket (PF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
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

    int r_bytes, s_bytes;
    struct msg msg;
    fpos_t pos;


    printf ("Input message: ");
    int i = 0;

    do {
        scanf ("%c", &msg.buf[i++]);
    } while (i < 99 && msg.buf[i - 1] != '\n');

    msg.len = i;
    msg.buf[i - 1] = '\0';

//    printf ("%s\n", msg.buf);
    
    s_bytes = sctp_sendmsg ( sockfd, &msg, sizeof (msg),
                             (struct sockaddr *) &addr,
                             addrlen, 0, 0, 1, 0, 0     );

    fgetpos (fp, &pos);
    while ((r_bytes = fread (msg.buf, 100, 1, fp))) {
        fgetpos (fp, &pos);
        msg.len = 100;
        s_bytes = sctp_sendmsg ( sockfd, &msg, sizeof (msg),
                                 (struct sockaddr *) &addr,
                                 addrlen, 0, 0, 0, 0, 0     );
//        printf ("s_bytes: %d\n", s_bytes);
    }

    fsetpos (fp, (const fpos_t *) &pos);
    while ((r_bytes = fread (&msg.buf[0], 1, 1, fp))) {
        msg.len = 1;
        s_bytes = sctp_sendmsg ( sockfd, &msg, sizeof (msg),
                                 (struct sockaddr *) &addr,
                                 addrlen, 0, 0, 0, 0, 0     );
//        printf ("s_bytes: %d\n", s_bytes);
    }


    close (sockfd);

    return 0;
}
