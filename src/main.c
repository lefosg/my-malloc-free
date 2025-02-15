#include <stdio.h>
#include <unistd.h>
#include "memman/memman.h"

struct hdr {
    size_t size;
    int is_free;
    struct header* next;
};


int main(int argc, char* argv[]) {

    void* p1 = allocate(25);
    void* p2 = allocate(20);
    void* p3 = allocate(30);
    void* p4 = allocate(40);
    void* p5 = allocate(50);

    print_heap();
    printf("\n");

    // header_t* h3 = get_header_of_ptr(p3);
    // char* m = (char*)h3;
    // printf("%p\n",&h3->size);
    // printf("%p\n",m);
    // printf("%d\n",m==&h3->size);

    // m = m + h3->size + sizeof(struct hdr);
    // printf("%p\n",m==get_header_of_ptr(p4));

    get_header_of_ptr(p5)->is_free=1;
    void* t4 = allocate(26);

    print_heap();
    printf("\n");


    return 0;
}






