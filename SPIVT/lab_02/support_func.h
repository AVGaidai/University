#ifndef SUPPORT_FUNC_H
#define SUPPORT_FUNC_H

#include <stddef.h>

short int bival_length (char *);
void swap_val (void *, void *, size_t);
void init_alph (void *, size_t, size_t);
int find_into_alph (void *, void *, size_t, size_t);
void offset_alph (size_t, void *, size_t, size_t);

#endif
