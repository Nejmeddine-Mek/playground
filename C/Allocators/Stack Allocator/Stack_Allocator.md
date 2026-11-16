# Stack Allocator

Built upon a simple idea, we define an arena, and we split space with the least possible fragmentation
initially, no alignment was enforced, but now, we are enforcing offsets to be multiples of 8.
This introduced a problem that shows when freeing space "we can't know how much space to free after aligning the offsets"
so, in order to keep it minimal, we decided to manage the size of the scratch buffer as well, and at allocation time, size is modified to be the exact space requested by the user. and for that reason, changing size after allocation leads to completely mess the algorithm.

the repository is experimental, so changes will be introduced to make the Allocator better. 