#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/sctp.h>


char     IPADDR[16] = "127.0.0.1\0";
uint16_t PORT;

char *FNAME;


int main (int argc, char *argv[])
{
    int sockfd;

    sockfd = socket (PF_INET, SOCK_STREAM, IPPROTO_SCTP);
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

    status = sctp_bindx (sockfd, (struct sockaddr *) &addr, 1, 0);
    if (status) {
        close (sockfd);
        return -1;
    }

    PORT = ntohs (addt.sin_port);

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hu\"\n"
             "====================================\n",
                                  sockfd, IPADDR, PORT );


    status = listen (sockfd, 20);
    if (status) {
        close (sockfd);
        return -1;
    }

    int sock_client = accept (sockfd, &addr, &addrlen);


    struct sctp_event_subscribe events;

    bzero (&events, sizeof (events));
    events.sctp_data_io_event = 1;
    setsockopt (sockfd, IPPROTO_SCTP, SCTP_EVENTS, &events, sizeof (events));

    struct sctp_sndrcvinfo sinfo_0;

    int r_bytes, msg_flags;
    char ch;

    while ((r_bytes = fread (&ch, 1, 1, fp))) {
        r_bytes = recv_send ( sockfd, &ch, 1, (struct sockaddr *) &addr,
                              &addrlen, &sinfo_0, &msg_flags             );
        printf ("r_bytes: %d\n", r_bytes);
        printf ("%c\n", ch);
    }

    close (sockfd);

    return 0;
}
