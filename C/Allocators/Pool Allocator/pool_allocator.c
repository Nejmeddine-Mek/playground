#include "pool_allocator.h"
#include <stdlib.h>
#include <string.h>

int init(pool_t *pool, size_t size, size_t slot_size){
    if(!pool)
        return 1;
    /**
     * the arena size is at least 4096 bytes, so we get at least 512 slots
     */
    if(size < ARENA_MIN)
        pool->size = ARENA_MIN;
    else
        pool->size = ARENA_MIN * (size / ARENA_MIN);
    
    pool->p = malloc(pool->size);   
    if(!pool->p)
        return 2;

    /**
     * with aligning on 8 bytes, the slot_size is at least 8 bytes
     */
    pool->slot_size = (slot_size + (BYTE_ALIGN - 1)) & ~(BYTE_ALIGN - 1);

    /**
     * we setup the free list, for this one, we go in reverse order instead of going in order from 0 to n 
     * we first set the tail, then we go up to the head of the list
     */
    int tail = -1;
    memcpy(pool->p + pool->size - pool->slot_size, &tail, sizeof(int));

    for(ptrdiff_t i = pool->size - 2 * pool->slot_size; i >= 0; i -= pool->slot_size){
        int next = i + pool->slot_size;
        memcpy(pool->p + i,&next , sizeof(int));
    }
    pool->free_list = 0;

    return 0;
}



void *allocate(pool_t *pool){
    if(!pool || !pool->p)
        return NULL;
    
    if(pool->free_list == -1)
        return -1;
    void *ptr = pool->p + pool->free_list;
    int next = pool->p[pool->free_list] + 
               pool->p[pool->free_list + 1] << 8 +
               pool->p[pool->free_list + 2] << 16 +
               pool->p[pool->free_list + 3] << 24;
    pool->free_list = next;

    return ptr;

}
int free_slot(pool_t *pool, void *ptr){

}

int destroy(pool_t *pool){

}