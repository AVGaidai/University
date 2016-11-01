/*
 * Программа, сожержит функции кодирования и декодирования данных
 * Кодировка выполняется с помощью функции фи2, описанной в методическом пособии ЛР-1.pdf
 * 
 * Этапы кодирования:
 * 1. Порционное считывание данных (по 1-ому байту) из кодируемого файла.
 * 2. Определение "бинарной длины" (length) считавшейся порции данных
 *    (количество значимых бит в двоичном представлении).
 * 3. Определение "бираной длины" (sub_length) от значения "бинарной длины" (length).
 * 4. Кодирование порции данных согласно функции фи2:
 *  4.1. Кодирование "бинарной длины" (sub_length) с помощью функции фи0 (фи0(sub_length)).
 *  4.2. Запись результата, полученного на шаге 3.1. в конец выходного файла.
 *  4.3. Запись 'sub_length - 1' младших бит значения "бинарной длины" (length)
 *       в конец выходного файла.
 *  4.4. Запись, считанной порции данных без ведущего единичного бита в 
 *       двоичном представлении, в конец выходного файла.
 * 5. Повтор шагов 1-4, до тех пор, пока не закончится содержимое кодируемого файла.
 *
 * Этапы декодирования:
 *  
 *  Порционное считывание данных
 *  (по 1-ому байту) из входного файла
 *  происходит по мере необходимости!
 *
 * 1. Определение "бинарной длины" (length) закодированной порции данных
 *    (подсчёт подрядидущих нулевых бит до первого ненулевого бита).
 * 2. Сичтывание 'length' следующих бит (включая первый ненулевой бит с шага 1).
 * 3. Декодирование полученных данных согласно функции фи1 (результат число N).
 * 4. Считывание 'N - 1' следующих бит, N-ый бит установлен в единицу (нумерация с 1).
 * 5. Запись результата, полученного на шаге 4, в конец выходного файла.
 * 6. Повтор шагов 1-5, до тех пор, пока не закончится содержимое декодируемого файла.
 */
#include <stdio.h>

#include "support_func.h"


/*
 * Функция кодирования данных c помощью фи2
 * in_file  - входной файл
 * out_file - выходной файл
 */
void fi_2_coding (char *in_file, char *out_file)
{
    FILE *fin, *fout;

    fin  = fopen (in_file, "rb");
    fout = fopen (out_file, "wb");

    char c_8;     // Буфер для считывания порции данных (8 бит)
    char buf;     // Буфер для нахождения "бинарной длины" от "бинарной длины"
    char c = 0x0; // Буфер для записи закодированной последовательности (8 бит)
    int  i = 0;   // Счётчик записанных нулевых бит Х 8

    short int c_16;       // Буфер для преобразования символа в число (16 бит)
    short int length;     // "Бинарная длина"
    short int sub_length; // "Бинарная длина" "бинарной длины"

    /* Текущая позиции обрабатываемого бита в буфере 'c' */
    short int cur_bit_pos = 7;

    /* Порционное считывание данных (по 1-ому байту) */
    while ( fread (&c_8, 1, sizeof (char), fin) ) {
        /* Определение длины (length) */
        length = bival_length (&c_8);

        buf = (char) length;
        /* Определение поддлины (sub_length) */
        sub_length = c_16 = bival_length (&buf); 

        /* Обработка оставшегося места в буфере 'c' */
        if (c != 0x0 || cur_bit_pos < 7) {
            if (c_16 - (cur_bit_pos + 1) >= 0) {
                c_16 -= cur_bit_pos + 1;
                fwrite (&c, 1, sizeof (char), fout);
                c = 0x0;
            } else {
                c_16 = 7 - (cur_bit_pos - c_16);
            }
        }

        /* Запись в конец выходного файла нулевых бит Х 8 */
        for (i = 0; i < c_16 / 8; ++i) {
            fwrite (&c, 1, sizeof (char), fout);
        }

        /* Добавление единичного бита в закодированную последовательность */
        c |= 1 << (7 - (c_16 - i * 8));
        cur_bit_pos = 7 - (c_16 - i * 8) - 1;

        --sub_length;  // Отбрасывание ведущего единичного бита

       /*
        * Добавление в буфер 'c' кодируемого символа 
        * без ведущего единичного бита в двоичном представлении
        */
        while ( (sub_length > 0) && (cur_bit_pos >= 0) ) {
            if ( (length >> (sub_length - 1)) & 1 ) {
                c |= 1 << cur_bit_pos;
            } 
            --cur_bit_pos;
            --sub_length;
        }
        
        /* 
         * Если буфер 'c' заполнился, 
         * сбросить его содержимое в конец выходного файла
         */
        if (sub_length > 0) {
            fwrite (&c, 1, sizeof (char), fout);
            c = 0x0;
            cur_bit_pos = 7;
        }

        /* Обработка оставшейся части кодируемого символа */
        while (sub_length > 0) {
            if ( (length >> (sub_length - 1)) & 1 ) {
                c |= 1 << cur_bit_pos;
            } 
            --cur_bit_pos;
            --sub_length;
        } 

       /*
        * Добавление значения считанной порции данных
        * без ведущего единичного бита
        */
        --length;    // Отбрасывание ведущего единичного бита
 
       /*
        * Добавление в буфер 'c' кодируемого символа 
        * без ведущего единичного бита в двоичном представлении
        */
        while ( (length > 0) && (cur_bit_pos >= 0) ) {
            if ( (c_8 >> (length - 1)) & 1 ) {
                c |= 1 << cur_bit_pos;
            } 
            --cur_bit_pos;
            --length;
        }

        /* 
         * Если буфер 'c' заполнился, 
         * сбросить его содержимое в конец выходного файла
         */
        if (length > 0) {
            fwrite (&c, 1, sizeof (char), fout);
            c = 0x0;
            cur_bit_pos = 7;
        }

        /* Обработка оставшейся части кодируемого символа */
        while (length > 0) {
            if ( (c_8 >> (length - 1)) & 1 ) {
                c |= 1 << cur_bit_pos;
            } 
            --cur_bit_pos;
            --length;
        } 
        

        /* 
         * Если буфер 'c' заполнился, 
         * сбросить его содержимое в конец выходного файла
         */
        if (cur_bit_pos == -1) {
            fwrite (&c, 1, sizeof (char), fout);
            c = 0x0;
            cur_bit_pos = 7;
        }

    }
    
   /* 
    * Если буфер 'c' не пуст, 
    * сбросить его содержимое в конец выходного файла
    */
    if (cur_bit_pos != 7) {
        fwrite (&c, 1, sizeof (char), fout);
    }

    fclose (fin);
    fclose (fout);
}




/*
 * Функция декодирования данных с помощью фи2
 * in_file  - входной файл
 * out_file - выходной файл
 */
void fi_2_decoding (char *in_file, char *out_file)
{
    FILE *fin, *fout;

    fin  = fopen (in_file, "rb");
    fout = fopen (out_file, "wb");

    char c_8;      // Буфер для считывания порции данных (8 бит)
    char c  = 0x0; // Буфер для промежуточного декодирования
    char c1 = 0x0; // Буфер для записи декодированного символа в выходной файл

    short int length = 0; // "Бинарная длина"

    /* Порционное считывание закодированных данных (по 1 байту) */
    while ( fread (&c_8, 1, sizeof (char), fin) ) {
        /* Обработка порции данных */
        for (int i = 7; i >= 0; --i) {
            /* Если встретился ненулевой бит */
            if ( (c_8 >> i) & 1 ) {
                /* Если закодирован ноль */
                if (!length) {
                    c = 0x0;
                /* Финальное декодировние символа */
                } else if (c1) {
                    c1 |= 1 << (--length);
                /* Промежуточное декодирование символа */
                } else {
                    c |= 1 << (--length);
                }
            } else {
                if (c || c1) {
                    --length;  // Декодирование символа
                } else {
                    ++length;  // Подсчёт "бинарной длины"
                }
            }
            
            /* Переход к стадии финального декодирования */
            if (!length && !c1) {

                length = (short int) c;
                /* Если закодирован ноль */
                if (!length) {
                    c1 = 0x0;
                /* Установка ведущего бита на позиции length (нумерация с 1) */
                } else {
                    c1 |= 1 << (--length);
                }
                /* Если закодирован ноль */
                if (!length) {
                    fwrite (&c1, 1, sizeof (char), fout);
                    c1 = c = 0x0;  // Сброс буфера 'c'
                }
            /* Запись декодированного символа в конец выходного файла */
            } else if (!length) {
                fwrite (&c1, 1, sizeof (char), fout);
                c1 = c = 0x0;  // Сброс буфера 'c'
            }
        }
    }

    fclose (fin);
    fclose (fout);
}