/* 
 * Library containing user functions for working with network
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <pthread.h>

#include "shared_data.h"
#include "user_socket.h"
#include "msghandler.h"
#include "network.h"
#include "types.h"
#include "error.h"


pthread_t    thr_recv_chat_msg;
pthread_t    thr_send_chat_msg;


int          stop_chat_flg = 0;
int          stop          = 0;

int init_shared_data (void)
{
    sigset_t newset;

    sigemptyset (&newset);
    sigaddset (&newset, SIGPIPE);
    sigprocmask (SIG_BLOCK, &newset, 0);


    extern int   CLIENT_SOCKET;

    CLIENT_SOCKET = socket (PF_INET, SOCK_STREAM, 0);
    if (CLIENT_SOCKET < 0) {
        return print_err ("CLIENT_SOCKET = socket ()!");
    }
    if (nblck_sock_mode (CLIENT_SOCKET, 1) == -1) {
        close (CLIENT_SOCKET);
        return print_err ("nblck_sock_mode ON!");
    }

    extern struct sockaddr_in  SERVERS[];
 
    int         port;
    char        addr[16];
    FILE       *fp;

    fp = fopen (CONF_FNAME, "r");
    if (fp == NULL) {
        close (CLIENT_SOCKET);
        return print_err ("read info about servers!");
    }

    for (int i = 0; i < NUM_OF_SERVERS; ++i) {
        fscanf (fp, "%s", addr);
        fscanf (fp, "%d", &port);

        bzero (&SERVERS[i], sizeof (SERVERS[i]));
        SERVERS[i].sin_family = AF_INET;
        SERVERS[i].sin_port   = htons (port);
        inet_aton (addr, &SERVERS[i].sin_addr);
    }
    
    extern int  PID;
    extern int  CUR_SERVER;

    PID = getpid ();

    CUR_SERVER = PID % NUM_OF_SERVERS;

    return 0;
}




int connect_to_server (int mode)
{
    extern int     CLIENT_SOCKET;
    extern int     CUR_SERVER;

    int            status;    

    if (mode == DISCONNECT) {
        status = send_msg_nblck ( CLIENT_SOCKET, DISCONNECT,
                                  NULL         , 0         , TIMEOUT_SEND );
        if (status <= 0) {
            return print_err ("send_msg_nblck ()!");
        }
        return 0;
    } else if (mode == RECONNECT) {
        close (CLIENT_SOCKET);
        CLIENT_SOCKET = socket (PF_INET, SOCK_STREAM, 0);
        if (CLIENT_SOCKET < 0) {
            return print_err ("CLIENT_SOCKET = socket ()!");
        }
        if (nblck_sock_mode (CLIENT_SOCKET, 1) == -1) {
            close (CLIENT_SOCKET);
            return print_err ("nblck_sock_mode ON!");
        }
        CUR_SERVER = (CUR_SERVER + 1) % NUM_OF_SERVERS;
    } else if (mode != CONNECT) {
        return print_err ("incorrect mode connect_to_server ()!");
    }

    socklen_t   addr_len;

    for (int i = CUR_SERVER; i < NUM_OF_SERVERS * TRY_CONNECT; ++i) {
        CUR_SERVER = i % NUM_OF_SERVERS;
        addr_len   = sizeof (SERVERS[CUR_SERVER]);
        status = connect_nblck ( CLIENT_SOCKET                           , 
                                 (struct sockaddr *) &SERVERS[CUR_SERVER],
                                 addr_len                                ,
                                 TIMEOUT_CONNECT                           );
        if (status == 0) {
            break;
        }
        close (CLIENT_SOCKET);
        CLIENT_SOCKET = socket (PF_INET, SOCK_STREAM, 0);
        if (CLIENT_SOCKET < 0) {
            return print_err ("CLIENT_SOCKET = socket ()!");
        }
        if (nblck_sock_mode (CLIENT_SOCKET, 1) == -1) {
            close (CLIENT_SOCKET);
            return print_err ("nblck_sock_mode ON!");
        }
    }
    if (status != 0) {
        return print_info ("All servers disabled!");
    }
    
    struct __login    login;
    int               type;
    char              buf[MAX_MSG_SIZE];

    login.pid = PID;
    inet_aton (ADDR, &login.addr);

    if (mode == RECONNECT) {
        status = send_msg_nblck ( CLIENT_SOCKET, RECONNECT     , 
                                  &login       , sizeof (login),
                                  TIMEOUT_SEND                   );
    } else {
        status = send_msg_nblck ( CLIENT_SOCKET, CONNECT       , 
                                  &login       , sizeof (login), 
                                  TIMEOUT_SEND                   );
    }

    if (status <= 0) {
        return print_err ("send_msg_nblck ()!");
    }
    
    status = recv_msg_nblck (CLIENT_SOCKET, &type, buf, TIMEOUT_RECV);
    if (status <= 0) {
        return print_err ("recv_msg_nblck ()!");
    }

    return handler_msg (type, buf);
}


void *recv_chat_msg (void *arg)
{
    char    buf[MAX_MSG_SIZE + 10];
    int     type;
    int     status;

    while (!stop_chat_flg) {
        status = recv_msg_nblck (CLIENT_SOCKET, &type, buf, TIMEOUT_RECV);
        if (status == -4 && !stop) {   
            stop = 1;
            if (connect_to_server (RECONNECT)) {
                stop = 0;
                break;
            }
            stop = 0;
        }
            
        if (status > 0) {
            handler_msg (type, buf);
        }
    }

    pthread_exit (NULL);
}


void *send_chat_msg (void *arg)
{
    FILE  *fp;
    char   buf[MAX_MSG_SIZE];
    int    status;


    while (!stop_chat_flg) {
        bzero (buf, sizeof (buf));
        fp = fopen (".buf.txt", "r");
        if (fp == NULL) {
            continue;
        }
        while (stop) sleep (1);
        fgets (buf, MAX_MSG_SIZE, fp);
        fclose (fp);
        if (strlen (buf) > 1) {
            fp = fopen (".buf.txt", "w");
            fclose (fp);
            status = send_msg_nblck ( CLIENT_SOCKET, CHAT        , 
                                      buf          , sizeof (buf),
                                      TIMEOUT_SEND                 );

            if (status <= 0) {
/*
                status = connect_to_server (RECONNECT);
                if (status == 0) {
                    status = send_msg_nblck ( CLIENT_SOCKET, CHAT        , 
                                              buf          , sizeof (buf), 
                                              TIMEOUT_SEND                 );
                } else {
                    close (CLIENT_SOCKET);
                    exit (1);
                }
*/
                continue;
            }

        }
    }

    pthread_exit (NULL);
}


void start_chat (void)
{
    int status;

    init_shared_data ();
    status = connect_to_server (CONNECT);
    if (status != 0) {
        close (CLIENT_SOCKET);
        exit (1);
    }

    system ("/bin/gnome-terminal --command ./send_msg.out");
    pthread_create (&thr_recv_chat_msg, NULL, recv_chat_msg, NULL);
    pthread_create (&thr_send_chat_msg, NULL, send_chat_msg, NULL);   
}


void stop_chat (void)
{
    stop = 1;
    connect_to_server (DISCONNECT);
    stop_chat_flg = 1;
    pthread_join (thr_recv_chat_msg, NULL);
    pthread_join (thr_send_chat_msg, NULL);
    close (CLIENT_SOCKET);
}
