# My malloc/free

## Fast version

Same codebase a the main branch aka implicit free list. To boost speed it does not coalesce or split blocks. For faster allocation we use next fit algorithm (worst case is same as first fit).
