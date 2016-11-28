#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <unistd.h>

#include <string.h>

#include <errno.h>

#include "TCP_FUNC.h"
#include "support_func.h"
#include "network.h"


char     IPADDR[16] = "127.0.0.1\0";

uint16_t PORT;

void handle_sigchld(int sig) {
    int saved_errno = errno;

    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
    errno = saved_errno;
}


int main (int argc, char *argv[])
{
    int sockfd;

    sockfd = tcp_sock_create (IPADDR, &PORT);

    if (sockfd == -1) return -1;

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hu\"\n"
             "====================================\n",
                                  sockfd, IPADDR, PORT );

    pid_t main_pid = getpid ();
    
    char ipaddr[16];

    uint16_t port;

    int client_sock;

    tcp_listen (sockfd, 20);

    signal (SIGCHLD, handle_sigchld);

    while (1) {
        client_sock = tcp_accept (sockfd, ipaddr, &port);
        printf ("Client %s:%hu connected!\n", ipaddr, port);
        fork ();
        if (getpid () != main_pid) break;
    }

    char *command;
    char sname[15] = "./dir_create.sh";  // Script name

    char sport[10];                      // Port in char format

    itoa (port, sport);

    int sname_len = 15;
    int sport_len = strlen (sport);
    int ipaddr_len = strlen (ipaddr);

    command = (char *) malloc (sname_len + ipaddr_len + sport_len + 2);

    memcpy (command, sname, sname_len);
    command[sname_len] = ' ';
    memcpy (command + sname_len + 1, ipaddr, ipaddr_len);
    command[sname_len + ipaddr_len + 1] = ' ';
    memcpy (command + sname_len + ipaddr_len + 2, sport, sport_len);

    char *dir;

    dir = (char *) malloc (ipaddr_len + sport_len + 3);
    memcpy (dir, ipaddr, ipaddr_len);
    dir[ipaddr_len] = '/';
    memcpy (dir + ipaddr_len + 1, sport, sport_len);
    dir[ipaddr_len + sport_len + 1] = '/';
    dir[ipaddr_len + sport_len + 2] = '\0';

    system (command);

    free (command);

    while (!recv_file (client_sock, dir));

    free (dir);

    tcp_sock_remove (client_sock);

    return 0;
}
