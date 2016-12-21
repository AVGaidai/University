#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <inttypes.h>
#include <omp.h>
#include <sys/time.h>

/*
 * Memory consumption: O(m * n + n + m)
 */

double *cs, *cp;

enum {
    m = 20000,
    n = 20000
};

void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

/* dgemv: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void dgemv(double *a, double *b, double *c, int m, int n)
{
    for (int i = 0; i < m; i++)
        c[i] = 0.0;
        //cs[i] = 0.0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
            //cs[i] += a[i * n + j] * b[j];
    }
}

/* dgemv_omp: Compute matrix-vector product c[m] = a[m][n] * b[n] */
void dgemv_omp(double *a, double *b, double *c, int m, int n)
{
    //bzero(c, m * sizeof (double));
    for (int i = 0; i < m; i++)
        c[i] = 0.0;
        //cp[i] = 0.0;

    // Parallelize this code
#pragma omp parallel 
{
    #pragma omp for collapse (2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
            //cp[i] += a[i * n + j] * b[j];
    }
}
}

double run_serial()
{
    double *a, *b, *c;

    // Allocate memory for 2-d array a[m, n]
    a = xmalloc(sizeof(*a) * m * n);
    b = xmalloc(sizeof(*b) * n);
    c = xmalloc(sizeof(*c) * m);
    //cs = xmalloc(sizeof(*cs) * m);

    #pragma omp parallel for collapse (2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    #pragma omp parallel for
    for (int j = 0; j < n; j++)
        b[j] = j;

    double t = wtime();
    dgemv(a, b, c, m, n);
    t = wtime() - t;

    printf("Elapsed time (serial): %.6f sec.\n", t);
    free(a);
    free(b);
    free(c);
    return t;
}

double run_parallel()
{
    double *a, *b, *c;

    // Allocate memory for 2-d array a[m, n]
    a = xmalloc(sizeof(*a) * m * n);
    b = xmalloc(sizeof(*b) * n);
    c = xmalloc(sizeof(*c) * m);
    //cp = xmalloc(sizeof(*cp) * m);

    #pragma omp parallel for collapse (2)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + j;
    }
    #pragma omp parallel for
    for (int j = 0; j < n; j++)
        b[j] = j;

    double t = wtime();
    dgemv_omp(a, b, c, m, n);
    t = wtime() - t;

    printf("Elapsed time (parallel): %.6f sec.\n", t);
    free(a);
    free(b);
    free(c);
    return t;
}

void verefication(void)
{
    double def = 0.0;

    for (int i = 0; i < m; ++i) {
        def = cp[i] - cs[i];
        def = def > 0.0 ? def : 0.0 - def;
        if (def > 0.0001) {
            printf("failure verefication!\n");
            break;
        }
    }
    free(cp), free(cs);
}

int main(int argc, char **argv)
{
    printf("DGEMV: general matrix-vector multiplication (c[m] = a[m, n] * b[n]; m = %d, n = %d)\n", m, n);
    printf("Memory used: %" PRIu64 " MiB\n", (uint64_t)(((double)m * n + m + n) * sizeof(double)) >> 20);
    double tser = run_serial();
    double tpar = run_parallel();
    //verefication();
    printf("Speedup: %.2f\n", tser / tpar);
    
    return 0;
}
