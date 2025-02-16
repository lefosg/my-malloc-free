#include "memman.h"

header_t *heap_head, *heap_tail;
size_t header_size = sizeof(header_t); 
pthread_mutex_t global_alloc_lock;


void* allocate(size_t size) {

    if (size == 0)
        return NULL;

    size_t total_size;
    total_size = header_size + size;

    //check if heap already has some gap of requested size
    header_t* header;
	pthread_mutex_lock(&global_alloc_lock);
    header = first_fit_search(size);
    if (header) {
        header->is_free = 0;
        //check if needed to split the block
        split_block(header, size);
		pthread_mutex_unlock(&global_alloc_lock);
        return (void*)(header+1);
    }
    
    //else extend the heap
    void* ptr = sbrk(total_size);
    if (ptr == (void*)-1) {
		pthread_mutex_unlock(&global_alloc_lock);
        return NULL;
    }

    header = (header_t*)ptr;
    header->size=size;
    header->is_free=0;
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
	pthread_mutex_unlock(&global_alloc_lock);
    return ptr;

}


void free(void* ptr) {
    // is this pointer valid?
    // was it malloc'd?


    //if all checks ok, mark free
    header_t* tmp = get_header_of_ptr(ptr);
    tmp->is_free = 1;

    //if needed, coalesce
    coalesce_successor(tmp);

}

header_t* coalesce_successor(header_t* header) {
    /** Some thoughts: need to coalesce with prev and next neighbours? Easy to find next. Options for previous:
     * Search from the start -> slow
     * If a previous search has been done, keep pass in the prev pointer as param as well
     * Double link the list
     */
    if (header->next && header->next->is_free) {
        header->next->is_free = 0;
        header->size = header->size + header->next->size + header_size;
        header->next = header->next->next;
        header->next->next = NULL;
    }
}


void split_block(header_t* prev, size_t size) {
    //even when splitting, check if new header+some bytes fit
    //small optimization?: check if 40% of the remaining free space (aka header excluded) will be free
    long tolerance = prev->size * SPLIT_TOL;
    if ((long)(prev->size - size - header_size) <= tolerance) {
        return;
    }

    header_t* newblk;
    char* tmp = (char*)prev;  
    //now we have a pointer looking at the addr of the prev block
    //some math: tmp = tmp + header_size(aka skip the hdr) + prev.size (go right after the small block)
    tmp = tmp + header_size + size;
    newblk = (header_t*)tmp;

    newblk->next = prev->next;
    newblk->is_free = 1;
    newblk->size = prev->size - size - header_size;
    
    prev->next=newblk;
    prev->size=size;
}

header_t* first_fit_search(size_t size) {

    if (size == 0)  //could skip this check??
        return NULL;
    
    header_t *curr = heap_head;
    while (curr) {
        if (curr->size >= size && curr->is_free == 1) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


void print_heap() {
    for (header_t* header = heap_head; header != NULL; header = header->next) {
        printf("Pointer: %p\n", header);
        printf("Is Free: %u\n", header->is_free);
        printf("Size: %ld\n", header->size);
    }
}

inline header_t* get_header_of_ptr(void* ptr) {
    return (header_t*)ptr - 1;
}

void print_header_info(header_t* header) {
    printf("Pointer: %p\n", header);
    printf("Is Free: %u\n", header->is_free);
    printf("Size: %ld\n", header->size);
}