#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <stdlib.h>
#include "memman/memman.h"


int main(int argc, char* argv[]) {

    int k = 16;
    int is_free = 1<<0;
    int prev_free = 1 << 1;

    printf("%d", k | is_free | prev_free);

    
    return 0;
}
