#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <stdlib.h>
#include "memman/memman.h"

#define flipBits(n) ((n)^((1u<<(1))-1))


void test(void);





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


    // printf("%p\n", p1);
    // printf("%p\n", p2);
    // printf("%p\n", p3);
    // printf("%p\n", p4);
    // printf("%p\n", p5);
    // printf("%p\n", p6);
    // printf("%p\n", p7);
    // printf("%p\n", p8);
    // printf("%p\n", p9);
    // printf("%p\n", p10);

    

    free(p5);
    print_heap();
    printf("\n");
    free(p4);
    print_heap();
    printf("\n");
    free(p2);
    print_heap();
    printf("\n");
    free(p1);
    print_heap();
    printf("\n");
    free(p3);
    print_heap();
    printf("\n");


    // void* a = allocate(5000);

    // print_heap();
    // printf("\n");
    free(p20);
    free(p19);
    free(p17);
    free(p16);
    free(p18);

    free(p15);
    free(p14);
    free(p12);
    free(p11);
    free(p13);

    free(p10);
    free(p9);
    free(p7);
    free(p6);
    free(p8);

    print_heap();
    printf("\n");



    
    return 0;
}
















void test(void) {
        // void* p1 = allocate(243);
    // void* p2 = allocate(6343);
    // void* p3 = allocate(15);
    // void* p4 = allocate(947);
    // void* p5 = allocate(987);
    // void* p6 = allocate(54);
    // void* p7 = allocate(3);
    // void* p8 = allocate(218);
    // void* p9 = allocate(876);
    // void* p10 = allocate(6059);
    // void* p11 = allocate(7070);
    // void* p12 = allocate(69804);
    // void* p13 = allocate(2854);
    // void* p14 = allocate(387);
    // void* p15 = allocate(9850);
    // void* p16 = allocate(385);
    // void* p17 = allocate(2000);
    // void* p18 = allocate(3000);
    // void* p19 = allocate(4000);
    // void* p20 = allocate(5000);

    // void* p1 = allocate(1000);
    // void* p2 = allocate(2000);
    // void* p3 = allocate(3000);
    // void* p4 = allocate(4000);
    // void* p5 = allocate(5000);
    // void* p6 = allocate(1000);
    // void* p7 = allocate(2000);
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

    // print_heap();
    // printf("\n");

    // free(p5);
    // free(p4);
    // free(p2);
    // free(p1);
    // free(p3);

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
}