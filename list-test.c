#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "linked-list.h"

int main(void) {
    list* l = list_new();
    assert(l->h == NULL);
    assert(l->t == NULL);

    list_addH(l, 1);
    assert(list_find(l, 1));
    assert(!list_find(l, 2));
    list_len(l);
    assert(l->len == 1);
    assert(list_del(l, 1));
    list_addH(l, 1);
    list_addH(l, 2);
    assert(list_find(l, 1));
    assert(list_find(l, 2));
    assert(!list_find(l, 3));
    list_len(l);
    assert(l->len == 2);

    list_addT(l, 3);
    assert(list_find(l, 1));
    assert(list_find(l, 3));
    assert(!list_find(l, 4));
    list_len(l);
    assert(l->len == 3);

    assert(list_del(l, 2));
    assert(!list_del(l, 44));
    assert(list_find(l, 1));
    assert(list_find(l, 3));
    assert(!list_find(l, 2));
    list_len(l);
    assert(l->len == 2);

    list_addT(l, 4);
    list_addT(l, 3);
    assert(list_find(l, 1));
    assert(list_find(l, 3));
    assert(!list_find(l, 2));
    assert(list_find(l, 4));
    list_len(l);
    assert(l->len == 4);
    assert(list_del(l, 3));
    list_len(l);
    assert(l->len == 2);

    list* l1 = list_new();
    list_addH(l1, 1);
    list_addH(l1, 2);
    assert(list_find(l1, 1));
    assert(list_find(l1, 2));
    list_len(l1);
    assert(l1->len == 2);

    l = list_join(l, l1);
    assert(list_find(l, 1));
    assert(list_find(l, 2));
    assert(!list_find(l, 3));
    assert(list_find(l, 4));
    list_len(l);
    assert(l->len == 4);
    node* t = l->h;
    while (t->n != NULL) {
        t = t->n;
    }
    assert(t == l->t);

    _list_freeN(l);
    free(l);
    return 0;
}
