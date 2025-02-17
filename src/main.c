#include <stdio.h>
#include <unistd.h>
#include "memman/memman.h"


struct hdr {
    size_t size;
    int is_free;
    struct header* next;
};


int main(int argc, char* argv[]) {

    void* p1 = allocate(100);
    void* p2 = allocate(200);
    void* p3 = allocate(300);
    void* p4 = allocate(400);
    void* p5 = allocate(500);

    print_heap();
    printf("\n");

    free(p2);
    free(p4);

    print_heap();
    printf("\n");
    
    free(p3);
    print_heap();
    printf("\n");

    // free(p2);
    // print_heap();
    // printf("\n");

    // free(p1);
    // print_heap();
    // printf("\n");

    return 0;
}






