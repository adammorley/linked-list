#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "node.h"

int main(void) {
    node* n0 = node_new(5);
    assert(sizeof(node) == sizeof(n0->p) + sizeof(n0->n) + sizeof(n0->d));
    assert(n0->d == 5);
    assert(n0->n == NULL);
    assert(n0->p == NULL);

    node* n1 = node_new(6);
    n0->n = n1;
    n1->p = n0;
    assert(n0->n == n1);
    assert(n1->p == n0);
    assert(n0->n->d == 6);
    assert(n1->p->d == 5);

    node* n2 = node_new(7);
    n1->n = n2;
    n2->p = n1;
    assert(n1->n == n2);
    assert(n2->p == n1);
    assert(n1->n->d == 7);
    assert(n2->p->d == 6);
    free(n0);
    free(n1);
    free(n2);
}
