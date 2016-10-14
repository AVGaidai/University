#include <stdio.h>

#include "shared_data.h"

int main (int argc, char *argv[])
{
    FILE *fp;
    char  buf[MAX_MSG_SIZE];

    fp = fopen (".buf.txt", "w");
    fclose (fp);

    while (1) {
        printf ("\E[32mInput text\E[34m > \E[0m");
        fgets (buf, MAX_MSG_SIZE, stdin);
        fp = fopen (".buf.txt", "w");
        if (!strncmp (buf, "/exit/", 6)) {
            fclose (fp);
            break;
        }
        fputs (buf, fp);
        fclose (fp);
    }

    return 0;
}
