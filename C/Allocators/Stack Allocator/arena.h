#ifndef ARENA_H
#define ARENA_H
#include <stdint.h>

#define ARENA_CAP 4000

typedef struct {
    uint8_t *arena;
    size_t size;
    size_t offset;
}Arena;


int init(Arena *arena);
void *allocate(Arena *arena, size_t size);
int free_space(void **ptr, size_t size);
int destroy_arena(Arena *arena);

#endif ARENA_H