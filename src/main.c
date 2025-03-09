#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "memman.h"


int main(int argc, char* argv[]) {

    void* p1 = allocate(400);
    void* p2 = allocate(100);
    void* p3 = allocate(500);
    void* p4 = allocate(300);
    void* p5 = allocate(200);
    void* p6 = allocate(50);
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
    free(p4);
    free(p6);

    //400 100 500 300 200 50 2000
    
    print_heap();
    printf("\n");
    
    void* a = allocate(40);
    void* b = allocate(250);

    print_heap();
    printf("\n");


    return 0;
}

