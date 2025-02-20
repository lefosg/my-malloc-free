#include <unistd.h>  // for using sbrk, size_t, NULL
#include <stdio.h>  //for some prints
#include <pthread.h>
#include <errno.h>

#define SPLIT_TOL 0.4
#define ALIGN_SIZE 16

typedef struct header header_t;

/**
 * The metadata header written right before the pointer returned by the allocate function
*/
struct header {
    size_t size;
    // int is_free;
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
 * Extends the heap by size bytes requested, plus the header size.
 * @param size requested
 * @returns a void pointer to an address on the heap 
 */
void* extend_heap(size_t size);

/**
 * Searches the heap for available space using best fit algorithm.
 * Finds the smallest possible block that can contain the size requested
 * @param size number of bytes requested
 * @returns A header pointer to the available block if found.
 * @returns (void*)-1 if size is 0. 
 * @returns NULL if no free space was found
*/
header_t* best_fit_search(size_t size);

/**
 * Checks if a block to be allocated is too big, and the request only needs a small
 * portion of it. If that is the case, split the big block into two.
 * @param prev pointer to the header of block to be allocated
 * @param size how much size is requested (the "small" portion)
 * @returns void - the linked list is 
*/
void split_block(header_t* prev, size_t size);

/**
 * Called by free. It checks if the next header is an empty block. If it is,
 * it coalesces (merges) the two blocks into one
 * @param header pointer to the current header (trying to coalesce with the next)
 * @returns a pointer to the new header. If coalesced, header should be different. If not coalesced,
 * it returns the initial header.
 * 
 * Some thoughts: need to coalesce with prev and next neighbours? Easy to find next. Options for previous:
 * Search from the start -> slow (Currently implemented!)
 * If a previous search has been done, keep pass in the prev pointer as param as well
 * Double link the list
*/
void coalesce_successor(header_t* header);

/**
 * Returns the previous block header. Serial search from head to given one.
 * @param header current header
 * @returns previous header. If given header is NULL or the heap_head, returns NULL.
 */
header_t* search_prev_header(header_t* header);

/**
 * Takes the pointer in question, and returns a pointer to its correspondent header
 * @param ptr to heap
 * @returns header of this ptr
*/
header_t* get_header_of_ptr(void* ptr);

/**
 * Prints the current heap (pointer address, size of block, and availability aka free or not)
*/
void print_heap(void);

/**
 * Prints the info of a given header (pointer address, size of block, and availability aka free or not)
*/
void print_header_info(header_t* header);

/**
 * Set the last bit of the size_t size property of a header to 1, meaning the block is free
 * @param header header
 */
void mark_block_free(header_t* header);

/**
 * Set the last bit of the size_t size property of a header to 0, meaning the block is not free aka occupied
 * @param header header
 */
void mark_block_occupied(header_t* header);

/**
 * Because the last bit of the size_t size property in the header has its last bit set to signal
 * whether the block is free or not, we mask it out.
 * We need to calculate the real size (due to 16-byte alignment we just cut off the last bit)
 * @paragraph size of the block (with last bit set)
 * @returns acutal size (same as size but sets last bit to zero)
 */
size_t get_block_size(header_t* size);

/**
 * Checks if the block is free by looking at the last bit of the header.size field
 * @param header to check if free
 * @returns 1 if the block is free, else returns 0;
 */
int block_is_free(header_t* header);