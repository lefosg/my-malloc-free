#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define SPLIT_TOL 0.35
#define ALIGN_SIZE 8 

typedef struct header header_t;
typedef struct free_block_header free_blk_header_t;
typedef struct footer footer_t;


/**
 * The metadata header written right before the pointer returned by the allocate function
*/
struct header {
    size_t size;
    struct header* next;
};

/**
 * The metadata header for the free blocks
*/
struct free_block_header {
    size_t size;
    struct free_block_header* next;
    struct free_block_header* prev;
};

/**
 * Footer is placed at the end of a block when it is freed. That way, a block can know
 * where/how to find the previous block quickly 
 */
struct footer {
    header_t* header;
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
 * Searches the heap for available space using first fit algorithm
 * @param size number of bytes requested
 * @returns A header pointer to the available block if found.
 * @returns (void*)-1 if size is 0. 
 * @returns NULL if no free space was found
*/
free_blk_header_t* first_fit_search(size_t size);

/**
 * Checks if a block to be allocated is too big, and the request only needs a small
 * portion of it. If that is the case, split the big block into two.
 * @param prev pointer to the header of block to be allocated
 * @param size how much size is requested (the "small" portion)
 * @returns void - the linked list is 
*/
void split_block(free_blk_header_t* prev, size_t size);

/**
 * Called by free. It checks if the next header is an empty block. If it is,
 * it coalesces (merges) the two blocks into one
 * @param header pointer to the current header (trying to coalesce with the next)
 * @returns 1 if coalesced with successor, else 0
*/
int coalesce_successor(header_t* header);

/**
 * Does the actual merging. Is called inside coalesce_successor function
 */
void merge_blocks(header_t* header);

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
 * @note Useless in explicit list implementation
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
 * Set the last bit of the size_t size property of a header to 0, meaning the block is not free aka its allocated
 * @param header header
 */
void mark_block_allocated(header_t* header);

/**
 * Because the last bit of the size_t size property in the header has its last bit set to signal
 * whether the block is free or not, we mask it out.
 * We need to calculate the real size (due to 16-byte alignment we just cut off the last bit)
 * @paragraph size of the block (with last bit set)
 * @returns acutal size (same as size but sets last bit to zero)
 */
size_t get_block_size(header_t* size);

/**
 * Returns 1 if the block is free, else returns 0;
 */
int block_is_free(header_t* header);

/**
 * Returns 1 if the previous block of a given header is free, else returns 0;
 */
int prev_block_is_free(header_t* header);

/**
 * Used to set the 'prev_free' bit to 1. When a block is freed, this function
 * is called to update the next header, that right now, its previous header was freed.
 */
void set_prev_allocation_status_free(header_t* header);

/**
 * Used to set the 'prev_free' bit to 0. When a block is freed, this function
 * is called to update the next header, that right now, its previous header was freed.
 */
void set_prev_allocation_status_allocated(header_t* header);

/**
 * When a block is freed OR split, we need to place a footer at the end.
 * @param header of the block, to which we want to place a footer
 */
void place_footer(header_t* header);

/**
 * Gets the footer of the given header. Should only be called when coalescing
 * @param header of the block
 * @returns the footer
 */
footer_t* get_footer(header_t* header);

/**
 * Print out the free list headers
 */
void print_free_list();