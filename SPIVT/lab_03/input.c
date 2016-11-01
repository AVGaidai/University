#include <stdio.h>

void fi_0_coding (FILE *fin, FILE *fout)
{
    char c_8;
    char c = 0x0;
    int  i = 0;

    short int c_32;
    short int cur_bit_pos = -1;

    while ( fread (&c_8, 1, sizeof (char), fin) ) {
        c_32 = (short int) c_8;

        if (c != 0x0) {
            if (c_32 - (cur_bit_pos + 1) >= 0) {
                c_32 -= cur_bit_pos + 1;
                fwrite (&c, 1, sizeof (char), fout);
                c = 0x0;
                cur_bit_pos = -2;
            } else {
                c |= 1 << (cur_bit_pos - c_32);
                cur_bit_pos -= c_32 - 1;
                continue;
            }
        }

        for (i = 0; i < c_32 / 8; ++i) {
            fwrite (&c, 1, sizeof (char), fout);
        }

        c |= 1 << (7 - (c_32 - i * 8));
        cur_bit_pos = 7 - (c_32 - i * 8) - 1;
    }
    
    if (cur_bit_pos != -2) {
        fwrite (&c, 1, sizeof (char), fout);
    }
}


void fi_0_decoding (FILE *fin, FILE *fout)
{
    char c_8;
    char c;

    short int res = 0;

    while ( fread (&c_8, 1, sizeof (char), fin) ) {
        for (int i = 7; i >= 0; --i) {
            if ((c_8 >> i) & 1) {
                c = (char) res;
                fwrite (&c, 1, sizeof (char), fout);
                res = 0;
            } else {
                ++res;
            }
        }
    }
}
        


int main (int argc, char *argv[])
{
    if (argc < 4) {
        return 0;
    }

    FILE *fout, *fin;    

    fin  = fopen (argv[1], "rb");
    fout = fopen (argv[2], "wb");

    fi_0_coding (fin, fout);

    fclose (fout);
    fclose (fin);

    fin  = fopen (argv[2], "rb");
    fout = fopen (argv[3], "wb");

    fi_0_decoding (fin, fout);

    fclose (fout);
    fclose (fin);


    return 0;
