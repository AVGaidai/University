#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include <time.h>

#include <math.h>


uint64_t get_time (void)
{
    uint32_t low, high;

    __asm__ __volatile__ ( "rdtsc\n" : "=a" (low), "=d" (high) );

    return ((uint64_t)high << 32) | low;
}



int sqrts (float *a, float *b, float *c, int size)
{
    for (int i = 0; i < size; ++i)
        c[i] = sqrtf (a[i] * b[i]);

    return 0;
}


int matrix_init (float *m, int n)
{
    for (int i = 0; i < n; ++i)
        m[i] = rand () / (float) RAND_MAX * 1000.0;

    return 0;
}


int matrix_print (float *m, int n)
{
    for (int i = 0; i < n; ++i)
        printf ("%f ", m[i]);

    printf ("\n");

    return 0;
}



int main (int argc, char *argv[])
{
    int N = 1024;

    uint64_t start, end;

    float *a, *b, *c;

    a = (float *) malloc (sizeof (float) * N);
    b = (float *) malloc (sizeof (float) * N);
    c = (float *) malloc (sizeof (float) * N);

    srand (time (NULL));

    matrix_init (a, N);
    matrix_init (b, N);

    
    sqrts (a, b, c, N);
    start = get_time ();
    for (int i = 0; i < 5; ++i)
        sqrts (a, b, c, N);
    end = get_time ();

    printf ("%ld cycles\n", (end - start) / 5);

//    matrix_print (a, N);
//    matrix_print (b, N);
//    matrix_print (c, N);

    free (a), free (b), free (c);

    return 0;
}
