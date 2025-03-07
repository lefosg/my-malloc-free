#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "memman/memman.h"
#include <time.h>


int main(int argc, char* argv[]) {

    void* p1 = allocate(100);
    void* p2 = allocate(200);
    void* p3 = allocate(300);
    void* p4 = allocate(400);
    void* p5 = allocate(500);
    void* p6 = allocate(600);
    // void* p7 = allocate(100);
    // void* p8 = allocate(100);
    // void* p9 = allocate(100);
    // void* p10 = allocate(100);
    // void* p11 = allocate(100);
    // void* p12 = allocate(100);
    // void* p13 = allocate(100);
    // void* p14 = allocate(100);
    // void* p15 = allocate(100);
    // void* p16 = allocate(100);
    // void* p17 = allocate(100);
    // void* p18 = allocate(100);
    // void* p19 = allocate(100);
    // void* p20 = allocate(100);
    free(p6);
    free(p4);
    free(p2);
    print_free_list();

    free(p5);
    print_free_list();


    
    return 0;
}
