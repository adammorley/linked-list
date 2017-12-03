#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../mutex/mutex.h"
#include "../node/node.h"
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
        l->h->l = n;
        n->r = l->h;
        l->h= n;
    }
    else {
        l->t->r = n;
        n->l = l->t;
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
        l->h = n->r;
        l->h->l = NULL;
    }
    else {
        n = l->t;
        l->t = n->l;
        l->t->r = NULL;
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
        c = c->r;
    }
    return i;
}

bool list_del(list* l, long d) {
    node* t;
    bool found = false;
    mutex_spinlock(l->m);
    node* c = l->h;
    while (c != NULL) {
        t = c->r;
        if (c->d == d) {
            if (c->l != NULL && c->r != NULL) { // mid-list
                c->l->r = c->r;
                c->r->l = c->l;
            }
            else if (c->r == NULL && c->l != NULL) { // last element
                c->l->r = NULL;
                l->t = c->l;
            }
            else if (c->r != NULL && c->l == NULL ) { // first element
                c->r->l = NULL;
                l->h = c->r;
            }
            else if (c->r == NULL && c->l == NULL) { // list has one element
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

bool list_find(list* l, long d) {
    node* c = l->h;
    while (c != NULL) {
        if (c->d == d) return true;
        c = c->r;
    }
    return false;
}

bool list_join(list* l0, list* l1) {
    if (!mutex_lock2(l0->m, l1->m)) return false;
    l0->t->r = l1->h;
    l1->h->l = l0->t;
    l0->t = l1->t;
    _len(l0, list_len(l1));
    free(l1);
    mutex_unlock(l0->m);
    return true;
}

unsigned long list_len(list* l) {
    return __atomic_load_n(&l->len, __ATOMIC_ACQUIRE);
}

list* list_new(void) {
    list* l = malloc(sizeof(list));
    if (l == NULL) assert(true);
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
    printf("h: %li t: %li\n",
            (long) l->h, (long) l->t);
    while (c != NULL) {
        printf("p: %li c: %li n: %li d: %li\n",
                (long) c->l, (long) c, (long) c->r, c->d);
        c = c->r;
    }
}

void list_replace(list* l, long d) {
    mutex_spinlock(l->m);
    node* c = l->h;
    while (c != NULL) {
        c->d = d;
        c = c->r;
    }
    mutex_unlock(l->m);
}

void _list_freeN(list* l) {
    mutex_spinlock(l->m);
    node* c = l->t;
    while (c != l->h) {
        c = c->l;
        free(c->r);
    }
    free(c);
    mutex_unlock(l->m);
}
