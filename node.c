#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "node.h"

// returns ownership
node* node_new(long d) {
    node* n = malloc(sizeof(node));
    if (n == NULL) assert(true);
    n->n = NULL;
    n->p = NULL;
    n->d = d;
    return n;
}
