# Offset-Based Fixed-Size Pool Allocator (C)

A lightweight, fixed-block memory pool allocator in C that stores free-list links **inline inside free slots as byte offsets** (`int`, using `-1` as the end-of-list sentinel) rather than raw pointers.

## Design Overview

* **Arena & Slot Sizing:**
  * Arena size is rounded down to multiples of `ARENA_MIN` (`4096` bytes), with a minimum of `ARENA_MIN`.
  * `slot_size` is clamped to at least `BYTE_ALIGN` (`8` bytes) and rounded up to an `8`-byte boundary.
  * `pool->size` is shrunk to an exact multiple of `slot_size` (`num_slots * slot_size`) so trailing partial-slot slack is never treated as a valid slot.
* **Inline Offset Free List:**
  * Free slots store a 4-byte `int` offset pointing to the next free slot at the start of the slot (`memcpy` is used to avoid alignment/strict-aliasing issues).
  * The tail slot stores `-1`.
  * `pool->free_list` holds the byte offset of the head free slot (`-1` when full/empty).
  * Slots are wired in reverse (`last_slot` down to `0`) so early allocations tend to hand out lower offsets first.
* **Operations:**
  * `init(pool_t *pool, size_t size, size_t slot_size)` — Allocates the arena, aligns `slot_size`, trims to whole slots, and builds the free list.
  * `allocate(pool_t *pool)` — Pops the head offset from `free_list`, updates `free_list` to the next offset, and returns a pointer to the slot (`NULL` if out of memory or invalid).
  * `free_slot(pool_t *pool, void *ptr)` — Validates bounds and slot alignment, pushes `ptr`'s offset onto the front of `free_list`, and links it to the previous head.
  * `reset(pool_t *pool)` — Rebuilds the free list across all slots without freeing or reallocating the backing buffer.
  * `destroy(pool_t *pool)` — Frees the backing buffer and resets pool metadata.


## Example Usage

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double x, y;
    int id;
} entity_t;

int main(void) {
    pool_t pool;
    if (init(&pool, ARENA_MIN, sizeof(entity_t)) != 0) {
        fprintf(stderr, "Failed to initialize pool\n");
        return 1;
    }

    entity_t *a = allocate(&pool);
    entity_t *b = allocate(&pool);
    if (a && b) {
        a->id = 1; a->x = 10.0; a->y = 20.0;
        b->id = 2; b->x = 30.0; b->y = 40.0;
    }

    free_slot(&pool, a);
    a = NULL;

    // Reclaims freed slot
    entity_t *c = allocate(&pool);
    if (c) {
        c->id = 3; c->x = 50.0; c->y = 60.0;
    }

    // Optional: rebuild free list at a phase/batch boundary
    reset(&pool);

    destroy(&pool);
    return 0;
}
```

## Trade-offs & Notes

* **O(1) `allocate` and `free_slot`:** Both operations are constant-time pointer/offset updates with zero external metadata overhead per slot.
* **No