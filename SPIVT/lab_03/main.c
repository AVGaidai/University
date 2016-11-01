/*
 * Демонстрационная программа
 */
#include <stdio.h>

#include "MTF.h"
#include "BWT.h"


/*
 * 1-ый аргумент - имя входного файла
 * 2-ой аргумент - имя закодированного файла
 * 3-ий аргумент - имя дееодированного фалй
 */
int main (int argc, char *argv[])
{
    if (argc < 4) {
        return -1;
    }

    BWT_coding (argv[1], argv[2]); // BWT прямое преобразование

    char alph[256];
    int st;

    st = MTF_coding (argv[2], "MTF_cod.bin", alph); // Сжатие результата

    if (st == -1) return st;

    st = MTF_decoding ("MTF_cod.bin", "MTF_decod.bin", alph); // Разжатие

    BWT_decoding ("MTF_decod.bin", argv[3]); // BWT обратное перобразование

    return 0;
}
