#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    pid_t main_pid = getpid ();

    printf ("Main process\n");
    printf ("pid:  %d\n", getpid ());
    printf ("ppid: %d\n", getppid ());
    printf ("uid:  %d\n", getuid ());
    printf ("euid: %d\n", geteuid ());
    printf ("gid:  %d\n", getgid ());
    printf ("egid: %d\n", getegid ());
    printf ("pgrp: %d\n", getpgrp ());
    printf ("sid:  %d\n\n", getsid (getpid ()));

    fork ();
    if (getpid () != main_pid) {
        pid_t child_pid = getpid ();
        printf ("Child process\n");
        printf ("pid:  %d\n", getpid ());
        printf ("ppid: %d\n", getppid ());
        printf ("uid:  %d\n", getuid ());
        printf ("euid: %d\n", geteuid ());
        printf ("gid:  %d\n", getgid ());
        printf ("egid: %d\n", getegid ());
        printf ("pgrp: %d\n", getpgrp ());
        printf ("sid:  %d\n\n", getsid (getpid ()));
        
        fork ();
        if (getpid () != child_pid) {
            printf ("Child-Child process\n");
            printf ("pid:  %d\n", getpid ());
            printf ("ppid: %d\n", getppid ());
            printf ("uid:  %d\n", getuid ());
            printf ("euid: %d\n", geteuid ());
            printf ("gid:  %d\n", getgid ());
            printf ("egid: %d\n", getegid ());
            printf ("pgrp: %d\n", getpgrp ());
            printf ("sid:  %d\n\n", getsid (getpid ()));
        }
    }

    return 0;
}
