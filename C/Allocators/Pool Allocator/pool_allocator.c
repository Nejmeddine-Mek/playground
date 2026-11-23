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
    if(slot_size == 0)
        slot_size = BYTE_ALIGN;
    pool->slot_size = (slot_size + (BYTE_ALIGN - 1)) & ~(BYTE_ALIGN - 1);

    /**
     * before we proceed, there is an important part to execute
     * if size is not a multiple of slot_size, we shrink the size to the end of the last slot as follows
     */
    size_t num_slots = pool->size / pool->slot_size;
    pool->size = num_slots * pool->slot_size;
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
        return NULL;

    if ((size_t)pool->free_list + sizeof(int) > pool->size)
        return NULL;

    void *ptr = pool->p + pool->free_list;
    int next;
    memcpy(&next, ptr, sizeof(int));
    pool->free_list = next;

    return ptr;

}

int free_slot(pool_t *pool, void *ptr){
    if (!pool || !pool->p || !ptr)
        return 1;
    
    ptrdiff_t diff = (ptrdiff_t)((uint8_t *)ptr - pool->p);

    if (diff < 0 || (size_t)diff >= pool->size)
        return 1;
    /**
     * we simply add that slot as the head of the list
     */
    size_t offset = (size_t)diff;
    if (offset % pool->slot_size != 0)
        return 1;
    
    int next = pool->free_list;
    memcpy(ptr, &next, sizeof(int));
    pool->free_list = (int)offset;

    return 0;

}

int destroy(pool_t *pool){
    if(!pool || !pool->p)
        return 1;
    free(pool->p);
    pool->size = 0;
    pool->free_list = 0;
    pool->slot_size = 0;
    pool->p = NULL;
    return 0;
}