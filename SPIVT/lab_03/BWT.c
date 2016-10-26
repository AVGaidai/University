#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#include "support_func.h"


#define BUF_SIZE 8


int compare (const void *a, const void *b, void *arg)
{
    size_t size = *(size_t *) arg;

    char *A, *B;

    A = (char *) malloc (size); 
    B = (char *) malloc (size); 

    memcpy (A, a, size);
    reverse_alph (A, sizeof (char), size);
    A[0] = 0x0;

    memcpy (B, b, size);
    reverse_alph (B, sizeof (char), size);
    B[0] = 0x0;
   
    int res = memcmp (A, B, size);

    free (A);
    free (B);

    return res;
}


void BWT_coding (char *in_file, char *out_file)
{
    char buf_1D[BUF_SIZE];
    char buf_2D[BUF_SIZE][BUF_SIZE];

    char res[BUF_SIZE + 1];

    FILE *fin, *fout;

    fin  = fopen (in_file, "rb");
    fout = fopen (out_file, "wb");

    fpos_t cur_fp;

    fgetpos (fin, &cur_fp);
    while (fread (buf_1D, BUF_SIZE, 1, fin)) {
        fgetpos (fin, &cur_fp);
        res[BUF_SIZE] = BUF_SIZE;
        memcpy (buf_2D[0], buf_1D, BUF_SIZE);
        for (int i = 1; i < BUF_SIZE; ++i) {
            memcpy (buf_2D[i], buf_2D[i - 1], BUF_SIZE);
            cyc_offset_alph (buf_2D[i], sizeof (char), BUF_SIZE);
        }

        size_t size = BUF_SIZE;

        qsort_r (buf_2D, BUF_SIZE, BUF_SIZE, compare, &size);

        for (int i = 0; i < BUF_SIZE; ++i) {
            res[i] = buf_2D[i][BUF_SIZE - 1];
            if (res[BUF_SIZE] == BUF_SIZE) {
                res[BUF_SIZE] = (res[i] == buf_1D[0]) ? (char) i : BUF_SIZE;
            }
        }

        fwrite (res, BUF_SIZE + 1, 1, fout);
    }

    fsetpos (fin, (const fpos_t *) &cur_fp);

    int r_bytes = 0;

    for (int i = 0; ; ++i) {
        if (fread (&buf_1D[i], sizeof (char), 1, fin)) {
            ++r_bytes;
        } else {
            break;
        }
    }

    if (r_bytes == 0) {
        fclose (fin);
        fclose (fout);
        return;
    }

    char rem_1D[r_bytes];
    char rem_2D[r_bytes][r_bytes];
    char res1[r_bytes + 1];

    memmove (rem_1D, buf_1D, r_bytes);    
    res1[r_bytes] = r_bytes;

    memcpy (rem_2D[0], rem_1D, r_bytes);
    for (int i = 1; i < r_bytes; ++i) {
        memcpy (rem_2D[i], rem_2D[i - 1], r_bytes);
        cyc_offset_alph (rem_2D[i], sizeof (char), r_bytes);
    }

    qsort_r (rem_2D, r_bytes, r_bytes, compare, &r_bytes);

    for (int i = 0; i < r_bytes; ++i) {
        res1[i] = rem_2D[i][r_bytes - 1];
        if (res1[r_bytes] == r_bytes) {
            res1[r_bytes] = (res1[i] == rem_1D[0]) ? (char) i : r_bytes;
        }
    }

    fwrite (res1, r_bytes + 1, 1, fout);


    fclose (fin);
    fclose (fout);
}


int main (int argc,char *argv[])
{
    if (argc < 4) {
        return -1;
    }

    BWT_coding (argv[1], argv[2]);

    return 0;
}
