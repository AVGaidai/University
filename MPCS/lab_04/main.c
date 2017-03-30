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

/*
 * Dimension filed SIZE_X * SIZE_Y
 */
#define SIZE_X 100 
#define SIZE_Y 100

/**
 * Data type describing coordinates of point.
 * pos -- serial nubmer;
 * x -- OX coordinate;
 * y -- OY coordinate;
 * Coordinates -- name type.
 */
typedef struct {

    int pos;  // Serial number
    
    int x;    // X coordinate
    int y;    // Y coordinate

} Coordinates;

/**
 * The function swaps 'a' and 'b' values, each 'size' bytes.
 * \param 'a' -- pointer to first element;
 * \param 'b' -- pointer to second element;
 * \param 'size' -- size of each element.
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


/**
 * The function finds the path length between 'nmemb' points.
 * \param 'points' -- pointer to set of points.
 * \param 'nmemb' -- number of the members in set of points.
 */
int calculate_path(Coordinates *points, size_t nmemb)
{
    int length = 0;

    for (int i = 0; i < nmemb - 1; ++i)
	length += sqrt(pow((points[i].x - points[i + 1].x), 2) +
	               pow((points[i].y - points[i + 1].y), 2));
    return length;
}

/**
 * The function finds the shortcut for set of points.
 * \param 'path' -- pointer to container for shortcut.
 * \param 'ipoints' -- pointer to initial set of points;
 * \param 'inmemb' -- number of the members in initial set of points;
 * \param 'points' -- pointer to current set of points;
 * \param 'nmemb' -- number of the members in current set of points.
 */
int
find_path(Coordinates *path,
	  Coordinates *ipoints, size_t inmemb,
	  Coordinates *points, size_t nmemb)
{
    static int min = MAX;   // Length of shortcut
    static int length;      // Length of current path

    if (nmemb > 1) {
	for (int i = 0; i < nmemb; ++i) {
	    swap_v((void *) &points[0], (void *) &points[i],
		   sizeof(Coordinates));
	    find_path(path, ipoints, inmemb, points + 1, nmemb - 1);
	    swap_v((void *) &points[0], (void *) &points[i],
		   sizeof(Coordinates));
	}
    } else {
	length = calculate_path(ipoints, inmemb);
	if (length < min) {
	    min = length;
	    memcpy(path, ipoints, sizeof(Coordinates) * inmemb);
	}
    }

    return length;
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

    Coordinates points[N + 1];
    srand(time(NULL));
    for (int i = 0; i < N; ++i) {
   	points[i].pos = i;
	points[i].x = rand() % (SIZE_X + 1);
	points[i].y = rand() % (SIZE_Y + 1);
	printf("%c(%d;%d) ", points[i].pos + 65, points[i].x, points[i].y);
    }
    printf("\n");

    points[N] = points[0];

    int length;
    Coordinates path[N + 1];
    struct timeval start, end;

    if (gettimeofday(&start, NULL)) {
	fprintf(stderr, "gettimeofday(...) failed\n");
	exit(1);
    }
    length = find_path(path, points, N + 1, points + 1, N -1);
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
    
    printf("Length of minimal path: %d\n{\n", length);
    for (int i = 0; i < N + 1; ++i)
	printf("%c(%d,%d)\n", path[i].pos + 65, path[i].x, path[i].y);
    printf("}\n");
}
