
#include <stdlib.h>
#include <string.h>
#include "./arena.h"

int init(Arena *arena){
    if(arena == NULL)
        return -1;
    arena->arena = malloc(ARENA_CAP);
    if(!arena->arena)
        return 1;
    arena->size = ARENA_CAP;
    arena->offset = 0;
    arena->stack_top = 0;
    memset(arena->allocations, 0, sizeof(arena->allocations));

    return 0;
    
}
void *allocate(Arena *arena, size_t size){
    if (size == 0) return NULL;

    if(arena == NULL)
        return NULL;

    void *result = arena->arena + arena->offset;
    size_t actual_size = (size + ARENA_ALIGN - 1) & ~(ARENA_ALIGN - 1);

    if(arena->size - arena->offset < actual_size)
        return NULL;
    
    arena->allocations[arena->stack_top++] = actual_size;
    arena->offset += actual_size; 
    return result;

}

int free_space(Arena *arena,void **ptr){
    
    if(arena == NULL)
        return -1;
    if(ptr == NULL || *ptr == NULL)
    /*NO MEMORY TO RECLAIM*/
        return -2;
    if(arena->stack_top == 0)
    /*no space allocated to be freed*/
        return -3;
    // here we have the provisional size we will free
    size_t size = arena->allocations[arena->stack_top - 1];

    if(arena->offset < size){
    /*more than what's allocated yet we can just set the offset to 0 */
        arena->offset = 0;
        arena->stack_top = 0;
        *ptr = NULL;
        /*not a failure, but a warning */
        return 2;
    }
    
    if(arena->arena + arena->offset - size != *ptr)
    /*NOT THE SPACE TO RECLAIM*/
        return 1;

        
    arena->offset -= size;
    arena->stack_top -= 1;
    *ptr = NULL;
    return 0;
}

int destroy_arena(Arena *arena){
    if(arena == NULL)
        return -1;
    if(arena->arena == NULL)
        return 1;
    free(arena->arena);
    arena->arena = NULL; // Prevent double-free risks
    arena->size = 0;
    arena->offset = 0;
    arena->stack_top = 0;
    return 0;
}