#include <stdio.h>
#include <unistd.h>

#include <string.h>

#include "TCP_FUNC.h"
#include "error.h"


#define FNAME     0
#define CONTENT   1

#define SI sizeof (int)


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

    for (i = 0; i < len / SI; ++i) {
        memcpy (&msg.payload, fname + i * SI, SI);
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
    }
    for (j = 0; fname[i * SI + j] != '\0'; ++j) {
        msg.payload[j] = fname[i * SI + j];
    }
    s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));

    msg.type = CONTENT;

    r_bytes = fread (&msg.payload, SI, 1, fp);

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
        r_bytes = fread (&msg.payload, SI, 1, fp);
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

        if (msg.type == FNAME) {
            fname = reallock (fname, SI * cnt_msg_fname + 1);
            memcpy (fname + cnt_msg_fname * SI, msg.payload, SI);
            ++cnt_msg_fname;
            continue;
        }
        break;
    }

    FILE *fp;

    fp = fopen (fname, "wb");

    while (1) {
        r_bytes = tcp_recv_msg (client_sock, &msg, sizeof (msg));
        
        if (r_bytes == 0) {
            tcp_sock_remove (client_sock);
            return 1;
        }

        printf ("recv bytes: %d\n", r_bytes);
        printf ("data: %s\n", BUF);
        printf ("from \"%s:%hd\"\n", ipaddr, port);
        
        fwrite (&msg.payload, SI, 1, fp);
    }

    fclose (fp);

    prinrf ("Received file \"%s\"\n", fname);
    free (fname);

    return 0;
}

