#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#include "support_func.h"


#define BUF_SIZE 8


struct PAIR {

    size_t  pos;
    char    val;

    short   flg;
};


int find_cmp (void *a, void *b)
{
    struct PAIR A = *(struct PAIR *) a;
    struct PAIR B = *(struct PAIR *) b;

    int res;
    
    res = memcmp (&A.val, &B.val, sizeof (char));
    
    if (res == 0 && A.flg == 1) {
        return 1;
    } else if (res == 0) {
        A.flg = 1;
        memcpy (a, &A, sizeof (struct PAIR));
    }

    return res;
}


int compare_1 (const void *a, const void *b, void *arg)
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


int compare_2 (const void *a, const void *b)
{
    struct PAIR A, B;

    A = *(struct PAIR *) a; 
    B = *(struct PAIR *) b; 

    int res;
    
    res = memcmp (&A.val, &B.val, sizeof (char));

    if (res == 0) {
        res = memcmp (&A.pos, &B.pos, sizeof (size_t));
    }

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

        qsort_r (buf_2D, BUF_SIZE, BUF_SIZE, compare_1, &size);

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

    qsort_r (rem_2D, r_bytes, r_bytes, compare_1, &r_bytes);

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



void BWT_decoding (char *in_file, char *out_file)
{
    char buf[BUF_SIZE + 1];

    char res[BUF_SIZE];

    FILE *fin, *fout;

    fin  = fopen (in_file, "rb");
    fout = fopen (out_file, "wb");
 
    fpos_t cur_fp;

    fgetpos (fin, &cur_fp);
    while (fread (buf, BUF_SIZE + 1, 1, fin)) {
        fgetpos (fin, &cur_fp);

        struct PAIR alph_1[BUF_SIZE];
        struct PAIR alph_2[BUF_SIZE];

        for (int i = 0; i < BUF_SIZE; ++i) {
            alph_1[i].pos = i;
            alph_1[i].val = buf[i];

            alph_1[i].flg = 0;
        }

        memcpy (alph_2, alph_1, sizeof (struct PAIR) * BUF_SIZE);

        qsort (alph_1, BUF_SIZE, sizeof (struct PAIR), compare_2);

        for (int i = 0; i < BUF_SIZE; ++i) {
            alph_2[i].pos = find_into_alph ( &alph_2[i], alph_1,
                                             sizeof (struct PAIR),
                                             BUF_SIZE, find_cmp   );
        }

        size_t start = (size_t) buf[BUF_SIZE];
        size_t pos = alph_2[start].pos;

        res[0] = alph_2[start].val;

        for (int i = 1; i < BUF_SIZE; ++i) {
            res[i] = alph_2[pos].val;
            pos = alph_2[pos].pos;
        }

        fwrite (res, BUF_SIZE, 1, fout);
    }

    fsetpos (fin, (const fpos_t *) &cur_fp);

    int r_bytes = -1;

    for (int i = 0; ; ++i) {
        if (fread (&buf[i], sizeof (char), 1, fin)) {
            ++r_bytes;
        } else {
            break;
        }
    }

    if (r_bytes == -1) {
        fclose (fin);
        fclose (fout);
        return;
    }

    char rem[r_bytes + 1];
    char res1[r_bytes];

    memmove (rem, buf, r_bytes + 1);    

    struct PAIR alph_1[r_bytes];
    struct PAIR alph_2[r_bytes];

    for (int i = 0; i < r_bytes; ++i) {
        alph_1[i].pos = i;
        alph_1[i].val = rem[i];

        alph_1[i].flg = 0;
    }

    memcpy (alph_2, alph_1, sizeof (struct PAIR) * r_bytes);
    qsort (alph_1, r_bytes, sizeof (struct PAIR), compare_2);

    for (int i = 0; i < r_bytes; ++i) {
        alph_2[i].pos = find_into_alph ( &alph_2[i], alph_1,
                                         sizeof (struct PAIR),
                                         r_bytes, find_cmp     );
    }

    size_t start = (size_t) rem[r_bytes];
    size_t pos = alph_2[start].pos;

    res1[0] = alph_2[start].val;

    for (int i = 1; i < BUF_SIZE; ++i) {
        res1[i] = alph_2[pos].val;
        pos = alph_2[pos].pos;
    }

    fwrite (res1, r_bytes, 1, fout);

    fclose (fin);
    fclose (fout);
   
}




int main (int argc,char *argv[])
{
    if (argc < 4) {
        return -1;
    }

    BWT_coding (argv[1], argv[2]);
    BWT_decoding (argv[2], argv[3]);

    return 0;
}
