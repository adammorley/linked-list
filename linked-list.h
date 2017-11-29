#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdbool.h>

#include "node.h"
#include "../mutex/mutex.h"

/*
    other functions to impl:

    merge sort (recursive divide and conquer)
*/

typedef struct list list;
struct list {
    node* h;
    node* t;
    unsigned long len;
    mutex* m;
};

/*
    add to the head or tail of the list
*/
void list_addH(list* list, long d);
void list_addT(list* list, long d);

/*
    count number of occurrences of number
*/
long list_count(list* list, long d);

/*
    find any matching elements and delete
*/
bool list_del(list* list, long d);

/*
    returns true if found in list
*/
bool list_find(list* list, long d);

/*
    joins l0 w/ l1, frees l1, l0 contains combination
    returns false if mutex not lockable (can re-drive)
*/
bool list_join(list* l0, list* l1);

/*
    return current list length
*/
unsigned long list_len(list* list);

/*
    returns ownership
*/
list* list_new(void);

/*
    pop elements
*/
long list_popH(list* list);
long list_popT(list* list);

/*
    print elements of list
*/
void list_print(list* list);

/*
    replace all elements in list with d
*/
void list_replace(list* list, long d);

/*
    helper: free nodes in list
*/
void _list_freeN(list* l);

#endif //LINKED_LIST_H
