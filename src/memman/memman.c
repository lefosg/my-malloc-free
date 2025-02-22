#include "memman.h"

header_t *heap_head, *heap_tail;
size_t header_size = sizeof(header_t); 
size_t footer_size = sizeof(footer_t);
pthread_mutex_t global_alloc_lock;


void* allocate(size_t size) {
    if (size == 0 || size >= __PTRDIFF_MAX__) {
        errno = ENOMEM;
        return NULL;
    }

    //check if heap already has some gap of requested size
    header_t* header;
    if (size < ALIGN_SIZE)
        size = ALIGN_SIZE;
    else   
        size = ALIGN_SIZE * (int)((size + (ALIGN_SIZE-1)) / ALIGN_SIZE);
	pthread_mutex_lock(&global_alloc_lock);
    header = first_fit_search(size);
    if (header) {
        // header->is_free = 0;
        mark_block_allocated(header);
        //check if needed to split the block
        split_block(header, size);
		pthread_mutex_unlock(&global_alloc_lock);
        return (void*)(header+1);
    }
    
    //else extend the heap
    void* ptr = extend_heap(size);
	pthread_mutex_unlock(&global_alloc_lock);
    return ptr;
}

void* extend_heap(size_t size) {
    void* ptr = sbrk(size+header_size);
    if (ptr == (void*)-1) {
        errno = ENOMEM;
		pthread_mutex_unlock(&global_alloc_lock);
        return NULL;
    }

    header_t* header = (header_t*)ptr;
    header->size=size;
    // header->is_free=0;
    mark_block_allocated(header);
    header->next=NULL;

    ptr += header_size;

    if (heap_head == NULL) {
        heap_head = header;
    }
    //when extending, old tail should point to current new header
    if (heap_tail) {
        heap_tail->next = header;
    }
    //new tail is this header after extending
    heap_tail = header;
    return ptr;
}

void free(void* ptr) {
    if (!ptr)
        return;
        
    pthread_mutex_lock(&global_alloc_lock);
    // is this pointer valid? was it malloc'd?  
    if (ptr < (void*)heap_head || ptr > (void*)(heap_tail+1)){
        return;
    }

    //if all checks ok, mark free
    header_t* tmp = get_header_of_ptr(ptr);
    // tmp->is_free = 1;
    
    //if needed, coalesce
    
    mark_block_free(tmp);


    coalesce_successor(tmp);
    // mark_block_free(tmp);
    place_footer(tmp);
    set_prev_allocation_status_free(tmp);
	pthread_mutex_unlock(&global_alloc_lock);
}

void coalesce_successor(header_t* header) {
    //merge with next block    
    if (header->next && block_is_free(header->next)) { //&& header->next->is_free
        // header->next->is_free = 0;
        mark_block_allocated(header->next);
        header->size = get_block_size(header) + get_block_size(header->next) + header_size; //header->size + header->next->size
        mark_block_free(header);
        if (header->next == heap_tail) {
            heap_tail = header;
            header->next=NULL;
        } else {
            header->next = header->next->next;
        }
    }
    //check if can coalesce with previous
    // header_t* prev = search_prev_header(header);
    // if (prev && block_is_free(prev))  // && prev->is_free
    //     coalesce_successor(prev);
    // check the 'prev_free' bit in the size var. if free, get prev footer, and coalesce (MUCH faster)
    if (prev_block_is_free(header)) {
        header_t* prev = (((footer_t*)header)-1)->header;
        coalesce_successor(prev);
    }    
}

header_t* search_prev_header(header_t* header) {
    if (header == heap_head || header == NULL)
        return NULL;
    for (header_t* curr = heap_head; curr != NULL; curr = curr->next) {
        if (curr->next == header)
            return curr;
    }
    return NULL;
}

void split_block(header_t* prev, size_t size) {
    //even when splitting, check if new header+some bytes fit
    //small optimization?: check if 40% of the remaining free space (aka header excluded) will be free
    long tolerance = get_block_size(prev) * SPLIT_TOL;  //prev->size * SPLIT_TOL
    if ((long)(get_block_size(prev) - size - header_size) <= tolerance) { //prev->size - ...
        set_prev_allocation_status_allocated(prev);
        return;
    }

    header_t* newblk;
    char* tmp = (char*)prev;  
    //now we have a pointer looking at the addr of the prev block
    //some math: tmp = tmp + header_size(aka skip the hdr) + prev.size (go right after the small block)
    tmp = tmp + header_size + size;
    newblk = (header_t*)tmp;


    newblk->next = prev->next;
    // newblk->is_free = 1;
    newblk->size = get_block_size(prev) - size - header_size; //prev->size
    mark_block_free(newblk);
    
    prev->next=newblk;
    prev->size=size;

    set_prev_allocation_status_free(newblk);
    place_footer(newblk);
}

header_t* first_fit_search(size_t size) {

    if (size == 0)  //could skip this check??
        return NULL;
    
    header_t *curr = heap_head;
    while (curr) {
        if (get_block_size(curr) >= size && block_is_free(curr)) {  //curr->size >= size && curr->is_free == 1
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


void print_heap(void) {
    printf("\n====== PRINT HEAP START ======\n");
    int heap_len=0;
    for (header_t* header = heap_head; header != NULL; header = header->next) {
        printf("Header address: %p\n", header);
        printf("Is Free: %u\n", block_is_free(header));
        printf("Is Prev Free Bit: %u\n", prev_block_is_free(header));
        printf("Size: %ld\n", get_block_size(header));
        heap_len++;
    }
    printf("No. headers: %d\n", heap_len);
    printf("====== PRINT HEAP END ======\n");
}

inline header_t* get_header_of_ptr(void* ptr) {
    return (header_t*)ptr - 1;
}

void print_header_info(header_t* header) {
    printf("\n====== PRINT HEADER INFO START ======\n");
    printf("Pointer: %p\n", header);
    printf("Is Free Bit: %u\n", block_is_free(header));
    printf("Is Prev Free Bit: %u\n", prev_block_is_free(header));
    printf("Size: %ld\n", get_block_size(header));
    printf("====== PRINT HEADER INFO END ======\n");
}

inline size_t get_block_size(header_t* header) {
    // return header->size & ~((1<<1)-1);
    return header->size & ~15;  
}

inline void mark_block_free(header_t* header) {
    header->size |= 1;
}

inline void mark_block_allocated(header_t* header) {
    header->size &= ~1;
}

inline int block_is_free(header_t* header) {
    return header->size & 1;  // Returns 1 if last bit is 1, 0 if last bit is 0
}

inline void set_prev_allocation_status_free(header_t* header) {
    if (header->next) {
        header->next->size = header->next->size | 2;
    }
}

inline void set_prev_allocation_status_allocated(header_t* header) {
    if (header->next)
        header->next->size = header->next->size & ~2;
}


inline int prev_block_is_free(header_t* header) {
    size_t x = (header->size & 2);
    return x>>1;  // Returns 1 if last bit is 1, 0 if last bit is 0
}

void place_footer(header_t* header) {
    char* tmp = (char*)(header) + get_block_size(header) + header_size - footer_size;
    footer_t* footer = (footer_t*)tmp;
    footer->header = header;
}

footer_t* get_footer(header_t* header) {
    return ((footer_t*)(header + get_block_size(header) + header_size - footer_size));
}