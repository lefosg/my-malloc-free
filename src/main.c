#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <stdlib.h>
#include "memman/memman.h"


int main(int argc, char* argv[]) {

    void* p1 = allocate(1000);
    void* p2 = allocate(2000);
    void* p3 = allocate(3000);
    void* p4 = allocate(4000);
    void* p5 = allocate(5000);
    void* p6 = allocate(1000);
    void* p7 = allocate(2000);
    void* p8 = allocate(3000);
    void* p9 = allocate(4000);
    void* p10 = allocate(5000);
    void* p11 = allocate(1000);
    void* p12 = allocate(2000);
    void* p13 = allocate(3000);
    void* p14 = allocate(4000);
    void* p15 = allocate(5000);
    void* p16 = allocate(1000);
    void* p17 = allocate(2000);
    void* p18 = allocate(3000);
    void* p19 = allocate(4000);
    void* p20 = allocate(5000);

    print_heap();
    printf("\n");


    free(p5);
    free(p4);
    free(p2);
    free(p1);
    free(p3);

    void* a = allocate(5000);
    print_heap();
    printf("\n");

    void* b = allocate(5000);
    print_heap();
    printf("\n");
    // free(p20);
    // free(p19);
    // free(p17);
    // free(p16);
    // free(p18);

    // free(p15);
    // free(p14);
    // free(p12);
    // free(p11);
    // free(p13);

    // free(p10);
    // free(p9);
    // free(p7);
    // free(p6);
    // free(p8);

    // print_heap();
    // printf("\n");



    
    return 0;
}

