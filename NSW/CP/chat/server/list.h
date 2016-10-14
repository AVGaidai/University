#ifndef LIST_H
#define LIST_H

struct node {

    void *content;     // Content

    struct node *next; // Pointer to next element

};


struct list {

    int size;          // List size

    struct node *head; // Pointer to first element in list

};


/*
 * Prototype
 */
void list_init (struct list *);
int  list_push_front (struct list *, void *, long int);
int  list_push_back (struct list *, void *, long int);
void list_free (struct list *);
int  list_elem_find ( struct list *, void *, void *, long int,
                      int (*func_cmp) (void *, void *, long int) );
int  list_elem_remove (struct list *, void *, long int);
int  list_first_elem_remove (struct list *);
int  list_elem_act ( struct list *, long int, 
                     int (*func_act) (void *, long int) );



#endif
