#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "memman/memman.h"
#include <stdlib.h>
#include <sys/mman.h>

int main(int argc, char* argv[]) {

    void* p1 = allocate(100);
    void* p2 = allocate(200);
    void* p3 = allocate(300);
    void* p4 = allocate(400);
    print_heap();

    void* b = allocate(129*1024);
    print_heap();
    printf("%p", b);


    
    return 0;
}
