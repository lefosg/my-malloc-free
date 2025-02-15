#include <stdio.h>
#include <unistd.h>
#include "memman/memman.h"

int main(int argc, char* argv[]) {

    void* p1 = allocate(1);
    void* p2 = allocate(2);
    void* p3 = allocate(3);
    void* p4 = allocate(4);
    void* p5 = allocate(5);

    print_heap();
    printf("\n");

    void* p6 = allocate(1);
    void* p7 = allocate(2);
    void* p8 = allocate(3);
    void* p9 = allocate(4);
    void* p10 = allocate(5);

    print_heap();
    printf("\n");

    header_t* h3 = get_header_of_ptr(p3);
    h3->is_free=1;

    get_header_of_ptr(p8)->is_free=1;

    print_heap();
    printf("\n");

    void* p11 = allocate(3);
    void* p12 = allocate(3);
    void* p13 = allocate(3);

    print_heap();
    printf("\n");


    return 0;
}






