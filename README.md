# My malloc/free

Description:

Codebase is the exact same as with main branch. Implicit free list. However, in the allocate function the search algorithm is not first fit but best fit.

Best fit searches for the optimal size in the heap, if available. This means that best fit needs to scan the whole heap (time inefficient), but guarantees, to find the smallest block which fits our requested size. 

Example: there are 3 free blocks in this order: one 300 bytes, one 200 and one 100. We call `allocate(90)`. Best fit will look the blocks one by one, and will end up allocating the 3rd one (100 bytes) because it is the best fit for the 90 bytes we want to allocate. First fit would allocate the first one (300 bytes) as it is a greedy algorithm.

