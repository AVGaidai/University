#include <stdio.h>
#include <unistd.h>

#include <signal.h>

#include <sys/time.h>

unsigned int flg = 0;


void func_1 (void)
{
    while (!flg) printf ("A");
}


void func_2 (void)
{
    while (flg) printf ("B");
}


void handler (int sig)
{
    flg = ~flg;
}



int main (void) {
    struct itimerval nval, oval;

    signal (SIGALRM, handler);

    nval.it_interval.tv_sec = 3;
    nval.it_interval.tv_usec = 0;

    nval.it_value.tv_sec = 3;
    nval.it_value.tv_usec = 0;

    setitimer (ITIMER_REAL, &nval, &oval);

    while (1) func_1 (), func_2 ();

    return (0);
}
