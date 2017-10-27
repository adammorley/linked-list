#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdbool.h>

#include "node.h"

/*
    other functions to impl:

    insertion at arbitrary location
    duplicate removal
    sorting (at insertion or existing)
    split
    move node from list 1 > list 2
    sorted merge (merge two sorted lists)
    merge sort (recursive divide and conquer)
    reverse (super easy!)
*/

typedef struct list list;
struct list {
    node* h;
    node* t;
    unsigned int len; // lazily updated by call to len
};

/*
    helper: is list free?
*/
static bool _empty(list* list);

/*
    add to the head or tail of the list
*/
static void _add(list* list, int d, bool head); // helper
void list_addH(list* list, int d);
void list_addT(list* list, int d);

/*
    count number of occurrences of number
*/
int list_count(list* list, int d);

/*
    find any matching elements and delete
*/
bool list_del(list* list, int d);

/*
    returns true if found in list
*/
bool list_find(list* list, int d);

/*
    joins l0 w/ l1, frees l1, returns combined list
*/
list* list_join(list* l0, list* l1);

/*
    updates cached list size
*/
void list_len(list* list);

/*
    returns ownership
*/
list* list_new(void);

/*
    pop elements
*/
static int _pop(list* list, bool head);
int list_popH(list* list);
int list_popT(list* list);
/*
    print elements of list
*/
void list_print(list* list);

/*
    helper: free nodes in list
*/
void _list_freeN(list* l);

#endif //LINKED_LIST_H
