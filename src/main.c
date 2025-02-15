#include <stdio.h>
#include <unistd.h>
#include "memman/memman.h"

int main(int argc, char* argv[]) {

    void* p1 = allocate(1);
    void* p2 = allocate(2);
    void* p3 = allocate(3);
    void* p4 = allocate(4);

    print_heap();
    void* p5 = allocate(5);

    printf("\n");

    print_heap();

    return 0;
}






