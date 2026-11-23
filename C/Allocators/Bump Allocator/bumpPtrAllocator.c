#include "bumpPtrAllocator.h"
#include <stdlib.h>

int init(arena_t *arena, size_t size){
    if(arena == NULL)
        return 1;
    if(size < ARENA_MIN)
        arena->size = ARENA_MIN;
    else
        arena->size = ARENA_MIN * (size / ARENA_MIN);
    
    arena->arena = malloc(arena->size);
    if(!arena->arena)
        return 2;
    arena->offset = 0;

    return 0;
}

void *allocate(arena_t *arena, size_t size){
    if(arena == NULL || arena->arena == NULL)
        return NULL;

    size_t eff_size = size + (BYTE_ALIGN - 1) & ~(BYTE_ALIGN - 1); // compute the aligned size
    if(arena->size - arena->offset < eff_size)
        return NULL;
    
    void *out = arena->arena + arena->offset;
    arena->offset += eff_size;

    return out;
    
}

int reset(arena_t *arena){
    if(!arena || !arena->arena)
        return 1;
    
    arena->offset = 0;
    return 0;
}

int destroy(arena_t *arena){
    if(arena == NULL || arena->arena == NULL)
        return 1;
    
    free(arena->arena);
    arena->arena == NULL;
    arena->size = 0;
    arena->offset = 0;
    return 0;
}