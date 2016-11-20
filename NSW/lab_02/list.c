#include <unistd.h>


struct sl_list_node {

    void   *val;

    struct sl_list_node *next;

};


struct sl_list {

    int nmemb;

    struct sl_list_node *head;

    size_t size;
};


struct sl_list *sl_list_malloc (void)
{
    return (struct sl_list *) malloc (sizeof (struct sl_list));
}


void sl_list_create (struct sl_list *list, size_t size)
{
    list = sl_list_malloc ();

    list->nmemb = 0;
    list->head = NULL;

    list->size = size;
}


void sl_list_remove (struct sl_list *list)
{
   /* To do */
    free (list);
}


struct sl_list_node *sl_list_node_malloc (void)
{
    return (struct sl_list_node *) malloc (sizeof (struct sl_list_node));
}


void sl_list_node_create ( struct sl_list_node* list_node,
                           void *val, size_t size          )
{
    list_node = sl_list_node_malloc ();
    list_node->val = malloc (size);
    memcpy (list_node->val, val, size);
}


void sl_list_node_free (struct sl_list_node* list_node)
{
    free (list_node->val);
    free (list_node);
}


void sl_list_insert (struct sl_list *list, void *val)
{
    struct sl_list_node *list_node;

    list_node_create (list_node, val, list->size);
    list_node->next = list->head;
    list->head = list_node;
    list->size++;
}



