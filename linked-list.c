#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../log/log.h"
#include "../mutex/mutex.h"
#include "../node/node.h"
#include "../queue/queue.h"
#include "../tree-node/tree_node.h"
#include "../binary-tree/tree.h"

#include "linked-list.h"

/*
    static functions
*/
static void _add(list* l, long d, bool head);
static bool _empty(list* l);
static void _len(list* l, long n);
static long _pop(list* l, bool head);

static void _add(list* l, long d, bool head) {
    node* n = node_new(d);
    mutex_spinlock(l->m);
    if (_empty(l)) {
        l->h = n;
        l->t = n;
    }
    else if (head) {
        l->h->p = n;
        n->n = l->h;
        l->h= n;
    }
    else {
        l->t->n = n;
        n->p = l->t;
        l->t = n;
    }
    _len(l, 1);
    mutex_unlock(l->m);
}

static bool _empty(list* l) {
    assert(mutex_islocked(l->m));
    if (l->h == NULL && l->t == NULL) {
        return true;
    }
    return false;
}

static void _len(list* l, long n) {
    __atomic_add_fetch(&l->len, n, __ATOMIC_RELEASE);
}

static long _pop(list* l, bool head) {
    node* n;
    long d;
    mutex_spinlock(l->m);
    if (l->h == NULL || l->t == NULL) assert(false);
    if (head) {
        n = l->h;
        l->h = n->n;
        if (l->h != NULL) l->h->p = NULL;
    }
    else {
        n = l->t;
        l->t = n->p;
        if (l->t != NULL) l->t->n = NULL;
    }
    d = n->d;
    _len(l, -1);
    mutex_unlock(l->m);
    free(n);
    return d;
}
 
void list_addH(list* l, long d) {
    _add(l, d, true);
}

void list_addT(list* l, long d) {
    _add(l, d, false);
}

long list_count(list* l, long d) {
    node* c = l->h;
    long i = 0;
    while (c != NULL) {
        if (c->d == d) i++;
        c = c->n;
    }
    return i;
}

bool list_del(list* l, long d) {
    node* t;
    bool found = false;
    mutex_spinlock(l->m);
    node* c = l->h;
    while (c != NULL) {
        t = c->n;
        if (c->d == d) {
            if (c->p != NULL && c->n != NULL) { // mid-list
                c->p->n = c->n;
                c->n->p = c->p;
            }
            else if (c->n == NULL && c->p != NULL) { // last element
                c->p->n = NULL;
                l->t = c->p;
            }
            else if (c->n != NULL && c->p == NULL ) { // first element
                c->n->p = NULL;
                l->h = c->n;
            }
            else if (c->n == NULL && c->p == NULL) { // list has one element
                l->h = NULL;
                l->t = NULL;
            }
            else {
                assert(false);
            }
            free(c);
            _len(l, -1);
            found = true;
        }
        c = t;
    }
    mutex_unlock(l->m);
    return found;
}

void list_del_dup(list* l) {
    mutex_spinlock(l->m);
    if (l->h == NULL) return;
    node* c = l->h;
    while (c != NULL) {
        node* t = c->n;
        while (t != NULL) {
            if (t->d == c->d) {
                node* x = t->n;
                if (t->n) t->n->p = t->p;
                if (t->p) t->p->n = t->n;
                t = x;
                _len(l, -1);
            } else {
                t = t->n;
            }
        }
        c = c->n;
    }
    mutex_unlock(l->m);
}
        

bool list_find(list* l, long d) {
    node* c = l->h;
    while (c != NULL) {
        if (c->d == d) return true;
        c = c->n;
    }
    return false;
}

bool list_join(list* l0, list* l1) {
    if (!mutex_lock2(l0->m, l1->m)) return false;
    l0->t->n = l1->h;
    l1->h->p = l0->t;
    l0->t = l1->t;
    _len(l0, list_len(l1));
    free(l1->m);
    free(l1);
    mutex_unlock(l0->m);
    return true;
}

unsigned long list_len(list* l) {
    return __atomic_load_n(&l->len, __ATOMIC_ACQUIRE);
}

list* list_new(void) {
    list* l = malloc(sizeof(list));
    if (l == NULL) assert(false);
    l->h = NULL;
    l->t = NULL;
    l->len = 0;
    l->m = mutex_create();
    return l;
}

       
long list_popH(list* l) {
    return _pop(l, true);
}

long list_popT(list* l) {
    return _pop(l, false);
}

void list_print(list* l) {
    node* c = l->h;
    printf("h: %li t: %li l: %lu\n",
            (long) l->h, (long) l->t, list_len(l));
    while (c != NULL) {
        printf("p: %li c: %li n: %li d: %li\n",
                (long) c->p, (long) c, (long) c->n, c->d);
        c = c->n;
    }
}

void list_replace(list* l, long d) {
    mutex_spinlock(l->m);
    node* c = l->h;
    while (c != NULL) {
        c->d = d;
        c = c->n;
    }
    mutex_unlock(l->m);
}

/*
    speed up options:
        - re-use the list elements (there will be the same number)
        - eliminate the use of the queue (details in the tree node impl)
*/
list* list_sort(list* l) {
    tree* t = tree_new();
    long d;
    unsigned long len = list_len(l);
    while (true) {
        if (list_len(l) == 0) break;
        d = list_popH(l);
        tree_insert(t, d);
    }
    queue* q = tree_inorder(t);
    list* ln = list_new();
    tree_node* p;
    while (true) {
        p = (tree_node*) q_dequeue(q);
        if (p == NULL) break;
        for (unsigned int i = 1; i <= p->c; i++) {
            list_addT(ln, p->d);
        }
    }
    Assert(len == list_len(ln), __func__, "lists not same length");
    free(l);
    return ln;
}

void _list_freeN(list* l) {
    mutex_spinlock(l->m);
    node* c = l->t;
    while (c != l->h) {
        c = c->p;
        free(c->n);
    }
    free(c);
    mutex_unlock(l->m);
}
