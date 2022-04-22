#include "stddef.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "mmu.h"

typedef struct
{
    void *aligned_addr;
} aligned_memory;
aligned_memory *allocate_aligned(size_t size, size_t align);
aligned_memory *default_allocate_aligned(size_t size);
int thread_creator(void (*fn)(void *), void *arg);

int thread_creator(void (*fn)(void *), void *arg)
{
    aligned_memory *memptr = (aligned_memory *)default_allocate_aligned(4096);
    void *stack = memptr->aligned_addr;
    int tid = thread_create(stack);
    if (tid < 0)
    {
        return -1;
    }
    else if (tid == 0)
    {
        (fn)(arg);
        free(memptr);
        exit();
    }

    return tid;
}

aligned_memory *allocate_aligned(size_t size, size_t align)
{
    if (!size || !align || (align & (align - 1)))
        return NULL;
    // e.g. 100 alignment will mask 011 in addresses
    const size_t mask = align - 1;
    // allocate memory for the size + align
    aligned_memory *memptr = (aligned_memory *)malloc(sizeof(aligned_memory) + size + align);
    // allocation failure
    if (!memptr)
        return NULL;
    // calculate the aligned memory address
    memptr->aligned_addr = (void *)((((uint)(memptr + 1)) + mask) & ~mask);
    // e.g. allocated address:0x09 => (0x09 + 0b011) & 0b11100 =
    // 0b10010 & 0b11100 = 0b1000 => aligned address:0x10
    return memptr;
}

aligned_memory *default_allocate_aligned(size_t size)
{
    if (!size)
        return NULL;
    // allocate memory for the size + align
    aligned_memory *memptr = (aligned_memory *)malloc(sizeof(aligned_memory) + size + PGSIZE);
    // allocation failure
    if (!memptr)
        return NULL;
    // round to upper aligned memory address
    memptr->aligned_addr = (void *)(PGROUNDUP((uint)(memptr) + 1));

    return memptr;
}