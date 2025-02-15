# My malloc/free

Description:

Implementation of my own allocate and free functions for C. Uses ``sbrk`` function. Current implementation uses implicit list and first fit algorithm.

The main.c file is just for testing the API. The library itself is in the src/memman folder (short for memory manager).

**API:**
- allocate(size_t size): allocates <i>size</i> bytes in the heap
- free(void* ptr): frees up the memory from a specified pointer

