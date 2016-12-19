#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    FILE *fp1, *fp2;

    fp1 = fopen (argv[2], "rb");
    fp2 = fopen (argv[1], "rb");

    int N = atoi (argv[3]);

    int num1, num2, i;

    for (i = 0; i < N; ++i) {
        fscanf (fp1, "%d", &num1);
        fscanf (fp2, "%d", &num2);
        if (num1 != num2) {
            printf ("failure!\n");
            break;
        }
    }

    if (i == N) printf ("successful!\n");

    fclose (fp1);
    fclose (fp2);

    return 0;
}
