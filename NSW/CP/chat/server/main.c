#include <stdio.h>

#include <string.h>

#include "server_func.h"


int main (int argc, char *argv[])
{
    start_server ();

    char   buf[10];

    do {
        fscanf (stdin, "%s", buf);
    } while (strncmp (buf, "/exit/", 6));

    stop_server ();

    return 0;
}
