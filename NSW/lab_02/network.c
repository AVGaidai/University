#include <stdio.h>

#include "TCP_FUNC.h"
#include "error.h"


#define FNAME     0
#define CONTENT   2


struct msg {
    
    int type;

    int payload;

};


int send_file (int sockfd, const char *fname)
{
    FILE *fp;

    fp = fopen (fname, "rb");
    if (fp <= 0) return print_info ("file does't exist");
 
    int r_bytes, s_bytes;

    struct msg msg;

    msg.type = FNAME;

    int len = strlen (fname);
    int i, j;

    for (i = 0; i < len / 4; ++i) {
        memcpy (&msg.payload, fname + i * 4, sizeof (int));
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
    }
    for (j = 0; fname[i * 4 + j] != '\0'; ++j) {
        msg.payload[j] = fname[i * 4 + j];
    }
    s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));

    msg.type = CONTENT;

    r_bytes = fread (&msg.payload, sizeof (int), 1, fp);

    i = 0;
    while (r_bytes > 0) {
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
        printf ("send bytes: %d\n", s_bytes);

        if (s_bytes <= 0) {
            r_bytes = 1;
            print_info ("transmission failure!");
            if (++i > 5) return -1;
            continue;
        }

        i = 0;
        r_bytes = fread (&msg.payload, sizeof (int), 1, fp);
        sleep (1);
    }

    fclose (fp);

    print_info ("transmission successful!");

    return 0;
}



int recv_file (int sockfd)
{
    struct msg msg;

    int r_bytes;

    char *fname = NULL;
    int cnt_msg_fname = 0;

    while (1) {
        r_bytes = tcp_recv_msg (client_sock, &msg, sizeof (msg));
        sleep (1);

        switch (msg.type) {
        FNAME:
            fname = reallock (fname, 4 * cnt_msg_fname + 1);
            memcpy (fname + cnt_msg_fname * 4, msg.payload, 4);
            ++cnt_msg_fname;
            break;
        CONTENT:
            cnt_msg_fname = 0;
             
            
        

        if (r_bytes == 0) {
            tcp_sock_remove (client_sock);
            return 0;
        }

        BUF[r_bytes] = '\0';

        printf ("recv bytes: %d\n", r_bytes);
        printf ("data: %s\n", BUF);
        printf ("from \"%s:%hd\"\n", ipaddr, port);
    }



    return 0;
}

