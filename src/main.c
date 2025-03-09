#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "memman.h"
#include <time.h>


int main(int argc, char* argv[]) {
    
    void* p1 = allocate(100);
	void* p2 = allocate(200);
	void* p3 = allocate(300);
	void* p4 = allocate(400);
	void* p5 = allocate(500);
	void* p6 = allocate(600);
	void* p7 = allocate(700);
	
    
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
    free(p6);
    free(p7);



    print_heap();


    return 0;
}
