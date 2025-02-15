#include "memman.h"

header_t *heap_head, *heap_tail;
size_t header_size = sizeof(header_t); 


void* allocate(size_t size) {

    if (size == 0)
        return NULL;

    size_t total_size;
    total_size = header_size + size;

    //check if heap already has some gap of requested size
    header_t* header;

    header = first_fit_search(size);
    if (header) {
        header->is_free = 0;
        //check if needed to split the block
        split_block(header, size);
        return (void*)(header+1);
    }
    
    //else extend the heap
    void* ptr = sbrk(total_size);
    if (ptr == (void*)-1)
        return NULL;

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

    return ptr;

}

void split_block(header_t* prev, size_t size) {
    //even when splitting, check if new header+some bytes fit
    //small optimization?: check if 40% of the remaining free space (aka header excluded) will be free
    printf("%d\n",prev->size);
    printf("%d\n",size);
    printf("%d\n",header_size);
    if ((long)(prev->size - size - header_size) <= 0) {
        printf("aborting split block\n");
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

inline void print_header_info(header_t* header) {
    printf("Pointer: %p\n", header);
    printf("Is Free: %u\n", header->is_free);
    printf("Size: %ld\n", header->size);
}