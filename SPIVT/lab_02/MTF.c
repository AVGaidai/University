#include <stdio.h>


void MTF_coding (char *alph)
{

}

int main (int argc, char *argv[])
{
    char alph[256];

    for (int i = 0; i < 256; ++i) {
        alph[i] = (char) i;
        printf ("%x ", alph[i]);
    }
    printf ("\n");

    return 0;
}
