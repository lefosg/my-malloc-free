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
        header->size = size;  //external fragmentation/splitting occurs here
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

header_t* first_fit_search(size_t size) {

    if (size == 0)  //could skip this check??
        return NULL;
    
    header_t *curr = heap_head;
    while (curr) {
        if (curr->size >= size && curr->is_free == 1) {
            printf("\n%p\n",curr);
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

header_t* get_header_of_ptr(void* ptr) {
    return (header_t*)ptr - 1;
}

void print_header_info(header_t* header) {
    printf("Pointer: %p\n", header);
    printf("Is Free: %u\n", header->is_free);
    printf("Size: %ld\n", header->size);
}