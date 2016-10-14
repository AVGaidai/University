/*
 * list
 */
#include <stdlib.h>

#include <string.h>

#include "error.h"


/*
 * Struct the element of the list
 */
struct node {

    void *content;     // Content

    struct node *next; // Pointer to next element

};


/*
 * Struct the list
 */
struct list {

    int size;          // List size

    struct node *head; // Pointer to first element in list
    struct node *tail; // Pointer to last element in list

};



/*
 * List initializer
 * list_ptr -- pointer to list
 */
void list_init (struct list *list_ptr)
{
   /*
    * List is empty
    */
    list_ptr->size = 0;   
    list_ptr->head = NULL;
    list_ptr->tail = NULL;
}


/*
 * Adding element to the top of the list
 * list_ptr -- pointer to list
 * content  -- adding element
 * len      -- size of element
 */
int list_push_front (struct list *list_ptr, void *content, long int len)
{
    struct node *node_ptr;

   /*
    * Allocating memory for node of the list
    */
    node_ptr = (void *) malloc (sizeof (struct node));
    if (!node_ptr) {
        return print_err ("list_elem_add ()");
    }
   /*
    * Allocating memory for content of the node
    * Adding content
    */
    node_ptr->content = (void *) malloc (len);
    memcpy (node_ptr->content, content, len);

   /*
    * If list is empty
    */
    if (list_ptr->head == NULL) {
        list_ptr->tail = node_ptr;
    }

   /*
    * List is no empty
    */
    node_ptr->next = list_ptr->head;
    list_ptr->head = node_ptr;
    ++list_ptr->size;       


    return 0;
}


/*
 * Adding element to the end of the list
 * list_ptr -- pointer to list
 * content  -- adding element
 * len      -- size of element
 */
int list_push_back (struct list *list_ptr, void *content, long int len)
{
    struct node *node_ptr;

   /*
    * Allocating memory for node of the list
    */
    node_ptr = (void *) malloc (sizeof (struct node));
    if (!node_ptr) {
        return print_err ("list_elem_add ()");
    }
   /*
    * Allocating memory for content of the node
    * Adding content
    */
    node_ptr->content = (void *) malloc (len);
    memcpy (node_ptr->content, content, len);
    node_ptr->next = NULL;

   /*
    * If list is empty
    */
    if (list_ptr->head == NULL) {
        list_ptr->head = node_ptr;
        list_ptr->tail = node_ptr;
   /*
    * If list is no empty
    */
    } else {
        list_ptr->tail->next = node_ptr;
        list_ptr->tail = node_ptr;
    }
    ++list_ptr->size;       

    return 0;
}



/*
 * Free memory (remove list)
 * list_ptr -- pointer to list
 */
void list_free (struct list *list_ptr)
{
    struct node *node_ptr;

    for ( node_ptr  = list_ptr->head; 
          node_ptr != NULL          ;  
          node_ptr = list_ptr->head   ) {
        list_ptr->head = node_ptr->next;
        free (node_ptr->content);  // Free content of the node
        free (node_ptr);           // Free node of the list
    }
   /*
    * List is empty
    */
    list_ptr->head = NULL;
    list_ptr->tail = NULL;
    list_ptr->size = 0;
}



/*
 * Finding element of the list
 * list_ptr -- pointer to list
 * dest     -- found element
 * src      -- finding element
 * len      -- size of element
 * func_cmp -- pointer to function for to compare of the elements
 *             param1 -- first parameter (element of the list)
 *             param2 -- second parameter (element --//--)
 *             l      -- size of parameters (size of elements --//--)
 */
int list_elem_find ( struct list *list_ptr                       , 
                     void *dest                                  ,
                     void *src                                   ,
                     long int len                                ,
                     int (*func_cmp) 
                         (void *param1, void *param2, long int l)  )
{
    struct node *node_ptr;

   /*
    * Default compare
    */
    if (func_cmp == NULL) {
        for ( node_ptr  = list_ptr->head;
              node_ptr != NULL          ;
              node_ptr  = node_ptr->next  ) {
            if (!memcmp (node_ptr->content, src, len)) {
               /*
                * Found element
                */
                break;
            }
        }
        if (node_ptr == NULL) {
           /*
            * Don't found element
            */
            return -1;
        }
   /*
    * User copmare function
    */
    } else {
        for ( node_ptr  = list_ptr->head;
              node_ptr != NULL          ;
              node_ptr  = node_ptr->next  ) {
            if (!func_cmp (node_ptr->content, src, len)) {
               /*
                * Found element
                */
                break;
            }
        }
        if (node_ptr == NULL) {
           /*
            * Don't found element
            */
            return -1;
        }
    }
   /*
    * Copy found element
    */
    memcpy (dest, node_ptr->content, len);

    return 0;
}



/*
 * Remove element from the list
 * list_ptr -- pointer to list
 * content  -- removing element
 * len      -- size of element
 */
int list_elem_remove (struct list *list_ptr, void *content, long int len)
{
    struct node *node_ptr, *node_prev;

    for ( node_ptr  = list_ptr->head, node_prev = NULL; 
          node_ptr != NULL                            ;
          node_ptr  = node_ptr->next                    ) {
        if (memcmp (content, node_ptr->content, len) == 0) {
            if (node_prev == NULL) {
                if (node_ptr == list_ptr->tail) {
                    list_ptr->tail = NULL;
                }
                list_ptr->head = node_ptr->next;
                free (node_ptr->content);  // Free content of the node
                free (node_ptr);           // Free node of the list 
                --list_ptr->size;          // Dicrement number elements
                return 0;                  // Success
            } else {
                if (node_ptr == list_ptr->tail) {
                    list_ptr->tail = node_prev;
                }
                node_prev->next = node_ptr->next;;
                free (node_ptr->content);  // Free content of the node
                free (node_ptr);           // Free node of the list 
                --list_ptr->size;          // Dicrement number elements
                return 0;                  // Success
            }
        } else {
            node_prev = node_ptr;          // Next iteration
        }
    }

    return -1;  // Element not found
}



int list_first_elem_remove (struct list *list_ptr)
{
    if (!list_ptr->size) {
        return print_war ("List is empty!");
    }

    struct node *node_ptr;

    node_ptr = list_ptr->head;
    list_ptr->head = node_ptr->next;
    if (list_ptr->size == 1) {
        list_ptr->tail = NULL;
    }
    free (node_ptr->content);
    free (node_ptr);

    --list_ptr->size;

    return 0;
}



/*
 * Action to all elements of the list
 * list_ptr -- pointer to list
 * len      -- size of element
 * func_act -- pointer to action function
 *             elem -- element of the list
 *             size -- size of element
 */
int list_elem_act ( struct list *list_ptr                       , 
                     long int len                               ,
                     int (*func_act) (void *elem, long int size)  )
{
    struct node *node_ptr;
    int          status;

   /*
    * Non action
    */
    if (func_act == NULL) {
            return 0;
    }
   /*
    * User action function
    */
    for ( node_ptr  = list_ptr->head;
          node_ptr != NULL          ;
          node_ptr  = node_ptr->next  ) {
        status = func_act (node_ptr->content, len);
        if (status == -1) {
            return -1;
        }
    }

    return 0;
}


