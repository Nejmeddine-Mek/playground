#ifndef ARENA_H
#define ARENA_H
#include <stdint.h>


#define ARENA_CAP 4096
#define ARENA_ALIGN 8
typedef struct {
    uint8_t *arena;
    size_t allocations[ARENA_CAP / ARENA_ALIGN];
    size_t stack_top;
    size_t size;
    size_t offset;
}Arena;


int init(Arena *arena);
void *allocate(Arena *arena, size_t size);
int free_space(Arena *arena, void **ptr);
int destroy_arena(Arena *arena);

#endif ARENA_H