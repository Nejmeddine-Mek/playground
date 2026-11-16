
#include <stdlib.h>
#include "./arena.h"

int init(Arena *arena){
    if(arena == NULL)
        return -1;
    arena->arena = malloc(ARENA_CAP);
    if(!arena->arena)
        return 1;
    arena->size = ARENA_CAP;
    arena->offset = 0;

    return 0;
    
}
void *allocate(Arena *arena, size_t *size){
    if(arena == NULL)
        return NULL;
    if(arena->size - arena->offset < *size)
        return NULL;
    void *result = arena->arena + arena->offset;
    *size = (*size + (ARENA_ALIGN - 1)) & ~(ARENA_ALIGN - 1);
    arena->offset += *size;
    return result;

}
int free_space(Arena *arena,void **ptr, size_t size){
    if(arena == NULL)
        return -1;
    if(*ptr == NULL)
    /*NO MEMORY TO RECLAIM*/
        return -2;
    
    if(arena->offset < size){
    /*more than what's allocated yet we can just set the offset to 0 */
        arena->offset = 0;
        *ptr = NULL;
        /*not a failure, but a warning */
        return 2;
    }
    
    if(arena->arena + arena->offset - size != *ptr)
    /*NOT THE SPACE TO RECLAIM*/
        return 1;

        
    arena->offset -= size;
    *ptr = NULL;
    return 0;
}

int destroy_arena(Arena *arena){
    if(arena == NULL)
        return -1;
    if(arena->arena == NULL)
        return 1;
    free(arena->arena);
    return 0;
}