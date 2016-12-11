#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include <time.h>

#include <math.h>



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

    float *a, *b, *c;

    a = (float *) malloc (sizeof (float) * N);
    b = (float *) malloc (sizeof (float) * N);
    c = (float *) malloc (sizeof (float) * N);

    srand (time (NULL));

    matrix_init (a, N);
    matrix_init (b, N);

    
    sqrts (a, b, c, N);

//    matrix_print (a, N);
//    matrix_print (b, N);
//    matrix_print (c, N);

    free (a), free (b), free (c);

    return 0;
}
