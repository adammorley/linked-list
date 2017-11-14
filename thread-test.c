#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "linked-list.h"

struct t_args {
    list* l;
};

void* populate_data(void* args) {
    struct t_args* arg = (struct t_args*) args;
    list* l = arg->l;
    for (long i = 0; i <= 30; i++) list_addH(l, i);
}

void* find_data(void* args) {
    struct t_args* arg = (struct t_args*) args;
    list* l = arg->l;
    for (long i = 30; i >= 0; i--) assert(list_find(l, i));
}

void* delete_data(void* args) {
    struct t_args* arg = (struct t_args*) args;
    list* l = arg->l;
    usleep(50);
    list_del(l, 17);
}

void* test_delete(void* args) {
    struct t_args* arg = (struct t_args*) args;
    list* l = arg->l;
    int count = 0;
    bool found = list_find(l, 17);
    assert(found);
    while(true) {
        found = list_find(l, 17);
        usleep(10);
        if (count++ > 100 || !found) break;
    }
    assert(!found);
}

int main(void) {
    printf("running thread test\n");
    struct t_args args;
    args.l = list_new();

    /*
        put some data in and make sure it's there
    */
    pthread_t tid0;
    pthread_t tid1;
    pthread_create(&tid0, NULL, populate_data, (void*) &args);
    usleep(50);
    pthread_create(&tid1, NULL, find_data, (void*) &args);
    pthread_join(tid0, NULL);
    pthread_join(tid1, NULL);

    /*
        run a background thread looking for the data item
        and delete it, make sure it deletes
    */
    pthread_t tid2;
    pthread_t tid3;
    pthread_create(&tid2, NULL, test_delete, (void*) &args);
    pthread_create(&tid3, NULL, delete_data, (void*) &args);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    return 0;
}
