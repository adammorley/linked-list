#ifndef NODE_H
#define NODE_H

typedef struct node node;
struct node {
    node* p;
    node* n;
    long d;
};

/*
    creates and returns a node, returns ownership
*/
node* node_new(long d);

#endif //NODE_H
