#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "TCP_FUNC.h"
#include "error.h"


#define FNAME        0
#define CONTENT      1
#define END_OF_FILE  2

#define SI sizeof (int)


struct msg {
    
    int type;

    int size;
    int payload;

};


int create_pack (void *pack, size_t size, FILE *fp)
{
    int i, status = 1;
    char ch;
    char *tmp_obj;

    tmp_obj = (char *) malloc (size);

    for (i = 0; i < size && status > 0; ++i) {
        status = fread (&ch, sizeof (char), 1, fp);
        tmp_obj[i] = ch;
    }

    if (i == 1) {
        free (tmp_obj);
        return -1;
    } else if (status == 0) {
        --i;
    }

    memcpy (pack, tmp_obj, size);

    free (tmp_obj);

    return i;
}


int send_file (int sockfd, const char *fname)
{
    FILE *fp;

    fp = fopen (fname, "rb");
    if (fp <= 0) return print_info ("file does't exist");
 
    int r_bytes = 0, s_bytes;

    struct msg msg;

    msg.type = FNAME;

    int len = strlen (fname) + 1;
    int i, j;

    for (i = 0; i < len / SI; ++i) {
        memcpy (&msg.payload, fname + i * SI, SI);
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
        printf ("send bytes: %d\n", s_bytes);
    }

    if (len % SI) {
        char rem_fname[SI];   // Remainder file name
 
        for (j = 0; fname[i * SI + j] != '\0'; ++j) {
            rem_fname[j] = fname[i * SI + j];
        }
        rem_fname[j] = '\0';
        memcpy (&msg.payload, rem_fname, SI);
 
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
        printf ("send bytes: %d\n", s_bytes);
    }

    msg.type = CONTENT;

    r_bytes = create_pack (&msg.payload, SI, fp);

    i = 0;
    while (r_bytes > 0) {
        msg.size = r_bytes;
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
        printf ("send bytes: %d from ./%s\n", s_bytes, fname);

        if (s_bytes <= 0) {
            r_bytes = 1;
            print_info ("transmission failure!");
            if (++i > 5) return -1;
            continue;
        }

        i = 0;
        r_bytes = create_pack (&msg.payload, SI, fp);
        sleep (1);
    }

    fclose (fp);

    print_info ("transmission successful!");

    msg.type = END_OF_FILE;
    s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
    printf ("send bytes: %d\n", s_bytes);

    return 0;
}



int recv_file (int sockfd, const char *dir)
{
    struct msg msg;

    int r_bytes;

    char *fname = NULL, *path;
    int cnt_msg_fname = 0;

    while (1) {
        r_bytes = tcp_recv_msg (sockfd, &msg, sizeof (msg));
        if (r_bytes == 0) {
            if (fname) free (fname);
            return 1;
        }

        if (msg.type == FNAME) {
            fname = realloc (fname, SI * cnt_msg_fname + 1);
            memcpy (fname + cnt_msg_fname * SI, &msg.payload, SI);
            ++cnt_msg_fname;
            printf ("recv bytes: %d\n", r_bytes);
            continue;
        }
        break;
    }

    int dir_len = strlen (dir);
    size_t fname_len = strlen (fname);

    path = (char *) malloc (fname_len + dir_len + 1);
    memcpy (path, dir, dir_len);
    memcpy (path + dir_len, fname, fname_len);
    path[fname_len + dir_len] = '\0';
    
    FILE *fp;

    fp = fopen (path, "wb");
    if (fp == NULL) {
        printf ("fp = NULL\n");
    }

    printf ("fname: %s\n", fname);
    printf ("path: ./%s\n", path);

    free (fname);

    printf ("recv bytes: %d in ./%s\n", r_bytes, path);
    fwrite (&msg.payload, SI, 1, fp);
 
    while (1) {
        r_bytes = tcp_recv_msg (sockfd, &msg, sizeof (msg));
        
        if (r_bytes == 0) {
           fclose (fp);
           free (path);
           return 1;
        }

        if (msg.type == END_OF_FILE) {
            printf ("recv bytes: %d\n", r_bytes);
            break;
        }

        printf ("recv bytes: %d in ./%s\n", r_bytes, path);
        fwrite (&msg.payload, msg.size, 1, fp);
    }

    fclose (fp);
    printf ("\E[32mReceived file\E[31m \"./%s\"\E[0m\n", path);

    free (path);


    return 0;
}

