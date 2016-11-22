#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "TCP_FUNC.h"
#include "error.h"


#define FNAME     0
#define CONTENT   1

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

    printf ("tmp_obj: %s\n", tmp_obj);

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

    int len = strlen (fname);
    int i, j;

    for (i = 0; i < len / SI; ++i) {
        memcpy (&msg.payload, fname + i * SI, SI);
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
    }

    char rem_fname[SI];   // Remainder file name

    for (j = 0; fname[i * SI + j] != '\0'; ++j) {
        rem_fname[j] = fname[i * SI + j];
    }
    memcpy (&msg.payload, rem_fname, SI);

    s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));

    msg.type = CONTENT;

    r_bytes = create_pack (&msg.payload, SI, fp);

    printf ("recv bytes: %d\n", r_bytes);
    i = 0;
    while (r_bytes > 0) {
        char tmp[4];
        msg.size = r_bytes;
        memcpy (tmp, &msg.payload, SI);
        printf ("data: %s\n", tmp);
        s_bytes = tcp_send_msg (sockfd, &msg, sizeof (msg));
        printf ("send bytes: %d\n", s_bytes);

        if (s_bytes <= 0) {
            r_bytes = 1;
            print_info ("transmission failure!");
            if (++i > 5) return -1;
            continue;
        }

        i = 0;
        r_bytes = create_pack (&msg.payload, SI, fp);
        printf ("recv bytes: %d\n", r_bytes);
//        sleep (1);
    }

    fclose (fp);

    print_info ("transmission successful!");

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
//        sleep (1);

        printf ("recv bytes: %d\n", r_bytes);

        if (msg.type == FNAME) {
            fname = realloc (fname, SI * cnt_msg_fname + 1);
            memcpy (fname + cnt_msg_fname * SI, &msg.payload, SI);
            ++cnt_msg_fname;
            continue;
        }
        break;
    }

    int dir_len = strlen (dir);
    printf ("dir_len: %d\n", dir_len);
    size_t fname_len = strlen (fname);
    printf ("dir_len: %d\n", dir_len);

    path = (char *) malloc (fname_len + dir_len);
    memcpy (path, dir, dir_len);
    memcpy (path + dir_len, fname, fname_len); 
    
    FILE *fp;

    fp = fopen (path, "wb");
    if (fp == NULL) {
        printf ("fp = NULL\n");
    }

    printf ("fname: %s\n", fname);
    printf ("path: %s\n", path);

    free (fname);
    free (path);

    printf ("recv bytes: %d\n", r_bytes);
    fwrite (&msg.payload, SI, 1, fp);
 
    while (1) {
        r_bytes = tcp_recv_msg (sockfd, &msg, sizeof (msg));
        
        if (r_bytes == 0) {
            return 1;
        }

        printf ("recv bytes: %d\n", r_bytes);
char tmp[SI];
memcpy (tmp, &msg.payload, SI);
        printf ("data: %s\n", tmp);
        fwrite (&msg.payload, msg.size, 1, fp);
    }

    fclose (fp);

    printf ("Received file \"%s\"\n", fname);

    return 0;
}

