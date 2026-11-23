#ifndef BUMPPTRALLOCATOR_H
#define BUMPPTRALLOCATOR_H

#include <stdint.h>
#define ARENA_MIN 4096
#define BYTE_ALIGN 8

typedef struct{
    uint8_t *arena;
    size_t size;
    size_t offset;
}arena_t;

int init(arena_t *arena, size_t size);
void *allocate(arena_t *arena, size_t size);
int reset(arena_t *arena);
int destroy(arena_t *arena);

#endif