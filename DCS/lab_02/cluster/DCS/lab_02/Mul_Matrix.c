#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <strings.h>

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
            tmp[i][j] = rand () % 5;
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


/*
 * A - rows
 * B - rows
 */
void compute_rr (int **A, int **B, int **C, int N)
{
    for (int i = 0; i < N; ++i) {
        bzero (C[i], N * sizeof (int));
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][k] += A[i][j] * B[j][k];
            }
        }
    }
}


/*
 * A - columns
 * B - columns
 */
void compute_cc (int **A, int **B, int **C, int N)
{
    for (int i = 0; i < N; ++i)
        bzero (C[i], N * sizeof (int));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[k][i] += A[k][j] * B[j][i];
            }
        }
    }
}



/*
 * A - rows
 * B - columns
 */
void compute_rc (int **A, int **B, int **C, int N)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/*
 * A - columns
 * B - rows
 */
void compute_cr (int **A, int **B, int **C, int N)
{
    for (int i = 0; i < N; ++i)
        bzero (C[i], N * sizeof (int));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[j][k] += A[j][i] * B[i][k];
            }
        }
    }
}



int XA = 16, YA = 16, XB = 16, YB = 16;

int main (int argc, char *argv[])
{
    int opt, save = 0, compute = 0;
    char *fname = NULL;

    while ((opt = getopt (argc, argv, "sc:n:f:")) != -1) {
        switch (opt) {
        case 'c':
            compute = atoi (optarg);
            break;
        case 's':
            save = 1;
            break;
        case 'n':
            XA = YA = XB = YB = atoi (optarg);
            break;
        case 'f':
            fname = (char *) malloc (strlen (optarg));
            memcpy (fname, optarg, strlen (optarg));
            break;
        default:
            printf ("Incorrect option\n");
            exit (1);
        }
    }

    int **A = NULL, **B = NULL, **C = NULL;

    srand (time (NULL));

    A = matrix_init (XA, YA);
    B = matrix_init (XB, YB);

    if (save && fname != NULL) {
        FILE *fp = fopen (fname, "wb");

        for (int i = 0; i < XA; ++i) {
            for (int j = 0; j < YA; ++j) {
                fprintf (fp, "%d ", A[i][j]);
            }
            fprintf (fp, "\n");
        }
        for (int i = 0; i < XB; ++i) {
            for (int j = 0; j < YB; ++j) {
                fprintf (fp, "%d ", B[i][j]);
            }
            fprintf (fp, "\n");
        }

        fclose (fp);
    } else if (fname != NULL) {
        FILE *fp = fopen (fname, "rb");

        for (int i = 0; i < XA; ++i) {
            for (int j = 0; j < YA; ++j) {
                fscanf (fp, "%d", &A[i][j]);
            }
        }
        for (int i = 0; i < XB; ++i) {
            for (int j = 0; j < YB; ++j) {
                fscanf (fp, "%d", &B[i][j]);
            }
        }

        fclose (fp);
    }

    if (compute) {
        C = matrix_init (XA, YB);

    //    matrix_print (A, XA, YA);
    //    matrix_print (B, XB, YB);
        switch (compute) {
        case 1:
            compute_rr (A, B, C, XA);
            break;
        case 2:
            compute_cc (A, B, C, XA);
            break;
        case 3:
            compute_rc (A, B, C, XA);
            break;
        case 4:
            compute_cr (A, B, C, XA);
            break;
        default:
            compute_rc (A, B, C, XA);
        }

        matrix_print (C, XA, YB);
        matrix_free (C, XA, YB);
    }

    matrix_free (A, XA, YA);
    matrix_free (B, XB, YB);

    if (fname) free (fname);

    return 0;
}
