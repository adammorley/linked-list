#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../mutex/mutex.h"
#include "../node/node.h"
#include "../queue/queue.h"
#include "../tree-node/tree_node.h"
#include "../binary-tree/tree.h"

#include "linked-list.h"

void test_simple() {
    printf("running simple linked list tests\n");
    list* l = list_new();
    assert(l->h == NULL);
    assert(l->t == NULL);
    assert(!mutex_islocked(l->m));
    assert(sizeof(list) == sizeof(l->h) + sizeof(l->t) + sizeof(l->len) + sizeof(l->m));

    list_addH(l, 1);
    assert(list_find(l, 1));
    assert(!list_find(l, 2));
    assert(list_len(l) == 1);
    assert(list_del(l, 1));
    list_addH(l, 1);
    list_addH(l, 2);
    assert(list_find(l, 1));
    assert(list_find(l, 2));
    assert(!list_find(l, 3));
    assert(list_len(l) == 2);

    list_addT(l, 3);
    assert(list_find(l, 1));
    assert(list_find(l, 3));
    assert(!list_find(l, 4));
    assert(list_len(l) == 3);

    assert(list_del(l, 2));
    assert(!list_del(l, 44));
    assert(list_find(l, 1));
    assert(list_find(l, 3));
    assert(!list_find(l, 2));
    assert(list_len(l) == 2);

    list_addT(l, 4);
    list_addT(l, 3);
    assert(list_find(l, 1));
    assert(list_find(l, 3));
    assert(!list_find(l, 2));
    assert(list_find(l, 4));
    assert(list_len(l) == 4);
    assert(list_del(l, 3));
    assert(list_len(l) == 2);

    list* l1 = list_new();
    list_addH(l1, 1);
    list_addH(l1, 2);
    assert(list_find(l1, 1));
    assert(list_find(l1, 2));
    assert(list_len(l) == 2);

    assert(list_join(l, l1));
    assert(list_find(l, 1));
    assert(list_find(l, 2));
    assert(!list_find(l, 3));
    assert(list_find(l, 4));
    assert(list_len(l) == 4);
    assert(list_count(l, 1) == 2);
    node* t = l->h;
    while (t->n != NULL) {
        t = t->n;
    }
    assert(t == l->t);

    assert(1 == list_popT(l));
    assert(list_len(l) == 3);
    assert(1 == list_popH(l));
    assert(list_len(l) == 2);

    list_replace(l, 66);
    assert(list_count(l, 66) == list_len(l));

    // FIXME: add signal handler and catch
    // assert from _pop with empty list

    _list_freeN(l);
    free(l->m);
    free(l);
}

void test_pop() {
    printf("running pop tests\n");
    list* l = list_new();
    list_addH(l, 1);
    list_addH(l, 2);
    list_addH(l, 3);
    assert(list_popT(l) == 1);
    assert(list_popH(l) == 3);
    assert(list_popH(l) == 2);
}

void test_sort() {
    printf("running sort test\n");
    list* l = list_new();
    list_addH(l, 1);
    list_addH(l, 2);
    list_addH(l, 2);
    list_addH(l, 4);
    list_addH(l, 3);
    list* ln = list_sort(l);
}

void test_del_dup() {
    printf("testing duplicate deletion\n");
    list* l = list_new();
    list_addT(l, 1);
    list_addT(l, 2);
    list_addT(l, 3);
    list_addT(l, 1);
    list_addT(l, 3);
    list_del_dup(l);
    assert(list_popH(l) == 1);
    assert(list_popH(l) == 2);
    assert(list_popH(l) == 3);
}

int main(void) {
    test_simple();

    test_pop();

    test_sort();
    
    test_del_dup();

    return 0;
}
