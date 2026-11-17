# Stack Allocator

Built around a contiguous memory arena, this allocator is designed to split space with minimal fragmentation and support fast LIFO (Last-In, First-Out) allocations.

## Design Evolution & Key Improvements

1. **Strict 8-Byte Alignment (`ARENA_ALIGN`)**
   * Initially, no alignment was enforced. To ensure architectural compatibility and performance, offsets and allocation sizes are now strictly aligned to multiples of 8 using bitwise masking:
     $$\text{actual\_size} = (\text{size} + \text{ARENA\_ALIGN} - 1) \& \sim(\text{ARENA\_ALIGN} - 1)$$

2. **Allocation Metadata Tracking**
   * **The Problem:** Aligning offsets meant the raw requested size differed from the actual bytes consumed in the arena, making it impossible to reliably compute how much space to reclaim during a free operation.
   * **The Solution:** To keep overhead minimal without dynamic metadata allocation, a static internal tracking array (`allocations`) records the aligned size of each block, managed via a stack pointer (`stack_top`). 
   * **Constraint:** Because size tracking relies strictly on the LIFO order of allocation and deallocation, modifying or freeing blocks out of sequence will invalidate the stack invariant and corrupt the arena state.

## Current Structure & Limits
* **Arena Capacity:** Fixed at compile-time (`ARENA_CAP = 4096` bytes).
* **Tracking Capacity:** Bounded statically by `ARENA_CAP / ARENA_ALIGN`.

*Note: This repository is experimental. Further refinements and optimizations will be introduced as the allocator evolves.*