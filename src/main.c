#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <stdlib.h>
#include "memman/memman.h"


int main(int argc, char* argv[]) {

    int* p1 = allocate(4);
    int* p2 = allocate(4);
    int* p3 = allocate(4);


    print_heap();

    *p1 = 4;
    *p2 = 98;
    *p3 = 56;

    free(p2);

    print_heap();

    free(p3);

    print_heap();

    free(p1);

    print_heap();

    return 0;
}
