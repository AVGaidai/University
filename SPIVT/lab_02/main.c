#include <stdio.h>

#include "MTF.h"


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
