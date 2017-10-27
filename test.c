#include <stdlib.h>

typedef struct f f;
struct f { int* z; };
void test(f* ff) {
    int* a = ff->z;
    free(a);
}

int main(void) {
    int *p = malloc(sizeof(int));
    f* ff = malloc(sizeof(f));
    ff->z = p;
    test(ff);
    free(ff);
    return 0;
}
