/* 
 * Library containing user functions for working with network
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "shared_data.h"
#include "user_socket.h"
#include "msghandler.h"
#include "network.h"
#include "types.h"
#include "error.h"
#include "list.h"


pthread_t    thr_accept_clients;
pthread_t    thr_recv_servers;
pthread_t    thr_recv_client[MAX_USERS];

pthread_mutex_t   mut = PTHREAD_MUTEX_INITIALIZER;

int stop_accept_clients = 0;
int stop_recv_servers   = 0;


int init_shared_data (void)
{
    sigset_t newset;

    sigemptyset (&newset);
    sigaddset (&newset, SIGPIPE);
    sigprocmask (SIG_BLOCK, &newset, 0);


    extern int   SERVER_TO_CLIENT_SOCKET;

    SERVER_TO_CLIENT_SOCKET = socket (PF_INET, SOCK_STREAM, 0);
    if (SERVER_TO_CLIENT_SOCKET < 0) {
        return print_err ("SERVER_TO_CLIENT_SOCKET = socket ()!");
    }
    if (nblck_sock_mode (SERVER_TO_CLIENT_SOCKET, 1) == -1) {
        return print_err ("nblck_sock_mode ON!");
    }

    extern struct sockaddr_in  SERVER;
    extern struct sockaddr_in  SERVERS[];
 
    int         port;
    char        addr[16];
    FILE       *fp;

    fp = fopen (CONF_FNAME, "r");
    if (fp == NULL) {
        return print_err ("read info about servers!");
    }

    for (int i = 0, j = 0; i < NUM_OF_SERVERS; ++i) {
        fscanf (fp, "%s", addr);
        fscanf (fp, "%d", &port);
        
        if (i == CUR_SERVER) {
            bzero (&SERVER, sizeof (SERVER));
            SERVER.sin_family = AF_INET;
            SERVER.sin_port   = htons (port);
            inet_aton (addr, &SERVER.sin_addr);
            continue;
        }

        bzero (&SERVERS[j], sizeof (SERVERS[j]));
        SERVERS[j].sin_family = AF_INET;
        SERVERS[j].sin_port   = htons (port + 1000);
        inet_aton (addr, &SERVERS[j].sin_addr);
        ++j;
    }

    int status;

    status = bind ( SERVER_TO_CLIENT_SOCKET    ,
                    (struct sockaddr *) &SERVER,
                    sizeof (SERVER)              );
    if (status < 0) {
        return print_err ("SERVER_TO_CLIENT_SOCKET bind ()!");
    }

    port = ntohs (SERVER.sin_port);
    SERVER.sin_port = htons (port + 1000);

    extern int   SERVER_TO_SERVER_SOCKET;

    SERVER_TO_SERVER_SOCKET = socket (PF_INET, SOCK_DGRAM, 0);
    if (SERVER_TO_SERVER_SOCKET < 0) {
        return print_err ("SERVER_TO_SERVER_SOCKET = socket ()!");
    }

    status = bind ( SERVER_TO_SERVER_SOCKET    ,
                    (struct sockaddr *) &SERVER,
                    sizeof (SERVER)              );
    if (status < 0) {
        return print_err ("SERVER_TO_SERVER_SOCKET bind ()!");
    }


    extern struct list  *history;
    extern struct list  *users;

    history = (struct list *) malloc (sizeof (struct list));
    users   = (struct list *) malloc (sizeof (struct list));

    list_init (history);
    list_init (users);
    for (int i = 0; i < MAX_USERS; ++i) {
        state_thr[i] = 0;
    }

    return 0;
}


void *recv_client (void *arg)
{
    struct __recv_msg   info;
    int                 type;
    int                 status;
    char                buf[MAX_MSG_SIZE];

    info = *(struct __recv_msg *) arg;

    while (state_thr[info.ind] == 1) {
        bzero (buf, sizeof (buf));
        status = recv_msg_nblck (info.user.sd, &type, buf, TIMEOUT_RECV);
        if (status == -4) {
            break;
        } else if (status <= 0) {
            continue;
        }

        if (type == DISCONNECT) {
            break;
        }

        handler_client_msg (info.user, type, buf);
    }

    type = REMOVE_USER;
    handler_client_msg (info.user, type, &info.user);

    close (info.user.sd);
    pthread_mutex_lock (&mut);
    state_thr[info.ind] = 0;
    pthread_mutex_unlock (&mut);

    printf ( "\E[32m[ ip: %s ; pid: %d ; sd: %d ]\E[35m "
             "has left the chat\E[0m\n", inet_ntoa (info.user.login.addr),
                                         info.user.login.pid, info.user.sd );

    pthread_exit (NULL);
}


void *recv_servers (void *arg)
{
    struct __serv_msg   msg;
    struct __user       user;

    while (!stop_recv_servers) {
        bzero (&msg, sizeof (msg));
        msg.type = -1;
        recvfrom (SERVER_TO_SERVER_SOCKET, &msg, sizeof (msg), 0, NULL, NULL);
        if (msg.type == -1) {
            continue;
        }

        user.login = msg.login;
        user.sd = -1;
        handler_server_msg (user, msg.type, msg.buf);
    }

    pthread_exit (NULL);
}


void *accept_clients (void *arg)
{
    extern int          state_thr[];

    int                 i;
    int                 client;
    int                 status;
    int                 type;
    char                buf[MAX_MSG_SIZE];
    struct __login      login;
    struct __recv_msg   info;

    if (listen (SERVER_TO_CLIENT_SOCKET, 20) != 0) {
        print_err ("listen ()!");
        pthread_exit (NULL);
    }


    while (!stop_accept_clients) {
        for (i = 0; i < MAX_USERS; ++i) {
            if (state_thr[i] == 0) {
                break;
            }
        }
 
        client = accept_nblck ( SERVER_TO_CLIENT_SOCKET, NULL          , 
                                NULL                   , TIMEOUT_ACCEPT  );
        if (client <= 0) {
            continue;
        }

        status = recv_msg_nblck (client, &type, buf, TIMEOUT_RECV);
        if (status <= 0) {
            continue;
        }

        info.user.sd      = client;
        info.ind          = i;

        memcpy (&login, buf, sizeof (struct __login));
        info.user.login = login;

        if (type == CONNECT) {
            type = ADD_USER;
        } else if (type == RECONNECT) {
/* print */
 printf ("EDIT_USER\n");
            type = EDIT_USER;
        } else {
            continue;
        }

        status = handler_client_msg (info.user, type, &info.user);

/* print */
 printf ("status handler edit user %d\n", status);
        if (status != 0) {
            handler_client_msg (info.user, REMOVE_USER, &info.user);
            continue;
        }
        
        printf ( "\E[32m[ addr: %s ; pid: %d ; sd: %d ]",
                 inet_ntoa (login.addr), login.pid, client );
        if (type == ADD_USER) {
            printf ("\E[35m connected new client\E[0m\n");
        } else if (type == EDIT_USER) {
            printf ("\E[35m reconnected client\E[0m\n");
        }

        pthread_mutex_lock (&mut);
        state_thr[i] = 1;
        pthread_mutex_unlock (&mut);
        pthread_create (&thr_recv_client[i], NULL, recv_client, &info);
    }


    pthread_exit (NULL);
}


int close_users (void *elem, long int size)
{
    struct __user   user;

    memcpy (&user, elem, size);

    if (user.sd > 0) {
        close (user.sd);
    }

    return 0;
}


int start_server (void)
{
    if (init_shared_data ()) {
        print_err ("Can't start server!");
        exit (1);
    } else {
        printf ("**************** START SERVER ****************\n");
        printf (" ip    : %s              \n",    inet_ntoa (SERVER.sin_addr));
        printf (" port 1: %d (for clients)\n", ntohs (SERVER.sin_port) - 1000);
        printf (" port 2: %d (for servers)\n",        ntohs (SERVER.sin_port));
        printf ("**********************************************\n");
    }

    pthread_create (&thr_accept_clients, NULL, accept_clients, NULL);
    pthread_create (&thr_recv_servers, NULL, recv_servers, NULL);

    return 0;
}


int stop_server (void)
{
    close (SERVER_TO_CLIENT_SOCKET);
    close (SERVER_TO_SERVER_SOCKET);

    pthread_mutex_lock (&mut);
    for (int i = 0; i < MAX_USERS; ++i) {
        state_thr[i] = 0;
    }
    pthread_mutex_unlock (&mut);

    sleep (TIMEOUT_RECV);

    pthread_mutex_lock (&mut);
    stop_recv_servers   = 1;
    stop_accept_clients = 1;
    pthread_mutex_unlock (&mut);
    pthread_join (thr_accept_clients, NULL);
//    pthread_join (thr_recv_servers, NULL);

    extern struct list  *history;
    extern struct list  *users;

    list_elem_act (users, sizeof (struct __user), close_users);
    list_free (history);
    free (history);
    list_free (users);
    free (users);

    return 0;
}

