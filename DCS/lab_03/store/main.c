#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>

#include <time.h>
#include <sys/time.h>

#include <pthread.h>


int T = 60;
int N = 5;
int K = 2;

int STOP = 0;

int *store;
int *L;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_barrier_t barrier;


void handler (int sig)
{
    STOP = 1;
}


void print_info (int type)
{
    printf ("\E[H\E[J");
    if (type) {
        printf ("costumer = %d\n", type - 1);
        for (int j = 0; j < N; ++j) {
            printf ("\E[32mstore[%d] = %d\E[0m\n", j, store[j]);
        }
    } else {
        printf ("loader\n");
        for (int j = 0; j < N; ++j) {
            printf ("\E[31mstore[%d] = %d\E[0m\n", j, store[j]);
        }
    }
    printf ("\n\n");
    for (int j = 0; j < K; ++j) {
        printf ("\E[34mL[%d] = %d\E[0m\n", j, L[j]);
    }
}


void *costumer_hdr (void *arg)
{
    pthread_barrier_wait (&barrier);

    int id = *(int *) arg;
    while (L[id] && !STOP) {

        pthread_mutex_lock (&mutex);
        for (int i = 0; i < N && !STOP; ++i) {

            print_info (id + 1); 
            if (store[i] >= L[id]) {
                store[i] -= L[id];
                L[id] = 0;
                break;
            } else {
                L[id] -= store[i];
                store[i] = 0;
            }
        }
        print_info (id + 1); 
        sleep (1);
        pthread_mutex_unlock (&mutex);

        if (L[id] && !STOP) sleep (5);
    }

    pthread_exit (NULL);
}



void *loader_hdr (void *arg)
{
    while (!STOP) {

        pthread_mutex_lock (&mutex);

        print_info (0);
        for (int i = 0; i < N && !STOP; ++i) {
            store[i] = 40;
        }
        print_info (0);
        sleep (1);
        pthread_mutex_unlock (&mutex);

        if (!STOP) sleep (5);
    }
    pthread_exit (NULL);
}



int main (int argc, char *argv[])
{
    int opt;

    while ((opt = getopt (argc, argv, "n:k:t:")) != -1) {
        switch (opt) {
        case 'n':
            N = atoi (optarg);
            break;
        case 'k':
            K = atoi (optarg);
            break;
        case 't':
            T = atoi (optarg);
            break;
        default:
            printf ("err opt");
        }
    }

    store = (int *) malloc (sizeof (int) * N);

    srand (time (NULL));
    printf ("\E[H\E[J");
    for (int i = 0; i < N; ++i) {
        store[i] = rand () % 40 + 1;
        printf ("store[%d] = %d\n", i, store[i]);
    }
    sleep (1);

    pthread_barrier_init (&barrier, NULL, K);

    pthread_t *costumers = (pthread_t *) malloc (sizeof (pthread_t) * K);

    L = (int *) malloc (sizeof (int) * K);
    
    int *ARG = (int *) malloc (sizeof (int) * K);

    for (int i = 0; i < K; ++i) {
        L[i] = rand () % 1000 + 1;
        ARG[i] = i;
        pthread_create (&costumers[i], NULL, costumer_hdr, (void *) &ARG[i]);
    }

    pthread_t loader;

    pthread_create (&loader, NULL, loader_hdr, NULL);

    signal (SIGALRM, handler);

    struct itimerval nval, oval;

    nval.it_interval.tv_sec = 0;
    nval.it_interval.tv_usec = 0;

    nval.it_value.tv_sec = T;
    nval.it_value.tv_usec = 0;

    setitimer (ITIMER_REAL, &nval, &oval);

    pthread_join (loader, NULL);
    for (int i = 0; i < K; ++i) {
        pthread_join (costumers[i], NULL);
    }

    free (ARG);
    free (L);
    free (costumers);
    free (store);

    pthread_mutex_destroy (&mutex);
    pthread_barrier_destroy (&barrier);

    return 0;
}
