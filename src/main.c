#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <stdlib.h>
#include "memman/memman.h"


int main(int argc, char* argv[]) {

    void* p1 = allocate(400);
    void* lol3= allocate(1);
    void* p2 = allocate(100);
    void* lol = allocate(1);
    void* p3 = allocate(500);
    void* lol2 = allocate(1);
    void* p4 = allocate(100);
    void* p5 = allocate(200);
    void* p6 = allocate(100);
    void* p7 = allocate(2000);
    // void* p8 = allocate(3000);
    // void* p9 = allocate(4000);
    // void* p10 = allocate(5000);
    // void* p11 = allocate(1000);
    // void* p12 = allocate(2000);
    // void* p13 = allocate(3000);
    // void* p14 = allocate(4000);
    // void* p15 = allocate(5000);
    // void* p16 = allocate(1000);
    // void* p17 = allocate(2000);
    // void* p18 = allocate(3000);
    // void* p19 = allocate(4000);
    // void* p20 = allocate(5000);

    print_heap();
    printf("\n");

    
    free(p2);
    free(p3);
    free(p4);
    // free(p5);

    //400 1 100 1 500 1 100 200 100 2000

    print_heap();
    printf("\n");
    void* a = allocate(500);
    print_heap();
    printf("\n");
    void* b = allocate(100);
    print_heap();
    printf("\n");
    free(p1);
    void* c = allocate(200);
    print_heap();
    printf("\n");
    void* d = allocate(100);
    print_heap();
    printf("\n");
    void* e = allocate(10000);
    print_heap();
    printf("\n");
    free(e);
    allocate(4000);
    print_heap();
    printf("\n");
    

    
    return 0;
}

