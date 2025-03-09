#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "memman.h"
#include <time.h>


int main(int argc, char* argv[]) {
    
    void* p = allocate(100);
    void* q = allocate(200);
    void* r = allocate(300);

	free(q);

	void* x = allocate(40);

    print_heap();


    return 0;
}
