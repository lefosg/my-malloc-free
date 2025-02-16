#include <stdio.h>
#include <unistd.h>
#include "memman/memman.h"


struct hdr {
    size_t size;
    int is_free;
    struct header* next;
};


int main(int argc, char* argv[]) {

    void* p1 = allocate(500);
    void* p2 = allocate(999999);
    void* p3 = allocate(4650);
    void* p4 = allocate(40);
    void* p5 = allocate(6557);

    // print_heap();
    printf("\n");

    free(p4);
    free(p3);
    free(p2);
    free(p1);

    // get_header_of_ptr(p5)->is_free=1;
    // void* t4 = allocate(400);

    print_heap();
    printf("\n");


    return 0;
}






