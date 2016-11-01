/*
 * Файл содержит вспомогательные функции необходимые
 * для кодирования и декодирования фалйов
 */
//#include <stdio.h>
#include <stdlib.h>

#include <string.h>


/*
 * Функция определения "бинарной длины" 
 * одного символа размером 1 байт
 */
short int bival_length (char *val)
{
    /* Максимальная длина может равняться 8 битам */
    short int length = 8;

    /* Поиск первого ненулевого бита (слева направа) */
    while ( !(*val >> (length - 1)) ) {
        if ( !(--length) ) {
            break;
        }
    }

    return length;
}


/*
 * Функция перестановки значений a и b местами
 * size - размер одного элемента
 */
void swap_val (void *a, void *b, size_t size)
{
    void *tmp;   // Временное значение

    tmp = malloc (size);

    memmove (tmp, a, size);  // tmp = a
    memmove (a, b, size);    // a = b
    memmove (b, tmp, size);  // b = tmp
    
    free (tmp);
}


/*
 * Функция инициализации алфавита alph, состоящего из nmemb элементов
 * по size байт каждый, значениями из таблицы ASCII
 */
void init_alph (void *alph, size_t size, size_t nmemb)
{

    for (size_t i = 0; i < nmemb; ++i) {
        memmove (alph + i * size, &i, size);
    }
}


/*
 * Функция определения позиции элемента
 * со значением val в алфавите alph, состоящего 
 * из nmemb элементов по size байт каждый
 * compar () - функция сравнения элементов
 */
int find_into_alph ( void *val, void *alph, size_t size,
                     size_t nmemb, int (*compar) (void *, void *) )
{
    for (size_t i = 0; i < nmemb; ++i) {
        if ( compar (alph + i * size, val) == 0 ) {
            return i;
        }
    }
    
    return -1;
}


/*
 * Функция сдвига элемента с позиции pos
 * в начало алфавита alph, состоящего 
 * из nmemb элементов по size байт каждый
 */
void offset_alph (size_t pos, void *alph, size_t size, size_t nmemb)
{
    for (size_t i = pos; i > 0; --i) { 
        swap_val (alph + i * size, alph + (i - 1) * size, size);
    }
}


/*
 * Функция реверса элементов алфавита alph, состоящего 
 * из nmemb элементов по size байт каждый
 */
void reverse_alph (void *alph, size_t size, size_t nmemb)
{
    for (size_t i = 0, j = nmemb - 1; i < j; ++i, --j) {
        swap_val (alph + i * size, alph + j * size, size);
    }
}


void cyc_offset_alph (void *alph, size_t size, size_t nmemb)
{
    for (int i = 1; i < nmemb; ++i) {
        swap_val (alph + i * size, alph + (i - 1) * size, size);
    }
}

