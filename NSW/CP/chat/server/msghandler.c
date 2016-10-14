/* 
 * Library containing user functions for working with network
 */
#include <stdio.h>
#include <unistd.h>

#include <string.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <pthread.h>

#include "shared_data.h"
#include "user_socket.h"
#include "network.h"
#include "types.h"
#include "error.h"


pthread_mutex_t    mut_users = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t    mut_history = PTHREAD_MUTEX_INITIALIZER;


struct __user   global_user;
struct __chat   global_msg;


int send_history (void *elem, long int size)
{
    send_msg_nblck (global_user.sd, CHAT, elem, size, TIMEOUT_SEND);

    return 0;
}


int users_info (void *elem, long int size)
{
    int             status;
    struct __user   user;

    memcpy (&user, elem, size);
    if (user.sd > 0) {

        status = send_msg_nblck ( user.sd    , INFO_MSG           ,
                                  &global_msg, sizeof (global_msg), 
                                  TIMEOUT_SEND                      );
        if (status <= 0) {
           close (user.sd);
        } 
    }

    return 0;
}



int add_user_c_handler (struct __user user, void *buf)
{
    extern struct list   *users;

    int                  status, val = 0;

    memcpy (&user, buf, sizeof (struct __user));
    pthread_mutex_lock (&mut_users);
    if (list_push_front (users, &user, sizeof (user))) {
        val = -1;
        return print_err ("Don't added user!");
    }

    status = send_msg_nblck ( user.sd, INFO        ,
                              &val   , sizeof (int), TIMEOUT_SEND );
    if (status <= 0 || val == -1) {
        return -1;
    }

    extern struct list   *history;

//    pthread_mutex_lock (&mut_history);
    global_user.sd = user.sd;
    list_elem_act (history, sizeof (struct __chat), send_history);
//    pthread_mutex_unlock (&mut_history);

    bzero (&global_msg, sizeof (global_msg));
    global_msg.login = user.login;
    memcpy (global_msg.buf, "has joined the chat", 19);
    list_elem_act (users, sizeof (struct __user), users_info);
    pthread_mutex_unlock (&mut_users);

    struct __serv_msg   msg;

    msg.type  = ADD_USER;
    msg.login = user.login;
    user.sd   = -1;
    memcpy (msg.buf, &user, sizeof (user));
    sendto_broadcast ( SERVER_TO_SERVER_SOCKET    , &msg     ,
                       sizeof (msg)               , 0        ,
                       (struct sockaddr *) SERVERS,
                       sizeof (SERVER) * (NUM_OF_SERVERS - 1)  );

    return 0;
}


int add_user_s_handler (struct __user user, void *buf)
{
    extern struct list   *users;
    
    pthread_mutex_lock (&mut_users);
    if (list_push_front (users, buf, sizeof (struct __user))) {
        return print_err ("Don't added user!");
    }
    printf ("add user %s\n", inet_ntoa (user.login.addr));
    bzero (&global_msg, sizeof (global_msg));
    global_msg.login = user.login;
    memcpy (global_msg.buf, "has joined the chat", 19);
    list_elem_act (users, sizeof (struct __user), users_info);
    pthread_mutex_unlock (&mut_users);

    return 0;
}



int remove_user_c_handler (struct __user user, void *buf)
{
    extern struct list   *users;

    int              status;

    pthread_mutex_lock (&mut_users);
    status = list_elem_find ( users, &user                 ,
                              buf  , sizeof (struct __user),
                              NULL                           );
    if (status == 0) {
        status = list_elem_remove (users, &user, sizeof (user));
    }
    bzero (&global_msg, sizeof (global_msg));
    global_msg.login = user.login;
    memcpy (global_msg.buf, "has left the chat", 17);
    list_elem_act (users, sizeof (struct __user), users_info);
    pthread_mutex_unlock (&mut_users);

    struct __serv_msg   msg;

    msg.type  = REMOVE_USER;
    msg.login = user.login;
    user.sd   = -1;
    memcpy (msg.buf, &user, sizeof (user));
    sendto_broadcast ( SERVER_TO_SERVER_SOCKET    , &msg     ,
                       sizeof (msg)               , 0        , 
                       (struct sockaddr *) SERVERS,
                       sizeof (SERVER) * (NUM_OF_SERVERS - 1)  );

    return 0;
}



int remove_user_s_handler (struct __user user, void *buf)
{
    extern struct list   *users;
    
    int                  status;

    pthread_mutex_lock (&mut_users);
    status = list_elem_find ( users, &user                 ,
                              buf  , sizeof (struct __user),
                              NULL                           );
    if (status == 0) {
        list_elem_remove (users, &user, sizeof (struct __user));
    }
    bzero (&global_msg, sizeof (global_msg));
    global_msg.login = user.login;
    memcpy (global_msg.buf, "has left the chat", 17);
    list_elem_act (users, sizeof (struct __user), users_info);
    pthread_mutex_unlock (&mut_users);



    return 0;
}


int edit_func (void *elem, long int size)
{
    struct __user   user;

    memcpy (&user, elem, size);
    if (!memcmp (&user.login, &global_user.login, sizeof (struct __login))) {
/* print */
printf ("EDIT! %s\n", inet_ntoa (user.login.addr));
        memcpy (elem, &global_user, size);
        return -1;
    }

    return 0;
}



int edit_user_handler (struct __user user, void *buf)
{
    extern struct list    *users;

    int                   status, val = 0;

    pthread_mutex_lock (&mut_users);
    memcpy (&global_user, buf, sizeof (struct __user));
    status = list_elem_act (users, sizeof (struct __user), edit_func);
//    pthread_mutex_unlock (&mut_users);

    if (status == -1) {
        val = 0;
    } else {
        val = -1;
    }

    status = send_msg_nblck ( user.sd, INFO        ,
                              &val   , sizeof (int), TIMEOUT_SEND );
    if (status <= 0 || val == -1) {
        return -1;
    }
    extern struct list   *history;

//    pthread_mutex_lock (&mut_users);
    global_user.sd = user.sd;
    list_elem_act (history, sizeof (struct __chat), send_history);
    pthread_mutex_unlock (&mut_users);

    return 0;
}


int chat_func (void *elem, long int size)
{
    int             status;
    struct __user   user;

    memcpy (&user, elem, size);
    if (user.sd > 0) {
        status = send_msg_nblck ( user.sd    , CHAT               ,
                                  &global_msg, sizeof (global_msg), 
                                  TIMEOUT_SEND                      );
        if (status <= 0) {
           close (user.sd);
        } 
    }

    return 0;
}




int chat_c_handler (struct __user user, void *buf)
{
    extern struct list   *users;
    extern struct list   *history;

    struct __serv_msg     msg;

    pthread_mutex_lock (&mut_users);
    bzero (&global_msg, sizeof (global_msg));
    memcpy (global_msg.buf, buf, MAX_MSG_SIZE);
    global_msg.login = user.login;
    list_elem_act (users, sizeof (struct __user), chat_func);

//    pthread_mutex_lock (&mut_history);
    list_push_back (history, &global_msg, sizeof (global_msg));
    if (history->size > MAX_HISTORY_SIZE) {
        list_first_elem_remove (history);
    }
//    pthread_mutex_unlock (&mut_history);
    pthread_mutex_unlock (&mut_users);

    msg.type  = CHAT;
    msg.login = user.login;
    memcpy (msg.buf, buf, MAX_MSG_SIZE);
    sendto_broadcast ( SERVER_TO_SERVER_SOCKET    , &msg     ,
                       sizeof (msg)               , 0        ,
                       (struct sockaddr *) SERVERS,
                       sizeof (SERVER) * (NUM_OF_SERVERS - 1)  );

    return 0;
}



int chat_s_handler (struct __user user, void *buf)
{
    extern struct list   *users;
    extern struct list   *history;

    pthread_mutex_lock (&mut_users);
    bzero (&global_msg, sizeof (global_msg));
    memcpy (global_msg.buf, buf, MAX_MSG_SIZE);
    global_msg.login = user.login;
    list_elem_act (users, sizeof (struct __user), chat_func);

//    pthread_mutex_lock (&mut_history);
    list_push_back (history, &global_msg, sizeof (global_msg));
    if (history->size > MAX_HISTORY_SIZE) {
        list_first_elem_remove (history);
    }
//    pthread_mutex_unlock (&mut_history);
    pthread_mutex_unlock (&mut_users);

    return 0;
}




int handler_client_msg (struct __user user, int type, void *buf)
{
    if (type == ADD_USER) {
/* print */
printf ("add_user %s\n", __func__);
        return add_user_c_handler (user, buf);
    } else if (type == REMOVE_USER) {
        return remove_user_c_handler (user, buf);
    } else if (type == EDIT_USER) {
        return edit_user_handler (user, buf);
    } else if (type == CHAT) {
        return chat_c_handler (user, buf);
    }

    return -1;
}


int handler_server_msg (struct __user user, int type, void *buf)
{
    if (type == ADD_USER) {
        return add_user_s_handler (user, buf);
    } else if (type == REMOVE_USER) {
        return remove_user_s_handler (user, buf);
    } if (type == CHAT) {
        return chat_s_handler (user, buf);
    } 

    return -1;
}


