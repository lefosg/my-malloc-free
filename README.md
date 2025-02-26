# My malloc/free

Description:

Codebase is the exact same as with main branch. Implicit free list. However, in the `allocate` function the search algorithm is not first fit but next fit.

Next fit algorithm does not search from the start every time `allocate` is called. It picks up from where it left off last time. This can (probabilistically) be a lot faster than first fit, or in the worst case, same as first fit.