#include "memman.h"

header_t *heap_head, *heap_tail;

size_t header_size = sizeof(header_t); 
size_t footer_size = sizeof(footer_t);
size_t mmap_hdr_size = sizeof(mmap_header_t);

pthread_mutex_t global_alloc_lock;


void* allocate(size_t size) {
    if (size == 0 || size >= __PTRDIFF_MAX__) {
        errno = ENOMEM;
        return NULL;
    }
    // allocate with mmap if size is  big enough
    //check if heap already has some gap of requested size
    header_t* header;
    if (size < ALIGN_SIZE)
        size = ALIGN_SIZE;
    else   
        size = ALIGN_SIZE * (int)((size + (ALIGN_SIZE-1)) / ALIGN_SIZE);

    if (size >= MMAP_THRESHOLD) {
        void* ptr = allocate_with_mmap(size);  //internally does mutex lock/unlock
        return ptr;
    }
    
	pthread_mutex_lock(&global_alloc_lock);
    header = first_fit_search(size);
    if (header) {
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

void* allocate_with_mmap(size_t size) {
    //mmap takes care of alignment
    void* pointer = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (pointer == (void *) -1)
        return NULL;
    // if mmap successful, store a mmap_header in the heap
    pthread_mutex_lock(&global_alloc_lock);
    mmap_header_t* mmap_header = (mmap_header_t*)first_fit_search(mmap_hdr_size);
    if (mmap_header) {  // alocate space if there is enough in heap
        mmap_header->ptr_size = size;
        mark_block_allocated((header_t*)mmap_header);
        mmap_header->mmap_ptr = pointer;
        //check if needed to split the block
        split_block((header_t*)mmap_header, mmap_hdr_size);
        mark_block_mmaped(mmap_header);
        pthread_mutex_unlock(&global_alloc_lock);
        return pointer;
    } else {  // extend heap
        mmap_header_t* newmmap_h = (mmap_header_t*)get_header_of_ptr(extend_heap(mmap_hdr_size));
        mark_block_mmaped(newmmap_h);
        newmmap_h->ptr_size = size;
        newmmap_h->mmap_ptr = pointer;
        print_mmap_header_info(newmmap_h);
        pthread_mutex_unlock(&global_alloc_lock);
        return pointer;
    }
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
        if (block_is_free(heap_tail))
            set_prev_allocation_status_free(heap_tail);
    }
    //new tail is this header after extending
    heap_tail = header;
    return ptr;
}

void free(void* ptr) {
    if (!ptr)
        return;
        
    pthread_mutex_lock(&global_alloc_lock);
    // is this pointer valid? was it malloc'd? was it mmaped? consider it was mmaped
    header_t* tmp;
    if ( ptr > (void*)(heap_tail+1)){  //ptr < (void*)heap_head || condition removed
        mmap_header_t* mmap_h = free_with_munmap(ptr); //checks if the ptr is stored in one of the metadata mmap_header headers
        tmp = (header_t*)mmap_h;
        mark_block_unmmaped(mmap_h);
    } else {
        tmp = get_header_of_ptr(ptr);   
    }


    //if all checks ok, mark free
    print_header_info(tmp);
    print_mmap_header_info((mmap_header_t*)tmp);
    //if needed, coalesce
    mark_block_free(tmp);
    coalesce_successor(tmp);
    // mark_block_free(tmp);
    place_footer(tmp);
    set_prev_allocation_status_free(tmp);
	pthread_mutex_unlock(&global_alloc_lock);
}

mmap_header_t* free_with_munmap(void* ptr) {
    // find the header of the mmap'd pointer, get the size, and munmap
    for (header_t* curr = heap_head; curr != NULL; curr = curr->next) {
        if (!header_is_mmaped((mmap_header_t*)curr)) continue; //if not mmaped, go to next
        mmap_header_t* mmap_h = (mmap_header_t*) curr;
        if (mmap_h->mmap_ptr == ptr) {
                size_t size = mmap_h->ptr_size;
                munmap(ptr, size);
                return mmap_h;
        }
    }
    return NULL;
}

void coalesce_successor(header_t* header) {
    //merge with next block    
    if (header->next && block_is_free(header->next)) { //&& header->next->is_free
        // header->next->is_free = 0;
        mark_block_allocated(header->next);
        int prev_free = prev_block_is_free(header);
        header->size = get_block_size(header) + get_block_size(header->next) + header_size; //header->size + header->next->size
        mark_block_free(header);
        if (prev_free)
            header->size = header->size | 2;
            
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
        printf("\t Block: %d\n", heap_len+1);
        printf("Header address: %p\n", header);
        printf("Is Free: %u\n", block_is_free(header));
        printf("Is Prev Free Bit: %u\n", prev_block_is_free(header));
        printf("Is mmap'd: %u\n", header_is_mmaped((mmap_header_t*)header));
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

int header_is_mmaped(mmap_header_t* mmap_h) {
    return (mmap_h->size & 4) >> 2;
}

inline void mark_block_mmaped(mmap_header_t* mmap_h) {
    mmap_h->size |= 4;
}

inline void mark_block_unmmaped(mmap_header_t* mmap_h) {
    mmap_h->size &= ~4;
}

void print_mmap_header_info(mmap_header_t* mmap_h) {
    printf("\n====== PRINT MMAP HEADER INFO START ======\n");
    printf("Header: %p\n", mmap_h);
    printf("Pointer: %p\n", mmap_h->mmap_ptr);
    printf("Size: %ld\n", get_block_size((header_t*)mmap_h));
    printf("Mmap'd pointer: %u\n", header_is_mmaped(mmap_h));
    printf("Pointer size: %u\n", mmap_h->size);
    printf("Next: %p\n", mmap_h->next);
    printf("====== PRINT MMAP HEADER INFO END ======\n");
}
