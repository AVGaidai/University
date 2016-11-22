#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <string.h>

#include "TCP_FUNC.h"
#include "support_func.h"
#include "network.h"


char IP_ADDR[16] = "127.0.0.1";

uint16_t PORT = 8888;


int main (int argc, char *argv[])
{
    if (argc >= 2) {
        memmove (IP_ADDR, argv[1], 16);
        printf ("%s\n", IP_ADDR);
    }

    if (argc == 3) {
        PORT = (uint16_t) atoi (argv[2]);
    }


    int sockfd;

    sockfd = tcp_sock_create (IP_ADDR, PORT);

    if (sockfd == -1) return -1;

    printf ( "====================================\n"
             "sockfd=%d\n"
             "Address: \"%s:%hd\"\n"
             "====================================\n",
                                  sockfd, IP_ADDR, PORT );

    pid_t main_pid = getpid ();

    char ipaddr[16];

    uint16_t port;

    int client_sock;

    tcp_listen (sockfd, 20);

    while (1) {
        client_sock = tcp_accept (sockfd, ipaddr, &port);
        printf ("Client %s:%hd connected!\n", ipaddr, port);
        fork ();
        if (getpid () != main_pid) break;
    }

    char *command;
    char sname[15] = "./dir_create.sh";  // Script name

    char sport[4];                       // Port in char format

    itoa (port, sport);
    printf ("port in char format: %s\n", sport);

    int sname_len = 15;
    int sport_len = 4;
    int ipaddr_len = strlen (ipaddr);

    printf ("sname_len: %d\n", sname_len);
    printf ("ipaddr_len: %d\n", ipaddr_len);
    printf ("sport_len: %d\n", sport_len);

    command = (char *) malloc (sname_len + ipaddr_len + sport_len + 2);

    memcpy (command, sname, sname_len);
    command[sname_len] = ' ';
    memcpy (command + sname_len + 1, ipaddr, ipaddr_len);
    command[sname_len + ipaddr_len + 1] = ' ';
    memcpy (command + sname_len + ipaddr_len + 2, sport, sport_len);

    printf ("command: %s\n", command);

    char *dir;

    dir = (char *) malloc (ipaddr_len + sport_len + 2);
    memcpy (dir, ipaddr, ipaddr_len);
    dir[ipaddr_len] = '/';
    memcpy (dir + ipaddr_len + 1, sport, sport_len);
    dir[ipaddr_len + sport_len + 1] = '/';

    system (command);

    free (command);

    while (!recv_file (client_sock, dir));

    free (dir);

    tcp_sock_remove (client_sock);

    return 0;
}
