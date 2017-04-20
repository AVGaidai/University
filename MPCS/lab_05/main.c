/** \file */
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>

#include <time.h>
#include <sys/time.h>

/*
 * Max value for integer type
 */
#define MAX ~(1 << (sizeof(int) * 8 -1))


/**
 * Data type describing detail.
 *
 */
typedef struct {

    int num;   /**< Number of detail */
    
    int t1;    /**< Work time for first machine. */
    int t2;    /**< Work time for second machine. */

} Detail;



/**
 * \brief The function swaps 'a' and 'b' values, each 'size' bytes.
 *
 * \param a is pointer to first element.
 * \param b is pointer to second element.
 * \param size is size of each element.
 */
void swap_v(void *a, void *b, size_t size)
{
    void *tmp;

    tmp = malloc(size);

    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);

    free(tmp);
}


int find_time(Detail *plan, int nmemb)
{
    int time = 0;
    int t1 = 0;
    
    for (int i = 0; i < nmemb; ++i) {
	time += plan[i].t1 + plan[i].t2;
	t1 += plan[i].t1;
    }
    t1 -= plan[0].t1;

    for (int i = 0; i < nmemb - 1; ++i) {
	if (t1 <= plan[i].t2) {
	    time -= plan[i].t2;
	    t1 -= plan[i].t2;
	} else {
	    time -= t1;
	    break;
	}
    }

    return time;
}
    
    

int
find_plan(Detail *iplan, Detail *fplan, int inmemb, Detail *plan, int nmemb)
{
    static int min = MAX;
    static int time = 0;

    if (nmemb > 1) {
	for (int i = 0; i < nmemb; ++i) {
	    swap_v((void *) &plan[0], (void *) &plan[i], sizeof(Detail));
	    find_plan(iplan, fplan, inmemb, plan + 1, nmemb - 1);
	    swap_v((void *) &plan[0], (void *) &plan[i], sizeof(Detail));
	}
    } else {   
	time = find_time(iplan, inmemb);
	if (time < min) {
	    min = time;
	    memcpy(fplan, iplan, sizeof(Detail) * inmemb);
	}
    }

    return time;
}


    
/**
 * The main function.
 * \param 'first' -- number of points;
 * \param 'second' -- filename for print shortcut time.
 */    
int main(int argc, char *argv[])
{
    int N = 4;
    
    if (argc > 1) N = atoi(argv[1]);

    Detail details[N];

    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
   	details[i].num = i;
	details[i].t1 = rand() % 6 + 5;
	details[i].t2 = rand() % 6 + 5;
	printf("D%d(t1=%d; t2=%d) ", details[i].num, details[i].t1, details[i].t2);
    }
    printf("\n");


    Detail plan[N];
    int time;
    struct timeval start, end;

    if (gettimeofday(&start, NULL)) {
	fprintf(stderr, "gettimeofday(...) failed\n");
	exit(1);
    }
    time = find_plan(details, plan, N, details, N);
    if (gettimeofday(&end, NULL)) {
	fprintf(stderr, "gettimeofday(...) failed\n");
	exit(1);
    }
    
    if (argc == 3) {
	FILE *fp;
	
	fp = fopen(argv[2], "ab");
	struct timeval res;

	res.tv_sec = end.tv_sec - start.tv_sec;
	res.tv_usec = end.tv_usec - start.tv_usec;
	if (res.tv_usec < 0) {
	    --res.tv_sec;
	    res.tv_usec = 1000000 - res.tv_usec;
	}
	
	fprintf(fp, "%d %ld.%ld\n", N, res.tv_sec, res.tv_usec);
	fclose(fp);
    }
    
    printf("Optimal plan time: %d\n{\n", time);
    for (int i = 0; i < N; ++i)
	printf("D%d(t1=%d,t2=%d)\n", plan[i].num, plan[i].t1, plan[i].t2);
    printf("}\n");
}
