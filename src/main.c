#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "memman/memman.h"
#include <time.h>


int main(int argc, char* argv[]) {

    srand(time(NULL));

    void* p1 = allocate(rand());
    void* p2 = allocate(rand());
    void* p3 = allocate(rand());
    void* p4 = allocate(rand());
    void* p5 = allocate(rand());
    void* p6 = allocate(rand());
    void* p7 = allocate(rand());
    void* p8 = allocate(rand());
    void* p9 = allocate(rand());
    void* p10 = allocate(rand());
    void* p11 = allocate(rand());
    void* p12 = allocate(rand());
    void* p13 = allocate(rand());
    void* p14 = allocate(rand());
    void* p15 = allocate(rand());
    void* p16 = allocate(rand());
    void* p17 = allocate(rand());
    void* p18 = allocate(rand());
    void* p19 = allocate(rand());
    void* p20 = allocate(rand());

    print_heap();

    free(p1);
    free(p2);
    
    print_heap();

    return 0;
}
