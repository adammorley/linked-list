#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../node/node.h"
#include "linked-list.h"

#define ns_to_s pow(10, -9)

void t(struct timespec* t) {
    if (clock_gettime(CLOCK_MONOTONIC, t)) {
        printf("error with gettime\n");
        assert(false);
    }
}

double sec(struct timespec* t) {
    return ((double) t->tv_sec + (double) t->tv_nsec * ns_to_s);
}

void test(int c) {
    list* l = list_new();
    struct timespec* b = malloc(sizeof(struct timespec));
    struct timespec* a = malloc(sizeof(struct timespec));
    printf("node count: %d\n", c);
    t(b);
    for (int i = 0; i <= c; i++) {
        list_addH(l, 2);
    }
    t(a);
    printf("new nodes: %e per sec\n", ((double) c / (sec(a) - sec(b))));

    t(b);
        list_replace(l, 3);
    t(a);
    printf("update nodes: %e per sec\n", ((double) c / (sec(a) - sec(b))));
    _list_freeN(l);
    free(l);
}

int nodeC(double c) {
    return (int) (c / sizeof(node));
}
    
int main(void) {
    test(nodeC(pow(2, 10)));
    test(nodeC(pow(2, 20)));
    test(nodeC(pow(2, 30)));
    return 0;
}
