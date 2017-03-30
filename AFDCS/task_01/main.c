/** \file */
/*
 * 1) Failure modeling in distributed computing system
 * default: lam=4*10^(-4) -- failure rate;
 * N=10^4 -- numbers of elementary machine;
 * T=10^4 -- time of distributed computing system operation;
 * dt=10^(-1) -- time interval;
 * r -- random number;
 * q -- probability of failure.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include <math.h>


double lam = 0.0004;
int N = 10000;
double T = 10000.0;
double dt = 0.1;
double r, q;


int main(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "l:N:T:t:")) != -1) {
	switch (opt) {
	case 'l':
	    lam = atof(optarg);
	    break;
        case 'N':
	    N = atoi(optarg);
	    break;
        case 'T':
	    T = atof(optarg);
	    break;
	case 't':
	    dt = atof(optarg);
	    break;
	default:
	    fprintf(stderr, "Incorrect options!\n");
	    exit(EXIT_FAILURE);
	}
    }


    srand(time(NULL));

    FILE *fp;

    fp = fopen("data.txt", "wb");

    for (double i = 0; i < T; i += dt) {
	r = (double) rand() / RAND_MAX;
	q = 1.00 - exp(-1.00 * lam * N * dt);
	if (r < q) --N;
	fprintf(fp, "%.1lf\t%d\n", i, N);
    }

    fclose(fp);

}
