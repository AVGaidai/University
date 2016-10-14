/* 
 * Library containing user functions for working with network
 */
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "shared_data.h"
#include "types.h"
#include "error.h"


int info_msg_handler (char *buf)
{
    struct __chat   chat;

    memcpy (&chat, buf, sizeof (struct __chat));

    printf ( "\n\E[35m-----------------------------\E[0m");
    printf ( "\E[35m-----------------------------\E[0m\n");
    printf ( "\E[32m[ ip: %s ; pid: %d ] "
             "\E[35m%s\E[0m\n", 
             inet_ntoa (chat.login.addr), chat.login.pid, chat.buf );
    printf ( "\n\E[35m-----------------------------\E[0m");
    printf ( "\E[35m-----------------------------\E[0m\n");

    return 0;
}


int chat_handler (char *buf)
{
    struct __chat   chat;

    memcpy (&chat, buf, sizeof (struct __chat));

    printf ( "[ \E[32mip: %s\E[0m ;"
             " \E[33mpid: %d \E[0m ]"
             " \E[34m-->\E[0m %s", 
             inet_ntoa (chat.login.addr), chat.login.pid, chat.buf );

    return 0;
}


int info_handler (void *buf)
{
    printf ("\n\E[H\E[2J\n");
    printf ("\n\E[H\E[2J\n");

    return *(int *) buf;
}


int handler_msg (int type, void *buf)
{
    if (type == CHAT) {
        return chat_handler (buf);
    } else if (type == INFO) {
        return info_handler (buf);
    } else if (type == INFO_MSG) {
        return info_msg_handler (buf);
    }

    return -1;
}
