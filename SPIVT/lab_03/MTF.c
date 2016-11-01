/*
 * Метод сжатия "Стопка книг"
 * Промежуточная кодирвока с помощью фи2
 */
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "fi.h"
#include "support_func.h"


/*
 * Функция сравнения для поиска желемента в алфавите
 */
int MTF_find_cmp (void *a, void *b)
{
    return memcmp (a, b, sizeof (char));
}


/*
 * Функция сжатия и кодирования входного файла in_file
 * Результат записывается в выходной файл out_file
 * Для сжатия используется алфавит alph[256], 
 * который инициализируется значениями ASCII таблицы
 */
int MTF_coding (char *in_file, char *out_file, char alph[256])
{
    FILE *in, *out;

    in  = fopen (in_file, "rb");        // Входной файл
    out = fopen ("tmp_file.bin", "wb"); // Времнный файл для сжатия

    char buf;   // Буффер
    int  pos;   // Позиция считанного символа в текущем алфавите
       
    init_alph (alph, sizeof (char), 256);  // Инициализация алфавита
 
    while (fread (&buf, 1, sizeof (char), in)) {
        /* Определение позции символа в алфавите */
        pos = find_into_alph (&buf, alph, sizeof (char), 256, MTF_find_cmp);
        if (pos == -1) {
            fclose (in);
            fclose (out);
            printf ("error: incorrect alph!\n");
            return -1;
        }
        buf = (char) pos;
        /* Запись во временный файл позиции считанного символа */
        fwrite (&buf, 1, sizeof (char), out);
        /* Сдвиг элементов алфавита согласно метода сжатия */
        offset_alph (pos, alph, sizeof (char), 256);
    }

    fclose (in);
    fclose (out);

    /* Кодирование сжатых данных с помощью функции фи2 */
    fi_1_coding ("tmp_file.bin", out_file);

    remove ("./tmp_file.bin");   // Удаление временного файла

    return 0;
}


/*
 * Функция декодирования и восстановления данных
 * по сжатому и закодированному файлу in_file
 * Результат записывается в выходной файл out_file
 * Для восстановления данных используется алфавит alph[256], 
 * который инициализируется значениями ASCII таблицы
 */
int MTF_decoding (char *in_file, char *out_file, char alph[256])
{
    /* Декодирование данных с помощью функции фи2 */
    fi_1_decoding (in_file, "tmp_file.bin");

    FILE *in, *out;

    in  = fopen ("tmp_file.bin", "rb"); // Временный файл для декодирования
    out = fopen (out_file, "wb");       // Выходной файл

    char buf;   // Буффер
    int  pos;   // Позиция считанного символа в текущем алфавите

    init_alph (alph, sizeof (char), 256); // Инициализация алфавита


    while (fread (&buf, 1, sizeof (char), in)) {
        pos = (int) buf;
        /* Восстановление данных по алфавиту */
        fwrite (&alph[pos], 1, sizeof (char), out);
        /* Сдвиг элементов алфавита согласно метода сжатия */
        offset_alph (pos, alph, sizeof (char), 256);
    }

    fclose (in);
    fclose (out);

    remove ("./tmp_file.bin");   // Удаление временного файла

    return 0;
}


