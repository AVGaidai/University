#include <stdio.h>

#include <string.h>

#include "client_func.h"


int main (int argc, char *argv[])
{
    start_chat ();
    
    char   buf[10];

    do {
        fscanf (stdin, "%s", buf);
    } while (strncmp (buf, "/exit/", 6));

    stop_chat ();

    return 0;
}
