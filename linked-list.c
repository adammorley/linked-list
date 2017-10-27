#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "linked-list.h"

static bool _empty(list* l) {
    if (l->h == NULL && l->t == NULL) {
        return true;
    }
    return false;
}

static void _add(list* l, int d, bool head) {
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
}
      
void list_addH(list* l, int d) {
    _add(l, d, true);
}

void list_addT(list* l, int d) {
    _add(l, d, false);
}

bool list_del(list* l, int d) {
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
            found = true;
        }
        c = t;
    }
    return found;
}

bool list_find(list* l, int d) {
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
    free(l1);
    return l0;
}

void list_len(list* l) {
    node* c = l->h;
    unsigned int i = 0;
    while (c != NULL) {
        i++;
        c = c->n;
    }
    //FIXME: mutex?
    l->len = i;
}

list* list_new(void) {
    list* l = malloc(sizeof(list));
    l->h = NULL;
    l->t = NULL;
    return l;
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

void _list_freeN(list* l) {
    node* c = l->t;
    while (c != l->h) {
        c = c->p;
        free(c->n);
    }
    free(c);
}
