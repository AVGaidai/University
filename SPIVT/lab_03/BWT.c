/*
 * Преобразование Берроуза-Уилера (BWT)
 * Данное преобразование позволяет выстроить символы
 * в порядке следования контекстов, т.е. символы,
 * идущие в одном контексте, групируются вместе.
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "support_func.h"


#define BUF_SIZE 120  // Размер буфера в байтах


/*
 * Структура необходимая для построения 
 * массивов в функции декодирования
 */
struct PAIR {

    size_t  pos;  // Позиция символа в массиве
    char    val;  // Значение элемента

   /*
    * Флаг проверки на использование:
    * 1 - элемент использовался;
    * 0 - --/-- не использовался.
    */
    short   flg;
};



/*
 * Функция сравнения для поиска элемента
 * с помощью пользовательской функции 
 * find_into_alph () определённой в "support_func.h"
 */
int BWT_find_cmp (void *a, void *b)
{
    struct PAIR A = *(struct PAIR *) a;
    struct PAIR B = *(struct PAIR *) b;

    int res;
    
    /* Сравнение элементов по их значению */
    res = memcmp (&A.val, &B.val, sizeof (char));
    
    if (res == 0 && A.flg == 1) {
       /* 
        * Если значения равны, но данный
        * элемент уже проверялся, тогда
        * он не является искомым
        */
        return 1;
    } else if (res == 0) {
       /*
        * Если значения равны и элемент
        * ранее не проверялся, тогда
        * устанавливаем флаг использования
        */
        A.flg = 1;
        memcpy (a, &A, sizeof (struct PAIR));
    }

    return res;  // Возвращаем результат сравнения
}


/*
 * Функция сравнения элементов а и b для сортировки (qsort_r)
 * двумерного массива состоящего из циклических сдвигов
 */
int compare_1 (const void *a, const void *b, void *arg)
{
    size_t size = *(size_t *) arg; // Размер одномерного массива

    char *A, *B;

    A = (char *) malloc (size); // Элемент a
    B = (char *) malloc (size); // Элемент b

    memcpy (A, a, size);
   /* Реверсирование массива для сортировки справа налево*/
    reverse_alph (A, sizeof (char), size);
    A[0] = 0x0; // Сортировка с предпоследнего элемента 1D-массива

    memcpy (B, b, size);
   /* Реверсирование массива для сортировки справа налево */
    reverse_alph (B, sizeof (char), size);
    B[0] = 0x0; // Сортировка с предпоследнего элемента 1D-массива
   
    int res = memcmp (A, B, size);  // Результат сровнения

    free (A);
    free (B);

    return res;
}


/*
 * Функция сравнения элементов a и b для сортировки (qsort)
 * массива состоящего из пар <значение, позиция> (+ флаг)
 */
int compare_2 (const void *a, const void *b)
{
    struct PAIR A, B;

    A = *(struct PAIR *) a; // Элемент a
    B = *(struct PAIR *) b; // Элемент b

    int res;
    
   /* Сравнение по значению */
    res = memcmp (&A.val, &B.val, sizeof (char));
 
    if (res == 0) {
       /* Если значения равны, сравнить по позиции */
        res = memcmp (&A.pos, &B.pos, sizeof (size_t));
    }

    return res;
}


/*
 * Функция прямого преобразования Берроуза-Уилера (BWT)
 * in_file  - имя входного файла
 * out_file - имя выходного файла
 */
void BWT_coding (char *in_file, char *out_file)
{
   /* Одномерный буфер для считывания порции данных */
    char buf_1D[BUF_SIZE];

   /* Двумерный буфер для хранения циклических сдвигов */
    char buf_2D[BUF_SIZE][BUF_SIZE];

   /*
    * Буфер для результата преобразования:
    * BUF_SIZE преобразованных символов +
    * позиция в данной последовательности 
    * первого символа исходного сообщения
    */
    char res[BUF_SIZE + 1]; 

    FILE *fin, *fout;

    fin  = fopen (in_file, "rb");
    fout = fopen (out_file, "wb");

    fpos_t cur_fp; // Текущая позиция файлового указателя (ФУ)

    fgetpos (fin, &cur_fp); // Определение текущей позиции ФУ

   /*
    * Порционное считывание данных по BUF_SIZE
    * байт в buf_1D из входного файла
    */
    while (fread (buf_1D, BUF_SIZE, 1, fin)) {
        fgetpos (fin, &cur_fp);   // Определение текущей позиции ФУ
       
       /* 
        * Установка недействительного значения позиции
        * первого символа исходного сообщения
        */ 
        res[BUF_SIZE] = BUF_SIZE;

       /* 
        * Заполнение двумерного массива buf_2D
        * циклицескими сдвигами исходного сообщения
        */
        memcpy (buf_2D[0], buf_1D, BUF_SIZE);
        for (int i = 1; i < BUF_SIZE; ++i) {
            memcpy (buf_2D[i], buf_2D[i - 1], BUF_SIZE);
            cyc_offset_alph (buf_2D[i], sizeof (char), BUF_SIZE);
        }

        size_t size = BUF_SIZE;

       /* 
        * Сортировка циклических сдвигов справа налево
        * начиная с предпоследнего элемента
        */
        qsort_r (buf_2D, BUF_SIZE, BUF_SIZE, compare_1, &size);

       /* Оформление результата преобразования */
        for (int i = 0; i < BUF_SIZE; ++i) {
            res[i] = buf_2D[i][BUF_SIZE - 1];
           /*
            * Определение позиции первого символа исходного
            * сообщения в преобраззованной последовательности
            */
            if (res[BUF_SIZE] == BUF_SIZE) {
                res[BUF_SIZE] = (memcmp (buf_1D + 1, buf_2D[i], BUF_SIZE - 1))
                              ? BUF_SIZE : (char) i;
            }
        }

        fwrite (res, BUF_SIZE + 1, 1, fout); // Запись реузльтата в вых. файл
    }

    fsetpos (fin, (const fpos_t *) &cur_fp); // Установка послед. значения ФУ

    size_t r_bytes = 0;   // Размер оставшейся части файла

    for (int i = 0; ; ++i) {
        if (fread (&buf_1D[i], sizeof (char), 1, fin)) {
            ++r_bytes;
        } else {
            break;
        }
    }

   /* Если файл полностью преобразован */
    if (r_bytes == 0) {
        fclose (fin);
        fclose (fout);
        return;
    }

   /* Одномерный буфер для считывания оставшихся данных */
    char rem_1D[r_bytes];
 
   /* Двумерный буфер для хранения циклических сдвигов */
    char rem_2D[r_bytes][r_bytes];

   /*
    * Буфер для результата преобразования:
    * r_bytes преобразованных символов +
    * позиция в данной последовательности 
    * первого символа исходного сообщения
    */
    char res1[r_bytes + 1];

    memcpy (rem_1D, buf_1D, r_bytes);    

   /* 
    * Установка недействительного значения позиции
    * первого символа исходного сообщения
    */ 
    res1[r_bytes] = r_bytes;

   /* 
    * Заполнение двумерного массива buf_2D
    * циклицескими сдвигами исходного сообщения
    */
    memcpy (rem_2D[0], rem_1D, r_bytes);
    for (int i = 1; i < r_bytes; ++i) {
        memcpy (rem_2D[i], rem_2D[i - 1], r_bytes);
        cyc_offset_alph (rem_2D[i], sizeof (char), r_bytes);
    }

   /* 
    * Сортировка циклических сдвигов справа налево
    * начиная с предпоследнего элемента
    */
    qsort_r (rem_2D, r_bytes, r_bytes, compare_1, &r_bytes);

   /* Оформление результата преобразования */
    for (int i = 0; i < r_bytes; ++i) {
        res1[i] = rem_2D[i][r_bytes - 1];
       /*
        * Определение позиции первого символа исходного
        * сообщения в преобраззованной последовательности
        */
        if (res1[r_bytes] == r_bytes) {
            res1[r_bytes] = (memcmp (rem_1D + 1, rem_2D[i], r_bytes - 1))
                          ? r_bytes : (char) i;
        }
    }

    fwrite (res1, r_bytes + 1, 1, fout); // Запись результата в вых. файл

    fclose (fin);
    fclose (fout);
}


/*
 * Функция обратного преобразования Берроуза-Уилера (BWT)
 * in_file  - имя входного файла
 * out_file - имя выходного файла
 */
void BWT_decoding (char *in_file, char *out_file)
{
   /*
    * Буфер для чтения преобразовванной порции данных:
    * BUF_SIZE преобразованных символов +
    * позиция в данной последовательности 
    * первого символа исходного сообщения
    */
    char buf[BUF_SIZE + 1];

   /*
    * Буфер для результата преобразования:
    * BUF_SIZE преобразованных символов
    */
    char res[BUF_SIZE];

    FILE *fin, *fout;

    fin  = fopen (in_file, "rb");
    fout = fopen (out_file, "wb");
 
    fpos_t cur_fp; // Текущая позиция файлового указателя (ФУ)

    fgetpos (fin, &cur_fp); // Определение текущей позиции ФУ

   /*
    * Порционное считывание данных по BUF_SIZE + 1
    * байт в buf из входного файла
    */
    while (fread (buf, BUF_SIZE + 1, 1, fin)) {
        fgetpos (fin, &cur_fp);  // Определение текущей позиции ФУ


        struct PAIR alph_1[BUF_SIZE];  // Массив пар №1
        struct PAIR alph_2[BUF_SIZE];  // Массив пар №2

       /* Инициализация массива №1 */
        for (int i = 0; i < BUF_SIZE; ++i) {
            alph_1[i].pos = i;
            alph_1[i].val = buf[i];

            alph_1[i].flg = 0;
        }

        memcpy (alph_2, alph_1, sizeof (struct PAIR) * BUF_SIZE);

       /* Сортировка массива №1 */
        qsort (alph_1, BUF_SIZE, sizeof (struct PAIR), compare_2);

       /* Инициализация массива №2 с переопределением позиций элементов */
        for (int i = 0; i < BUF_SIZE; ++i) {
            alph_2[i].pos = find_into_alph ( &alph_2[i], alph_1,
                                             sizeof (struct PAIR),
                                             BUF_SIZE, BWT_find_cmp );
        }

       /* Позиция первого символа в исходном сообщении */
        size_t start = (size_t) buf[BUF_SIZE];
        size_t pos = alph_2[start].pos;        // Позиция перехода


       /* Оформление результата обратного преобразования */
        res[0] = alph_2[start].val;
        for (int i = 1; i < BUF_SIZE; ++i) {
            res[i] = alph_2[pos].val;
            pos = alph_2[pos].pos;
        }

        fwrite (res, BUF_SIZE, 1, fout);  // Запись результата в вых. файл
    }

    fsetpos (fin, (const fpos_t *) &cur_fp); // Уст. послед. знач. ФУ

    size_t r_bytes = -1;  // Количество оставшихся данных

    for (int i = 0; ; ++i) {
        if (fread (&buf[i], sizeof (char), 1, fin)) {
            ++r_bytes;
        } else {
            break;
        }
    }

   /* Если все данные перобразованны */
    if (r_bytes == -1) {
        fclose (fin);
        fclose (fout);
        return;
    }

   /*
    * Буфер для чтения оставшихся данных
    * r_bytes преобразованных символов +
    * позиция в данной последовательности 
    * первого символа исходного сообщения
    */ 
    char rem[r_bytes + 1];

   /*
    * Буфер для результата преобразования:
    * r_bytes преобразованных символов
    */
    char res1[r_bytes];

    memmove (rem, buf, r_bytes + 1);    

    struct PAIR alph_1[r_bytes];   // Массив пар №1
    struct PAIR alph_2[r_bytes];   // Массив пар №2

   /* Инициализация массива №1 */
    for (int i = 0; i < r_bytes; ++i) {
        alph_1[i].pos = i;
        alph_1[i].val = rem[i];

        alph_1[i].flg = 0;
    }

    memcpy (alph_2, alph_1, sizeof (struct PAIR) * r_bytes);

   /* Сортировка массива №1 */
    qsort (alph_1, r_bytes, sizeof (struct PAIR), compare_2);

   /* Инициализация массива №2 с переопределением позиций элементов */
    for (int i = 0; i < r_bytes; ++i) {
        alph_2[i].pos = find_into_alph ( &alph_2[i], alph_1,
                                         sizeof (struct PAIR),
                                         r_bytes, BWT_find_cmp );
    }

   /* Позиция первого символа в исходном сообщении */
    size_t start = (size_t) rem[r_bytes];
    size_t pos = alph_2[start].pos;       // Позиция перехода

   /* Оформление результата преобразования */
    res1[0] = alph_2[start].val;
    for (int i = 1; i < BUF_SIZE; ++i) {
        res1[i] = alph_2[pos].val;
        pos = alph_2[pos].pos;
    }

    fwrite (res1, r_bytes, 1, fout);     // Запись результата в вых. файл

    fclose (fin);
    fclose (fout);
}

