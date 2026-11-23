#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <stdint.h>

#define ARENA_MIN 4096
#define BYTE_ALIGN 8
/**
 * for this, we shall allocate a specific number of slots, something like CAP / slot_size
 * and in each slot, the first 4 bytes are a pointer to the next free slot, the tail of the list will point to -1
 * we use offsets and not absolute pointers
 */

typedef struct {
    uint8_t *p;
    size_t size;
    size_t slot_size;
    int free_list; // we use int because when the list is empty, it points to -1
    // the allocation works as one slot at a time
}pool_t;

int init(pool_t *pool, size_t size, size_t slot_size);
void *allocate(pool_t *pool);
int free_slot(pool_t *pool, void *ptr);

int destroy(pool_t *pool);


#endif