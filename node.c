#include <stdlib.h>
#include "node.h"

// returns ownership
node* node_new(int d) {
    node* n = malloc(sizeof(node));
    n->n = NULL;
    n->p = NULL;
    n->d = d;
    return n;
}
