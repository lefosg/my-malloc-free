#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "memman.h"


int main(int argc, char* argv[]) {

    void* p1 = allocate(1475137632);
    void* p2 = allocate(1071021120);
    void* p3 = allocate(1475137632);
    void* p4 = allocate(1071021120);
    void* p5 = allocate(100);
    void* p6 = allocate(100);
    void* p7 = allocate(100);
    void* p8 = allocate(100);
    void* p9 = allocate(100);
    void* p10 = allocate(100);
    void* p11 = allocate(100);
    void* p12 = allocate(100);
    void* p13 = allocate(100);
    void* p14 = allocate(100);
    void* p15 = allocate(100);
    void* p16 = allocate(100);
    void* p17 = allocate(100);
    void* p18 = allocate(100);
    void* p19 = allocate(100);
    void* p20 = allocate(100);

    print_heap();
    
    free(p20);
    void* a = allocate(200);

    print_heap();
    
    
}


