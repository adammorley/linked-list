#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
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
}

static bool _empty(list* l) {
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
    if (l->h == NULL || l->t == NULL) assert(false);
    if (head) {
        n = l->h;
        l->h = n->n;
        l->h->p = NULL;
    }
    else {
        n = l->t;
        l->t = n->p;
        l->t->n = NULL;
    }
    d = n->d;
    _len(l, -1);
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
    node* c = l->h;
    node* t;
    bool found = false;
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
    return found;
}

bool list_find(list* l, long d) {
    node* c = l->h;
    while (c != NULL) {
        if (c->d == d) return true;
        c = c->n;
    }
    return false;
}

list* list_join(list* l0, list* l1) {
    l0->t->n = l1->h;
    l1->h->p = l0->t;
    l0->t = l1->t;
    _len(l0, list_len(l1));
    free(l1);
    return l0;
}

unsigned long list_len(list* l) {
    return __atomic_load_n(&l->len, __ATOMIC_ACQUIRE);
}

list* list_new(void) {
    list* l = malloc(sizeof(list));
    l->h = NULL;
    l->t = NULL;
    l->len = 0;
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
    printf("h: %d t: %d\n",
            l->h, l->t);
    while (c != NULL) {
        printf("p: %d c: %d n: %d d: %d\n",
                c->p, c,    c->n, c->d);
        c = c->n;
    }
}

void list_replace(list* l, long d) {
    node* c = l->h;
    while (c != NULL) {
        c->d = d;
        c = c->n;
    }
}

void _list_freeN(list* l) {
    node* c = l->t;
    while (c != l->h) {
        c = c->p;
        free(c->n);
    }
    free(c);
}
