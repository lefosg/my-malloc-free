#include "memman.h"

header_t *heap_head, *heap_tail;
size_t header_size = sizeof(header_t); 
pthread_mutex_t global_alloc_lock;


void* allocate(size_t size) {
    if (size == 0 || size >= __PTRDIFF_MAX__) {
        errno = ENOMEM;
        return NULL;
    }

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
    return ptr;
}

void free(void* ptr) {
    if (!ptr)
        return;
        
    pthread_mutex_lock(&global_alloc_lock);
    // is this pointer valid?
    if (ptr < (void*)heap_head || ptr > (void*)heap_tail){
        return;
    }
    // was it malloc'd?


    //if all checks ok, mark free
    header_t* tmp = get_header_of_ptr(ptr);
    tmp->is_free = 1;

    //if needed, coalesce
    coalesce_successor(tmp);

	pthread_mutex_unlock(&global_alloc_lock);
}

void coalesce_successor(header_t* header) {
    //merge with next block    
    if (header->next && header->next->is_free) {
        header->next->is_free = 0;
        header->size = header->size + header->next->size + header_size;
        if (header->next == heap_tail) {
            heap_tail = header;
            header->next=NULL;
        } else {
            header->next = header->next->next;
        }
        
    }
    //check if can coalesce with previous
    header_t* prev = search_prev_header(header);
    if (prev && prev->is_free)
        coalesce_successor(prev);
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


void print_heap(void) {
    printf("\n====== PRINT HEAP START ======\n");
    int heap_len=0;
    for (header_t* header = heap_head; header != NULL; header = header->next) {
        printf("Header address: %p\n", header);
        printf("Is Free: %u\n", header->is_free);
        printf("Size: %ld\n", header->size);
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
    printf("Is Free: %u\n", header->is_free);
    printf("Size: %ld\n", header->size);
    printf("====== PRINT HEADER INFO END ======\n");
}