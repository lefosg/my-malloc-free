# My malloc/free

## Description

Codebase is more or less the same as in main branch. 

However, in this branch we have an explicit free list implementation. Instead of having our heap as one huge linked list and parse all of it whenever
needed, we only parse the free blocks (there is not really a reason to parse allocated blocks). Instead of parsing #blocks, we parse #free_blocks.
Still linear, but a huge optimization over implicit lists. 

This is a FIFO explicit free list. Each time a new block is freed, it is placed at the start of the list.

- Alternatives: address ordered list -> order free blocks by their memory addresses

Still, right now first fit is used (quick, but not memory optimal at all).

- Alternatives: next-fit or best-fit algorithm (best-fit is the memory-optimal solution).

**Splitting** is done to improve the aforementioned problem (need to spend a bit of time splitting..!).

## Other strategy

Treat this problem as a data structure problem. Could implement the whole header structure as a tree, e.g.:

```C
struct free_blk_header {
    size_t size;
    struct free_blk_header *parent;
    struct free_blk_header *left;
    struct free_blk_header *right;
}
```

Trees have lower time complexity (O(logn)) in comparison to linked lists (O(n)).