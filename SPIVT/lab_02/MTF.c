#include <stdio.h>
#include <stdlib.h>

#include <string.h>


#include "fi.h"


void swap_val (void *a, void *b, size_t size)
{
    void *tmp;

    tmp = malloc (size);

    memmove (tmp, a, size);
    memmove (a, b, size);
    memmove (b, tmp, size);

    free (tmp);
}


void init_alph (char alph[256])
{
    for (int i = 0; i < 256; ++i) {
        alph[i] = (char) i;
    }
}


int find_into_alph (char alph[256], char val)
{
    for (int i = 0; i < 256; ++i) {
        if (alph[i] == val) {
            return i;
        }
    }

    return -1;
}


void offset_alph (char alph[256], int pos)
{
    for (int i = pos; i > 0; --i) {
        swap_val (&alph[i], &alph[i -1], sizeof (char));
    }
}



int MTF_coding (char *in_file, char *out_file, char alph[256])
{
    FILE *in, *out;

    in  = fopen (in_file, "rb");
    out = fopen ("tmp_cfile.bin", "wb");

    char buf;
    int  pos;
       
    init_alph (alph);
 
    while (fread (&buf, 1, sizeof (char), in)) {
        pos = find_into_alph (alph, buf);
        if (pos == -1) {
            fclose (in);
            fclose (out);
            printf ("error: incorrect alph!\n");
            return -1;
        }
        buf = (char) pos;
        fwrite (&buf, 1, sizeof (char), out);
        offset_alph (alph, pos);
    }

    fclose (in);
    fclose (out);

    fi_0_coding ("tmp_cfile.bin", out_file);

//    remove ("./tmp_file.bin");

    return 0;
}



int MTF_decoding (char *in_file, char *out_file, char alph[256])
{
    fi_0_decoding (in_file, "tmp_dfile.bin");

    FILE *in, *out;

    in  = fopen ("tmp_dfile.bin", "rb");
    out = fopen (out_file, "wb");

    char buf;
    int  pos;

    init_alph (alph);

    while (fread (&buf, 1, sizeof (char), in)) {
        pos = (int) buf;
        fwrite (&alph[pos], 1, sizeof (char), out);
        offset_alph (alph, pos);
    }

    fclose (in);
    fclose (out);

//    remove ("./tmp_file.bin");

    return 0;
}



int main (int argc, char *argv[])
{
    if (argc < 4) {
        return -1;
    }

    char alph[256];
    int st;

    st = MTF_coding (argv[1], argv[2], alph);

    if (st == -1) return st;

    st = MTF_decoding (argv[2], argv[3], alph);

    return 0;
}
