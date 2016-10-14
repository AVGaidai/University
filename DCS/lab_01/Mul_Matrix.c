#include <stdio.h>
#include <stdlib.h>

#include <time.h>


int **matrix_init (int x, int y)
{
    int **tmp = NULL;

    tmp = (int **) malloc (sizeof (int *) * x);
    for (int i = 0; i < x; ++i) {
        tmp[i] = (int *) malloc (sizeof (int) * y);
    }


    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            tmp[i][j] = rand () % 20;
        }
    }
    
    return tmp;
}


void matrix_free (int **M, int x, int y)
{
    for (int i = 0; i < x; ++i) {
        free (M[i]);
    }

    free (M);
}


void matrix_print (int **M, int x, int y)
{
    for (int i = 0; i < y * 5; ++i) {
        printf ("=");
    }
    printf ("\n");

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            printf (" %4d", M[i][j]);
        }
        printf ("\n");
    }

    for (int i = 0; i < y * 5; ++i) {
        printf ("=");
    }
    printf ("\n");
}


int main (int argc, char *argv[])
{
    if (argc != 4) return -1;

    int XA, YA, XB, YB;

    XA = atoi (argv[1]);
    YA = XB = atoi (argv[2]);
    YB = atoi (argv[3]);

    int **A = NULL, **B = NULL, **C = NULL;

    srand (time (NULL));

    A = matrix_init (XA, YA);
    B = matrix_init (XB, YB);
    C = matrix_init (XA, YB);

    matrix_print (A, XA, YA);
    matrix_print (B, XB, YB);

    for (int k = 0; k < XA; ++k) {
        for (int l = 0; l < YB; ++l) {
            C[k][l] = 0;
            for (int j = 0; j < YA; ++j) {
                C[k][l] += A[k][j] * B[j][l];
            }
        }
    }

    matrix_print (C, XA, YB);

    matrix_free (A, XA, YA);
    matrix_free (B, XB, YB);
    matrix_free (C, XA, YB);

    return 0;
}
