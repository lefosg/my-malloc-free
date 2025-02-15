#include <unistd.h>  // for using sbrk, size_t, NULL
#include <stdio.h>  //for some prints

typedef struct header header_t;

/**
 * The metadata header written right before the pointer returned by the allocate function
*/
struct header {
    size_t size;
    int is_free;
    struct header* next;
};



// ================= API =================


/**
 * Allocates a number of bytes in a heap and returns a void pointer to that address
 * @param size bytes of memory in the heap
 * @returns a void pointer to the address
*/
void* allocate(size_t size);


/**
 * Frees up the memory from the heap by specifing the pointer to that address
 * @param ptr the pointer to the heap
 * @returns nothing (void)
*/
void free(void* ptr);


// ================= HELPER FUNCTIONS =================

/**
 * Searches the heap for available space using first fit algorithm
 * @param size number of bytes requested
 * @returns A header pointer to the available block if found.
 * @returns (void*)-1 if size is 0. 
 * @returns NULL if no free space was found
*/
header_t* first_fit_search(size_t size);

/**
 * Checks if a block to be allocated is too big, and the request only needs a small
 * portion of it. If that is the case, split the big block into two.
 * @param prev pointer to the header of block to be allocated
 * @param size how much size is requested (the "small" portion)
 * @returns void - the linked list is 
*/
void split_block(header_t* prev, size_t size);

/**
 * Takes the pointer in question, and returns a pointer to its correspondent header
 * @param ptr to heap
 * @returns header of this ptr
*/
header_t* get_header_of_ptr(void* ptr);

/**
 * Prints the current heap (pointer address, size of block, and availability aka free or not)
*/
void print_heap();

/**
 * Prints the info of a given header (pointer address, size of block, and availability aka free or not)
*/
void print_header_info(header_t* header);

