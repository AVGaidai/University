#include <stdio.h>
#include <unistd.h>

int main ()
{
    char command[20] = "mkdir 1234\0";
    char fname[5] = "1234\0";

    execl ("/bin/sh", "sh", "-c", command, (char *) 0);

    return 0;
}
