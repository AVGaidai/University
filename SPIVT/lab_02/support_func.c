/*
 * Файл содержит вспомогательные функции необходимые
 * для кодирования и декодирования фалйов
 */
#include <stdio.h>
#include <stdlib.h>

#include <string.h>


/*
 *  * Функция определения "бинарной длины" 
 *   * одного символа размером 1 байт
 *    */
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
 * Функция инициализации алфавита 
 * значениями из таблицы ASCII
 */
void init_alph (char alph[256])
{
    for (int i = 0; i < 256; ++i) {
        alph[i] = (char) i;
    }
}


/*
 * Функция определения позиции элемента
 * со значением val в алфавите alph
 */
int find_into_alph (char alph[256], char val)
{
    for (int i = 0; i < 256; ++i) {
        if (alph[i] == val) {
            return i;
        }
    }
    
    return -1;
}


/*
 * Функция сдвига элемента с позиции pos
 * в начало алфавита alph
 */
void offset_alph (char alph[256], int pos)
{
    for (int i = pos; i > 0; --i) { 
        swap_val (&alph[i], &alph[i -1], sizeof (char));
    }
}

